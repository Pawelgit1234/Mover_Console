#include "Server.h"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

Server::Server(unsigned short clients_count)
    : clients_count_(clients_count), acceptor_(io_context_), udp_socket_(io_context_, udp::endpoint(udp::v4(), settings::UDP_PORT))
{
    mobs_.reserve(settings::MOB_COUNT);
    movers_.reserve(clients_count);
}

void Server::start()
{
    try
    {
        if (settings::logging)
            logger::log("Server start", logger::LoggerType::INFO);

        tcp::endpoint endpoint(tcp::v4(), settings::TCP_PORT);
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();

        acceptNewConnections();

        while (gameIsOn)
        {
            calculateNextGameObjectsPosition();

            std::vector<Mover> newMovers;
            std::vector<Bullet> newBullets;
            for (auto& s : clients_)
            {
                s->movers_ = movers_;
                s->bullets_ = bullets_;
                s->mobs_ = mobs_;
                s->booster_ = booster_;
                s->newDataToSend = true;

                if (!s->newDataReceived)
                    continue;

                newMovers.emplace_back(s->mover_);
                for (Bullet& b : s->mover_bullets_)
                    newBullets.emplace_back(b);
                s->newDataReceived = false;
            }

            movers_ = newMovers;
            bullets_ = newBullets;
        }
    }
    catch (const std::exception& e)
    {
        if (settings::logging)
            logger::log("Server error: " + std::string(e.what()), logger::LoggerType::CRITICAL);
    }
}

void Server::calculateNextGameObjectsPosition()
{   
    if (mobs_.empty())
    {
        for (int i = 0; i < settings::MOB_COUNT; i++)
        {
            Mob mob;
            mobs_.emplace_back(mob);
        }
    }

    for (auto& client : clients_)
    {
        movers_.emplace_back(client->mover_);

        for (Bullet& b : client->bullets_)
            bullets_.emplace_back(b);
    }

    for (Mob& mob : mobs_)
        mob.calculateNextCoordinate(movers_);

    for (Bullet& b : bullets_)
    {
        b.move();
        if (map[b.getY()][b.getX()] == BLOCK)
            b.setXY(-1, -1);
        else
        {
            auto mobIt = std::remove_if(mobs_.begin(), mobs_.end(), [&b](const Mob& mob) {
                return b.getX() == mob.getX() && b.getY() == mob.getY();
                });

            if (mobIt != mobs_.end())
            {
                mobs_.erase(mobIt, mobs_.end());
                if (!booster_.is_pierce())
                    b.setXY(-1, -1);
            }
        }
    }

    bullets_.erase(std::remove_if(bullets_.begin(), bullets_.end(), [](const Bullet& b) {
        return b.getX() == -1 && b.getY() == -1;
        }), bullets_.end());
}

void Server::acceptNewConnections()
{
    if (settings::logging)
        logger::log("Accepting of new connections started.\n", logger::LoggerType::INFO);

    while (true)
    {
        auto session = std::make_shared<ClientSession>(io_context_, udp_socket_);
        clients_.emplace_back(session);

        acceptor_.accept(session->tcp_socket_);

        if (settings::logging)
            logger::log(session->tcp_socket_.remote_endpoint().address().to_string() + ':' + std::to_string(session->tcp_socket_.remote_endpoint().port()) + " connected", logger::LoggerType::INFO);

        std::thread([session, this]() { session->handleClient(); }).detach();

        while (!session->isReady)
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (clients_.size() == clients_count_)
        {
            // TODO: do later to a thread pool
            for (auto& s : clients_)
            {
                message::Message startMsg;
                startMsg.header.type = message::MessageType::START;
                s->tcpSendData(startMsg);
            }

            gameIsOn = true;

            if (settings::logging)
                logger::log("Game started.", logger::LoggerType::INFO);

            break;
        }
    }
}

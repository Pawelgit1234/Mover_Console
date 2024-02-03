#include "Server.h"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

Server::Server(unsigned short clients_count) : clients_count_(clients_count)
{
    mobs_.reserve(settings::MOB_COUNT);
    movers_.reserve(clients_count);
}

void Server::start(unsigned short port)
{
    try
    {
        tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();

        while (true)
        {
            tcp::socket tcp_socket(io_context_);
            acceptor_.accept(tcp_socket);
            std::cout << tcp_socket.remote_endpoint().address().to_string() << ':' << tcp_socket.remote_endpoint().port() << " connected" << std::endl;

            udp::endpoint udp_endpoint(udp::v4(), port);
            udp::socket udp_socket(io_context_, udp_endpoint);
            ClientSession session(udp_socket, tcp_socket, udp_endpoint);
            clients_.emplace_back(session);

            message::Message mapMsg;
            for (std::vector<Map>& row : map)
                for (Map& c : row)
                    mapMsg.add<Map>(c);

            std::thread([&]()
                {
                    session.handleClient();
                }).detach();

            if (clients_.size() >= clients_count_)
            {                
                for (auto& s : clients_)
                {
                    
                }

                game_is_on = true;
                break;
            }
        }

        while (game_is_on)
            calculateNextGameObjectsPosition();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Server Error: " << e.what() << std::endl;
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
        movers_.emplace_back(client.mover_);

        for (Bullet& b : client.bullets_)
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

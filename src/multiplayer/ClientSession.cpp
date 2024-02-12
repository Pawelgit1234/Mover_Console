#include "ClientSession.h"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

ClientSession::ClientSession(boost::asio::io_context& io_context, udp::socket& udp_socket)
    : udp_socket_(udp_socket), tcp_socket_(io_context) {}

void ClientSession::handleClient()
{
    try
    {
        if (settings::logging)
        {
            // udp закрыт
            logger::log((tcp_socket_.is_open() ? "tcp socket is open" : "tcp socket is close"), (tcp_socket_.is_open() ? logger::LoggerType::INFO : logger::LoggerType::CRITICAL));
            logger::log((udp_socket_.is_open() ? "udp socket is open" : "udp socket is close"), (udp_socket_.is_open() ? logger::LoggerType::INFO : logger::LoggerType::CRITICAL));
        }

        message::Message mapMsg;
        mapMsg.header.type = message::MessageType::MAP;
        for (std::vector<Map>& row : map)
            for (Map& c : row)
                mapMsg.add<Map>(c);

        tcpSendData(mapMsg);

        if (settings::logging)
            logger::log("Map sent to client. Is ready.\n", logger::LoggerType::INFO);

        isReady = true;

        while (true)
        {
            if (newDataToSend)
                sendGameData();

            receiveData();
            handleMessage();
        }
    }
    catch (std::exception& e)
    {
        if (settings::logging)
            logger::log("Exception in handleClient: " + std::string(e.what()), logger::LoggerType::CRITICAL);
    }
}

void ClientSession::tcpSendData(message::Message& msg)
{
    boost::asio::write(tcp_socket_, boost::asio::buffer(&msg.header, sizeof(msg.header)));
    boost::asio::write(tcp_socket_, boost::asio::buffer(msg.body));
}

void ClientSession::udpSendData(message::Message& msg)
{
    udp_socket_.send_to(boost::asio::buffer(&msg.header, sizeof(msg.header)), udp_remote_endpoint_);
    udp_socket_.send_to(boost::asio::buffer(msg.body), udp_remote_endpoint_);
}

void ClientSession::receiveData()
{
    try
    {
        if (tcp_socket_.available())
        {
            message::MessageHeader header;
            boost::asio::read(tcp_socket_, boost::asio::buffer(&header, sizeof(header)));

            message::Message msg;
            msg.header = header;
            msg.body.resize(header.size);
            boost::asio::read(tcp_socket_, boost::asio::buffer(msg.body));
            messages_.emplace_back(msg);
        }

        if (udp_socket_.available())
        {
            message::MessageHeader header;
            size_t bytes_transferred = udp_socket_.receive_from(boost::asio::buffer(&header, sizeof(header)), udp_remote_endpoint_);

            if (bytes_transferred > 0)
            {
                message::Message msg;
                msg.header = header;
                msg.body.resize(header.size);

                bytes_transferred = udp_socket_.receive_from(boost::asio::buffer(msg.body), udp_remote_endpoint_);
                if (bytes_transferred > 0)
                    messages_.emplace_back(msg);
            }
        }
    }
    catch (std::exception& e)
    {
        if (settings::logging)
            logger::log("Exception in receiveData(): " + std::string(e.what()), logger::LoggerType::CRITICAL);
    }
}

void ClientSession::handleMessage()
{
	while (!messages_.empty())
	{
        newDataReceived = true;
        message::Message& msg = messages_.front();

        if (msg.header.type == message::MessageType::MOVER)
            mover_ = msg.get<Mover>();
        else if (msg.header.type == message::MessageType::BULLETS)
        {
            std::vector<Bullet> receivedBullets;
            while (msg.header.size)
                receivedBullets.emplace_back(msg.get<Bullet>());
            bullets_ = receivedBullets;
        }

		messages_.pop_front();
	}
}

void ClientSession::sendGameData()
{
    message::Message moversMsg;
    moversMsg.header.type = message::MessageType::MOVERS;
    for (Mover& m : movers_)
        moversMsg.add<Mover>(m);

    message::Message bulletsMsg;
    bulletsMsg.header.type = message::MessageType::BULLETS;
    for (Bullet& b : bullets_)
        bulletsMsg.add<Bullet>(b);

    message::Message mobsMsg;
    mobsMsg.header.type = message::MessageType::MOBS;
    for (Mob& m : mobs_)
        mobsMsg.add<Mob>(m);

    message::Message boosterMsg;
    boosterMsg.header.type = message::MessageType::BOOSTER;
    boosterMsg.add<Booster>(booster_);

    udpSendData(moversMsg);
    udpSendData(bulletsMsg);
    udpSendData(mobsMsg);
    udpSendData(boosterMsg);

    newDataToSend = false;
}

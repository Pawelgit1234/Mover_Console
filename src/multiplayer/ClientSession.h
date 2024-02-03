#pragma once

#include <boost/asio.hpp>

#include <deque>
#include <vector>

#include "../objects/Mover.h"
#include "../objects/Bullet.h"
#include "Message.h"

class Server;

class ClientSession
{
public:
    ClientSession(boost::asio::ip::udp::socket& udp_socket, boost::asio::ip::tcp::socket& tcp_socket, const boost::asio::ip::udp::endpoint& udp_endpoint);

public:
    void handleClient();

    void tcpSendData(message::Message& msg);
    void udpSendData(message::Message& msg);

    void receiveData();

private:
    boost::asio::ip::udp::socket& udp_socket_;
    boost::asio::ip::udp::endpoint udp_endpoint_;
    boost::asio::ip::tcp::socket& tcp_socket_;
    std::deque<message::Message> messages_;

    Mover mover_;
    std::vector<Bullet> bullets_;

    bool newDataToSend = false;
    bool newDataToReceive = false;

    friend class Server;
};

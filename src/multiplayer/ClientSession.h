#pragma once

#include <boost/asio.hpp>

#include <deque>
#include <vector>
#include <memory>
#include <thread>

#include "../objects/Mover.h"
#include "../objects/Bullet.h"
#include "../objects/Mob.h"
#include "../objects/Booster.h"
#include "Message.h"
#include "../utils/Logger.h"

class Server;

class ClientSession : public std::enable_shared_from_this<ClientSession>
{
public:
    ClientSession(boost::asio::io_context& io_context, boost::asio::ip::udp::socket& udp_socket);

public:
    void handleClient();

    void tcpSendData(message::Message& msg);
    void udpSendData(message::Message& msg);

private:
    void receiveData();
    void handleMessage();
    void sendGameData();

    boost::asio::ip::udp::socket& udp_socket_;
    boost::asio::ip::udp::endpoint udp_remote_endpoint_;
    boost::asio::ip::tcp::socket tcp_socket_;
    std::deque<message::Message> messages_;

    // ClientSession receive
    mutable Mover mover_;
    mutable std::vector<Bullet> mover_bullets_;

    // ClientSession send
    mutable std::vector<Mover> movers_; 
    mutable std::vector<Bullet> bullets_;
    mutable std::vector<Mob> mobs_;
    mutable Booster booster_; 

    bool newDataToSend = false;
    bool newDataReceived = false;
    bool isReady = false;

    friend class Server;
};

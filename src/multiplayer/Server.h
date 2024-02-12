#pragma once

#include <boost/asio.hpp>

#include <deque>
#include <vector>
#include <iostream>
#include <thread>
#include <memory>
#include <mutex>

#include "ClientSession.h"
#include "../objects/Bullet.h"
#include "../objects/Map.h"
#include "../objects/Booster.h"
#include "../objects/Mover.h"
#include "../objects/Mob.h"
#include "../settings.h"
#include "Message.h"
#include "../utils/Utils.h";
#include "../utils/Logger.h"

class Server final
{
public:
	Server(unsigned short clients_count);

public:
	void start();

private:
	void calculateNextGameObjectsPosition();
	void acceptNewConnections();

	boost::asio::io_context io_context_;
	boost::asio::ip::tcp::acceptor acceptor_;
	boost::asio::ip::udp::socket udp_socket_;
	std::deque<std::shared_ptr<ClientSession>> clients_;

	std::vector<Bullet> bullets_;
	std::vector<Mover> movers_;
	std::vector<Mob> mobs_;
	Booster booster_;

	unsigned short clients_count_;
	bool gameIsOn = false;
};
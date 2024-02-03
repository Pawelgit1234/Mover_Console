#pragma once

#include <boost/asio.hpp>

#include <deque>
#include <vector>
#include <iostream>
#include <thread>

#include "ClientSession.h"
#include "../objects/Bullet.h"
#include "../objects/Map.h"
#include "../objects/Booster.h"
#include "../objects/Mover.h"
#include "../objects/Mob.h"
#include "../settings.h"
#include "Message.h"
#include "../utils/Utils.h";

class Server final
{
public:
	Server(unsigned short clients_count);

public:
	void start(unsigned short port);

private:
	void calculateNextGameObjectsPosition();

	boost::asio::io_context io_context_;
	boost::asio::ip::tcp::acceptor acceptor_{io_context_};
	std::deque<ClientSession> clients_;

	std::vector<Bullet> bullets_;
	std::vector<Mover> movers_;
	std::vector<Mob> mobs_;
	Booster booster_;

	unsigned short clients_count_;
	bool game_is_on = false;
};
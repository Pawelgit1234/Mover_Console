#pragma once

#include <boost/asio.hpp>

#include <string>
#include <vector>
#include <deque>
#include <thread>

#include "../objects/Bullet.h"
#include "../objects/Map.h"
#include "../objects/Booster.h"
#include "../objects/Mover.h"
#include "../objects/Mob.h"
#include "../utils/Keyboard.h"
#include "Message.h"

class Client final
{
public:
	Client() = default;

public:
	void connect(const std::string& ip, unsigned short port);

private:
	void play();
	void receiveMessage();
	void sendMessage();

	boost::asio::io_context io_context_;
	boost::asio::ip::tcp::socket tcp_socket_{io_context_};
	boost::asio::ip::udp::socket udp_socket_{io_context_};
	boost::asio::ip::tcp::endpoint tcp_endpoint_;
	boost::asio::ip::udp::endpoint udp_endpoint_;

	Mover mover_;
	std::vector<Bullet> bullets_;
	std::vector<std::vector<Map>> map_;
	std::deque<message::Message> messages_;

	bool game_is_on = false;
};
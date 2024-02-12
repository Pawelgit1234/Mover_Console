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
#include "../objects/Booster.h"
#include "../utils/Keyboard.h"
#include "../draw/Draw.h"
#include "Message.h"
#include "../utils/Logger.h"

class Client final
{
public:
	Client();

public:
	void connect(const std::string& ip);

private:
	void play();
	void receiveData();
	void tcpSendData(message::Message& msg);
	void udpSendData(message::Message& msg);
	void handleMessage();
	void sendData();

	boost::asio::io_context io_context_;
	boost::asio::ip::tcp::socket tcp_socket_;
	boost::asio::ip::udp::socket udp_socket_;
	boost::asio::ip::tcp::endpoint tcp_endpoint_;
	boost::asio::ip::udp::endpoint udp_endpoint_;

	// Client receive
	mutable std::vector<Bullet> bullets_;
	mutable std::vector<Mob> mobs_;
	mutable std::vector<std::vector<Map>> map_;
	mutable Booster booster_;
	mutable std::vector<Mover> movers_;

	// Client send
	mutable Mover mover_; 
	mutable std::vector<Bullet> mover_bullets_;

	mutable std::deque<message::Message> messages_;

	Clock clock_;

	bool gameIsOn = false;
	bool newDataToSend = false;
};
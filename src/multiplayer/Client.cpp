#include "Client.h"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

Client::Client() : tcp_socket_(io_context_), udp_socket_(io_context_) {}

void Client::connect(const std::string& ip)
{
	try
	{
		tcp_endpoint_ = tcp::endpoint(boost::asio::ip::address::from_string(ip), settings::TCP_PORT);
		tcp_socket_.connect(tcp_endpoint_);
        udp_endpoint_ = udp::endpoint(boost::asio::ip::address::from_string(ip), settings::UDP_PORT);
		udp_socket_.connect(udp_endpoint_);

		if (settings::logging)
		{
			logger::log((tcp_socket_.is_open() ? "tcp socket is open" : "tcp socket is close"), (tcp_socket_.is_open() ? logger::LoggerType::INFO : logger::LoggerType::CRITICAL));
			logger::log((udp_socket_.is_open() ? "udp socket is open" : "udp socket is close"), (udp_socket_.is_open() ? logger::LoggerType::INFO : logger::LoggerType::CRITICAL));
			logger::log("Connected to server.", logger::LoggerType::INFO);
		}
		
		while (true)
		{
			if (gameIsOn)
				play();

			if (newDataToSend)
				sendData();

			receiveData();
			handleMessage();
		}
	}
	catch (const std::exception& e)
	{
		if (settings::logging)
			logger::log("Client error: " + std::string(e.what()), logger::LoggerType::CRITICAL);
	}
}

void Client::play()
{
	switch (pressed())
	{
	case Keyboard::W:
		mover_.up();
		newDataToSend = true;
		break;
	case Keyboard::A:
		mover_.left();
		newDataToSend = true;
		break;
	case Keyboard::S:
		mover_.down();
		newDataToSend = true;
		break;
	case Keyboard::D:
		mover_.right();
		newDataToSend = true;
		break;
	case Keyboard::M:
		mover_.setXY(settings::CONSOLE_MIDDLE_X, settings::CONSOLE_MIDDLE_Y);
		newDataToSend = true;
		break;
	case Keyboard::E:
		settings::extra_info = !settings::extra_info;
		newDataToSend = true;
		break;
	case Keyboard::ARROW_UP:
		mover_bullets_.emplace_back(mover_.getX(), mover_.getY(), BulletDirection::UP);
		newDataToSend = true;
		break;
	case Keyboard::ARROW_DOWN:
		mover_bullets_.emplace_back(mover_.getX(), mover_.getY(), BulletDirection::DOWN);
		newDataToSend = true;
		break;
	case Keyboard::ARROW_LEFT:
		mover_bullets_.emplace_back(mover_.getX(), mover_.getY(), BulletDirection::LEFT);
		newDataToSend = true;
		break;
	case Keyboard::ARROW_RIGHT:
		mover_bullets_.emplace_back(mover_.getX(), mover_.getY(), BulletDirection::RIGHT);
		newDataToSend = true;
		break;
	case Keyboard::NOTHING:
		break;
	}
	std::vector<Mover> allMovers;
	allMovers = movers_;
	allMovers.push_back(mover_);

	std::vector<Bullet> allBullets;
	allBullets = bullets_;
	for (Bullet& b : mover_bullets_)
		allBullets.push_back(b);

	drawConsoleFrame(allMovers, mobs_, allBullets, clock_, booster_);
}

void Client::receiveData()
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
		size_t bytes_transferred = udp_socket_.receive(boost::asio::buffer(&header, sizeof(header)));

		if (bytes_transferred > 0)
		{
			message::Message msg;
			msg.header = header;
			msg.body.resize(header.size);

			bytes_transferred = udp_socket_.receive(boost::asio::buffer(msg.body));
			if (bytes_transferred > 0)
				messages_.emplace_back(msg);
		}
	}
}

void Client::tcpSendData(message::Message& msg)
{
	boost::asio::write(tcp_socket_, boost::asio::buffer(&msg.header, sizeof(msg.header)));
	boost::asio::write(tcp_socket_, boost::asio::buffer(msg.body));
}

void Client::udpSendData(message::Message& msg)
{
	udp_socket_.send_to(boost::asio::buffer(&msg.header, sizeof(msg.header)), udp_endpoint_);
	udp_socket_.send_to(boost::asio::buffer(msg.body), udp_endpoint_);
}

void Client::handleMessage()
{
	while (!messages_.empty())
	{
		message::Message& msg = messages_.front();
		if (msg.header.type == message::MessageType::MAP)
		{
			std::vector<std::vector<Map>> receivedMap;
			receivedMap.reserve(settings::CONSOLE_HEIGHT);

			for (unsigned short y = 0; y < settings::CONSOLE_HEIGHT; ++y)
			{
				std::vector<Map> row;
				row.reserve(settings::CONSOLE_WIDTH);
				for (unsigned short x = 0; x < settings::CONSOLE_WIDTH; ++x)
					row.push_back(msg.get<Map>());
				receivedMap.push_back(row);
			}

			map_ = receivedMap;

			if (settings::logging)
				logger::log("Received map.", logger::LoggerType::INFO);
		}
		else if (msg.header.type == message::MessageType::START)
		{
			gameIsOn = true;
			clock_.clear();

			if (settings::logging)
				logger::log("Game started.", logger::LoggerType::INFO);
		}
		else if (msg.header.type == message::MessageType::MOVERS)
		{
			std::vector<Mover> receivedMovers;
			while (msg.header.size)
				receivedMovers.emplace_back(msg.get<Mover>());
			movers_ = receivedMovers;
		}
		else if (msg.header.type == message::MessageType::BULLETS)
		{
			std::vector<Bullet> receivedBullets;
			while (msg.header.size)
				receivedBullets.emplace_back(msg.get<Bullet>());
			bullets_ = receivedBullets;
		}
		else if (msg.header.type == message::MessageType::MOBS)
		{
			std::vector<Mob> receivedMobs;
			while (msg.header.size)
				receivedMobs.emplace_back(msg.get<Mob>());
			mobs_ = receivedMobs;
		}
		else if (msg.header.type == message::MessageType::BOOSTER)
			booster_ = msg.get<Booster>();

		messages_.pop_front();
	}
}

void Client::sendData()
{
	message::Message moverMsg;
	moverMsg.header.type = message::MessageType::MOVER;
	moverMsg.add<Mover>(mover_);

	message::Message bulletsMsg;
	bulletsMsg.header.type = message::MessageType::BULLETS;
	for (Bullet& b : mover_bullets_)
		bulletsMsg.add<Bullet>(b);

	udpSendData(moverMsg);
	udpSendData(bulletsMsg);
}

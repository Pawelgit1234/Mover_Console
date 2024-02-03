#include "Client.h"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

void Client::connect(const std::string& ip, unsigned short port)
{
	try
	{
		tcp_endpoint_ = tcp::endpoint(boost::asio::ip::address::from_string(ip), port);
		tcp_socket_.connect(tcp_endpoint_);
        udp_endpoint_ = udp::endpoint(boost::asio::ip::address::from_string(ip), port);
		udp_socket_.connect(udp_endpoint_);
        
        std::thread([&]() {
            while (true)
                receiveMessage();
            }).detach();

        std::thread([&]() {
            while (true)
                sendMessage();
            }).detach();
		
		while (true)
			play();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Client Error: " << e.what() << std::endl;
	}
}

void Client::play()
{
	if (game_is_on)
	{
		while (true)
		{
			switch (pressed())
			{
			case Keyboard::W:
				mover_.up();
				break;
			case Keyboard::A:
				mover_.left();
				break;
			case Keyboard::S:
				mover_.down();
				break;
			case Keyboard::D:
				mover_.right();
				break;
			case Keyboard::M:
				mover_.setXY(settings::CONSOLE_MIDDLE_X, settings::CONSOLE_MIDDLE_Y);
				break;
			case Keyboard::E:
				settings::extra_info = !settings::extra_info;
				break;
			case Keyboard::ARROW_UP:
				bullets_.emplace_back(mover_.getX(), mover_.getY(), BulletDirection::UP);
				break;
			case Keyboard::ARROW_DOWN:
				bullets_.emplace_back(mover_.getX(), mover_.getY(), BulletDirection::DOWN);
				break;
			case Keyboard::ARROW_LEFT:
				bullets_.emplace_back(mover_.getX(), mover_.getY(), BulletDirection::LEFT);
				break;
			case Keyboard::ARROW_RIGHT:
				bullets_.emplace_back(mover_.getX(), mover_.getY(), BulletDirection::RIGHT);
				break;
			case Keyboard::NOTHING:
				break;
			}

			
		}
	}
}

void Client::receiveMessage()
{
	if (tcp_socket_.available())
	{
		
	}

	if (udp_socket_.available())
	{

	}
}

void Client::sendMessage()
{
    // TODO: check that data change
}

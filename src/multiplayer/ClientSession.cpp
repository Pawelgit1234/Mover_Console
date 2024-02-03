#include "ClientSession.h"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

ClientSession::ClientSession(udp::socket& udp_socket, tcp::socket& tcp_socket, const udp::endpoint& udp_endpoint)
    : tcp_socket_(tcp_socket), udp_socket_(udp_socket), udp_endpoint_(udp_endpoint) {}

void ClientSession::handleClient()
{

}

void ClientSession::tcpSendData(message::Message& msg)
{
    boost::asio::write(tcp_socket_, boost::asio::buffer(&msg.header, sizeof(msg.header)));
    boost::asio::write(tcp_socket_, boost::asio::buffer(msg.body));
}

void ClientSession::udpSendData(message::Message& msg)
{
    udp_socket_.send_to(boost::asio::buffer(&msg.header, sizeof(msg.header)), udp_endpoint_);
    udp_socket_.send_to(boost::asio::buffer(msg.body), udp_endpoint_);
}

void ClientSession::receiveData()
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

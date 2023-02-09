#include "../includes/ircserver.h"

void IrcServer::onNewConnection(std::size_t id)
{
	std::cout << '[' << id << "]new client !" << std::endl;
}

void IrcServer::onNewData(std::size_t id, const std::string& data)
{
	std::cout << '[' << id << "]received: \'" << data << '\'' << std::endl;
}

void IrcServer::onClientDisconnect(std::size_t id)
{
	std::cout << '[' << id << "]disconnected" << std::endl;
}

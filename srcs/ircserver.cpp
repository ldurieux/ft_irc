#include "../includes/ircserver.h"

void IrcServer::onNewConnection(std::size_t id)
{
	(void)id;
}

void IrcServer::onNewData(std::size_t id, const std::string& data)
{
	(void)id;
	(void)data;
}

void IrcServer::onClientDisconnect(std::size_t id)
{
	(void)id;
}

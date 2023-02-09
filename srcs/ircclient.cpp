#include "../includes/ircclient.h"

IrcClient::IrcClient(const std::string& username, const std::string& nickname) : BaseClient(username, nickname)
{

}

void IrcClient::onNewData(const std::string& data)
{
	std::cout << ">> '" << data << '\'' << std::endl;
}


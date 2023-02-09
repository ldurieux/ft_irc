#include "../includes/ircclient.h"

IrcClient::IrcClient(const std::string& username, const std::string& nickname) : BaseClient(username, nickname)
{
	_cmds.push_back("PING ");
	_cmds.push_back("PRIVMSG ");
}

void IrcClient::onNewData(const std::string& data)
{
	std::string sender;
	std::cout << ">> '" << data << '\'' << std::endl;

	std::string::const_iterator it = data.begin();
	if (*it == ':')
	{
		while (*it != '!')
		{
			if (it == data.end())
			{
				std::cout << "Invalid command" << std::endl;
				return;
			}
			it++;
		}
		sender = std::string(data.begin() + 1, it);
		while (*it != ' ')
		{
			if (it == data.end())
			{
				std::cout << "Invalid command" << std::endl;
				return;
			}
			it++;
		}
		it++;
	}
	std::string cmd(it, data.end());

	int cmd_idx = getCmd(cmd);
	if (cmd_idx < 0)
	{
		std::cout << "Command not found: " << cmd << std::endl;
		return ;
	}

	switch(cmd_idx)
	{
	case 0: onPing(cmd); break;
	case 1: onPrivMsg(cmd); break;
	}
}

int IrcClient::getCmd(const std::string &data)
{
	int idx = 0;
	std::vector<std::string>::iterator it;
	for (it = _cmds.begin(); it != _cmds.end(); it++, idx++)
		if (data.compare(0, it->size(), *it) == 0)
			return idx;
	return -1;
}

void IrcClient::onPing(const std::string &data)
{
	sendData("PONG " + std::string(data.begin() + 5, data.end()));
}

void IrcClient::onPrivMsg(const std::string &data)
{
	std::string::const_iterator it = data.begin();
	while (*it != ':')
	{
		if (it == data.end())
			return;
		it++;
	}
	it++;
	if (it == data.end())
		return;
	sendData(std::string(it, data.end()));
}


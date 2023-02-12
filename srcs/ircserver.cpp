#include "../includes/ircserver.h"
#include "../includes/user.h"
#include <list>
#include <cstddef>
#include <ostream>
#include <string>

void IrcServer::onNewConnection(std::size_t id)
{
	_userList.push_back(User(id));
	std::cout << '[' << id << "]new client !" << std::endl;
}

void IrcServer::onNewData(std::size_t id, const std::string& data)
{
	std::list<User>::iterator it = findUser(id);
	if (it == _userList.end())
	{
		std::cout << "Couldn't find the user id " << id << " " << __FUNCTION__ << std::endl;
		return;
	}
	User* user = &*it;

	std::size_t i;
	for (i = 0; i < data.length(); i++)
		if (data.at(i) < 65 || data.at(i) > 90)
			break ;
	std::string requestType(data, 0, i);

	while (std::isspace(data[i]) && i < data.size())
		i++;
	std::string content(data.begin() + i, data.end());

	std::cout << '[' << id << "]request: \'" << requestType
			  << "\' content: \'" << content << '\'' << std::endl;

	std::string instruction[10] = {"JOIN", "NICK", "PASS", "USER", "QUIT", "PART", "PRIVMSG", "NOTICE", "MODE", "KICK"};
	for (i = 0; i < 10; i++)
	{
		if (requestType == instruction[i])
			break ;
	}
	switch (i)
	{
	case 0: onJoinChannel(user, content); break;
	case 1: onNick(user, content); break;
	case 2: onPass(user, content); break;
	case 3: onUser(user, content); break;
	case 4: onQuit(user, content); break;
	case 5: onPart(user, content); break;
	case 6: onPrivmsg(user, content); break;
	case 7: onNotice(user, content); break;
	case 8: onMode(user, content); break;
	case 9: onKick(user, content); break;
	default:
		std::cout << '\'' << requestType << "\' not handled" << std::endl;
	}
}

void IrcServer::onClientDisconnect(std::size_t id)
{
	std::cout << '[' << id << "]disconnected" << std::endl;

	onDisconnect(id);
}

std::string	grab_first_arg(const std::string& content)
{
	std::size_t n = 0;
	while (n < content.length() && !std::isspace(content[n]))
		n++;
	std::string first_arg(content, 0, n);
	return (first_arg);
}

void IrcServer::onJoinChannel(User* user, const std::string& content)
{
	std::string channelName = grab_first_arg(content);
	if (!joinChannel(user, channelName))
		return;

	sendTo(user->getId(), getMsgPrefix(user) + " JOIN :" + channelName);
}

void IrcServer::onNick(User* from, const std::string& content)
{
	std::string nickname = grab_first_arg(content);
	from->setNickname(nickname);
}

void IrcServer::onPass(User* user, const std::string& content)
{
	std::string pass = grab_first_arg(content);
	if (password() == pass)
		user->setAuthenticated(true);
	else
		std::cout << "Invalid password: " << pass << std::endl;
}

void IrcServer::onUser(User* user, const std::string& content)
{
	std::string username = grab_first_arg(content);
	user->setUsername(username);

	if (!user->authenticated())
	{
		disconnectClient(user->getId());
		onDisconnect(user->getId());
		return;
	}

	sendTo(user->getId(), "PING :ft_irc");
}

void IrcServer::onQuit(User* user, const std::string& content)
{
	std::string msg = grab_first_arg(content);
	(void)msg;

	disconnectClient(user->getId());
	onDisconnect(user->getId());
}

void IrcServer::onPart(User* user, const std::string& content)
{
	std::string::const_iterator it = content.begin();
	std::vector<std::string> channels;
	std::string message;

	while (isspace(*it) && it != content.end())
		it++;
	if (it == content.end())
		return;

	std::string::const_iterator it2 = it;
	for (; it2 != content.end(); it2++)
	{
		if (*it2 == ',' || std::isspace(*it2))
		{
			if (it2 - it < 2)
				return;
			channels.push_back(std::string(it, it2));
			it = it2 + 1;
			if (std::isspace(*it2))
				break;
		}
	}
	if (it2 == content.end())
		return;
	it = it2;
	while (*it != ':' && it != content.end())
		it++;
	if (it == content.end())
		return;

	message = std::string(it, content.end());
	if (message.size() < 1)
		return;
	for (std::size_t i = 0; i < channels.size(); i++)
	{
		std::list<Chanel>::iterator chanIt = findChannel(channels[i]);
		if (chanIt == _channelList.end())
			continue;
		std::vector<User*> users = chanIt->getUsers();
		std::vector<User*>::iterator it = users.begin();
		for (; it != users.end(); it++)
			sendTo((*it)->getId(), getMsgPrefix(user) + " PART " + channels[i] + " :" + message);
	}
}

void IrcServer::onPrivmsg(User* user, const std::string& content)
{
	std::size_t n = 0;
	while (n < content.length() && !std::isspace(content[n]))
		n++;
	std::string dest(content, 0, n);
	std::string message(content, n + 2, std::string::npos);

	std::list<Chanel>::iterator chanIt = findChannel(dest);
	if (chanIt->isInChannel(user) == false)
		return;
	if (chanIt != _channelList.end())
	{
		if (chanIt->isBanned(user->getUsername()))
			return;
		std::vector<User*> users = chanIt->getUsers();
		std::vector<User*>::iterator it = users.begin();
		for (; it != users.end(); it++)
		{
			User* tmp = *it;
			if (tmp == user)
				continue;
			sendTo(tmp->getId(), getMsgPrefix(user) + " PRIVMSG " + dest + " :" + message);
		}
		return;
	}

	std::list<User>::iterator userIt = findUser(dest);
	if (userIt != _userList.end())
	{
		User userDest = *userIt;
		sendTo(userDest.getId(), getMsgPrefix(user) + " PRIVMSG " + dest + " :" + message);
		return ;
	}
	std::cout << "user or channel not found: " << dest << std::endl;
}

void IrcServer::onNotice(User* user, const std::string& content)
{
	std::size_t n = 0;
	while (n < content.length() && !std::isspace(content[n]))
		n++;
	std::string dest(content, 0, n);
	std::string message(content, n + 2, std::string::npos);

	std::list<Chanel>::iterator chanIt = findChannel(dest);
	if (chanIt->isInChannel(user) == false)
		return;
	if (chanIt != _channelList.end())
	{
		if (chanIt->isBanned(user->getUsername()))
			return;
		std::vector<User*> users = chanIt->getUsers();
		std::vector<User*>::iterator it = users.begin();
		for (; it != users.end(); it++)
		{
			User* tmp = *it;
			if (tmp == user)
				continue;
			sendTo(tmp->getId(), getMsgPrefix(user) + " NOTICE " + dest + " :" + message);
		}
		return;
	}

	std::list<User>::iterator userIt = findUser(dest);
	if (userIt != _userList.end())
	{
		User userDest = *userIt;
		sendTo(userDest.getId(), getMsgPrefix(user) + " NOTICE " + dest + " :" + message);
		return ;
	}
	std::cout << "user or channel not found: " << dest << std::endl;
}

void IrcServer::onMode(User* user, const std::string& content)
{
	std::size_t start;
	std::size_t n = 0;
	while (n < content.length() && !std::isspace(content[n]))
		n++;
	if (n >= content.length())
		return;
	std::string channel(content, 0, n);

	start = ++n;
	while (n < content.length() && !std::isspace(content[n]))
		n++;
	std::string action(content, start, n - start);

	start = ++n;
	while (n < content.length() && !std::isspace(content[n]))
		n++;
	std::string userStr(content, start, n - start);

	(void)user;
	//-------------DEBUG-----------------------
	std::cout << "MODE:" << std::endl;
	std::cout << "channel: " << channel << '\'' << std::endl;
	std::cout << "action: " << action << '\'' << std::endl;
	std::cout << "user: " << userStr << '\'' << std::endl;
	//-----------------------------------------
	
}

void IrcServer::onDisconnect(std::size_t id)
{
	std::list<User>::iterator it = findUser(id);
	if (it == _userList.end())
	{
		std::cout << "Couldn't find the user id " << id << " " << __FUNCTION__ << std::endl;
		return;
	}
	User* user = &*it;

	std::list<Chanel>::iterator chanIt = _channelList.begin();
	for (; chanIt != _channelList.end(); chanIt++)
		chanIt->removeUser(user);
	std::list<User>::iterator userIt = _userList.begin();
	for (; userIt != _userList.end(); userIt++)
	{
		if (userIt->getId() == user->getId())
		{
			_userList.erase(userIt);
			break;
		}
	}
}

void IrcServer::onKick(User* user, const std::string& content)
{
	std::size_t start;
	std::size_t n = 0;
	while (n < content.length() && !std::isspace(content[n]))
		n++;
	if (n >= content.length())
		return;
	std::string nickname(content, 0, n);

	start = ++n;
	while (n < content.length() && !std::isspace(content[n]))
		n++;
	std::string channel(content, start, n - start);

	if (content[n])
		start = ++n;
	std::string	message(content, start);
	std::list<Chanel>::iterator chanIt = findChannel(channel);
	if (chanIt->isOp(user) == false || chanIt->isInChannel(&*findUser(nickname)) == false 
			|| &*findUser(nickname) == user)
		return ;
	if (chanIt != _channelList.end())
	{
		if (chanIt->isBanned(user->getUsername()))
			return;
		std::vector<User*> users = chanIt->getUsers();
		std::vector<User*>::iterator it = users.begin();
		for (; it != users.end(); it++)
		{
			User* tmp = *it;
			sendTo(tmp->getId(), getMsgPrefix(user) + " KICK " + channel + " " + nickname + " :" + message);
		}
		chanIt->removeUser(&*findUser(nickname));
		return;
	}
}

std::string IrcServer::getMsgPrefix(User* user) const
{
	std::string res(":" + user->getNickname() + "!" + user->getUsername());
	return res;
}


std::list<Chanel>::iterator	IrcServer::findChannel(const std::string &name)
{
	std::list<Chanel>::iterator it = _channelList.begin();
	for(; it != _channelList.end(); it++)
	{
		if (it->getName() == name)
			return it;
	}
	return _channelList.end();
}


std::list<User>::iterator	IrcServer::findUser(const std::size_t id)
{
	std::list<User>::iterator it = _userList.begin();
	for(; it != _userList.end(); it++)
	{
		if (it->getId() == id)
			return it;
	}
	return _userList.end();
}

std::list<User>::iterator	IrcServer::findUser(const std::string& name)
{
	std::list<User>::iterator it = _userList.begin();
	for(; it != _userList.end(); it++)
	{
		if (it->getNickname() == name)
			return it;
	}
	return _userList.end();
}

////////////////////////////////////////////////////////////////////////////////
///                       COMMAND                                            ///
////////////////////////////////////////////////////////////////////////////////

bool	IrcServer::joinChannel(User *user, const std::string &channel)
{
	if (channel[0] != '#' || channel.length() < 2)
	{
		std::cout << channel << " isn't a valid name" << std::endl;
		return false;
	}
	std::list<Chanel>::iterator it = findChannel(channel);
	if (it == _channelList.end())
	{
		_channelList.push_back(Chanel(channel, user));
		return true;
	}
	if (it->addUser(user) == true)
	{
		std::list<User>::iterator it_user = findUser(user->getId());
		it_user->joinChanel(channel);
		return true;
	}
	return false;
}

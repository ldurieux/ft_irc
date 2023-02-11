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

	std::string type_of_request;
	std::string content;
	std::string first_arg;
	std::string second_arg;
	std::string third_arg;
	int i;
	int n;
	for (i = 0; i < (int)data.length(); i++)
	{
		if (data.at(i) < 65 || data.at(i) > 90)
			break ;
	}
	type_of_request.assign(data, 0, i);
	content.append(data);
	content.erase(0, i);
	std::cout << '[' << id << "]received: \'" << data << '\'' << std::endl << std::endl;
	std::cout << " [" << id << "]Type_of_request: " << type_of_request << "\n";
	std::cout << " [" << id << "]Content: " << content << "\n";
	std::string instruction[9] = {"JOIN", "NICK", "PASS", "USER", "QUIT", "PART", "PRIVMSG", "NOTICE", "MODE"};
	for (i = 0; i < 9; i++)
	{
		if (type_of_request == instruction[i])
			break ;
	}
	n = 0;
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
	case 8:
		std::cout << " [" << id << "]MODE DETECTED " << "\n"; //Commande MODE
		while (n < (int)content.length() && isspace(content.at(n)))
		{
			n++;
		}
		content.erase(0, n);
		n = 0;
		while (n < (int)content.length())
		{
			if (isspace(content.at(n)))
				break ;
			n++;
		}
		first_arg.assign(content, 0, n); //first_arg = Channel sur lequel on met un user moderator
		content.erase(0, n);
		n = 0;
		while (n < (int)content.length())
		{
			if (isspace(content.at(n)))
				break ;
			n++;
		}
		second_arg.assign(content, 0, n); //second_arg = Le type d'action MODE effectuer (+/-[lettres])
		content.erase(0, n);
		n = 0;
		while (n < (int)content.length())
		{
			if (isspace(content.at(n)))
				break ;
			n++;
		}
		third_arg.assign(content, 0, n); //Le nom du USER que l'on met operator
		//ICI Mettre le USER selectionner OPERATOR sur le bon CHANNEL
		//-------------DEBUG-----------------------
		std::cout << first_arg << "\n";
		std::cout << second_arg << "\n";
		std::cout << third_arg << "\n";
		//-----------------------------------------
		break ;
	}
}

void IrcServer::onClientDisconnect(std::size_t id)
{
	std::cout << '[' << id << "]disconnected" << std::endl;
}

void IrcServer::onJoinChannel(User* user, const std::string& content)
{
	int start = 0;

	while (start < (int)content.length() && isspace(content.at(start)))
		start++;
	int n = start;
	while (n < (int)content.length())
	{
		if (isspace(content.at(n)))
			break ;
		n++;
	}

	std::string channelName(content, start, n - start);
	if (!joinChannel(user, channelName))
		return;

	sendTo(user->getId(), getMsgPrefix(user) + " JOIN :" + channelName);
}

void IrcServer::onNick(User* from, const std::string& content)
{
	int start = 0;

	while (start < (int)content.length() && isspace(content.at(start)))
	{
		start++;
	}
	int n = start;
	while (n < (int)content.length())
	{
		if (isspace(content.at(n)))
			break ;
		n++;
	}
	std::string nickname(content, start, n - start);
	from->setNickname(nickname);
}

void IrcServer::onPass(User* user, const std::string& content)
{
	int start = 0;

	while (start < (int)content.length() && isspace(content.at(start)))
	{
		start++;
	}
	int n = start;
	while (n < (int)content.length())
	{
		if (isspace(content.at(n)))
			break ;
		n++;
	}
	std::string pass(content, start, n - start);
	if (password() == pass)
		user->setAuthenticated(true);
	else
		std::cout << "Invalid password" << std::endl;
}

void IrcServer::onUser(User* user, const std::string& content)
{
	int start = 0;

	while (start < (int)content.length() && isspace(content.at(start)))
	{
		start++;
	}
	int n = start;
	while (n < (int)content.length())
	{
		if (isspace(content.at(n)))
			break ;
		n++;
	}
	std::string username(content, start, n - start);
	user->setUsername(username);

	sendTo(user->getId(), "PING :ft_irc");
}

void IrcServer::onQuit(User* user, const std::string& content)
{
	int start = 0;

	while (start < (int)content.length() && isspace(content.at(start)))
	{
		start++;
	}
	int n = start;
	while (n < (int)content.length())
	{
		if (isspace(content.at(n)))
			break ;
		n++;
	}
	std::string msg(content, start, n - start);


	(void)user;
	std::cout << "TODO send exit message: " << msg << std::endl;
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
	int start = 0;

	while (start < (int)content.length() && isspace(content.at(start)))
	{
		start++;
	}
	int n = start;
	while (n < (int)content.length())
	{
		if (isspace(content.at(n)))
			break ;
		n++;
	}

	std::string dest(content, start, n - start);
	std::string message(content, n + 2, std::string::npos);

	std::list<Chanel>::iterator chanIt = findChannel(dest);
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
	int start = 0;

	while (start < (int)content.length() && isspace(content.at(start)))
	{
		start++;
	}
	int n = start;
	while (n < (int)content.length())
	{
		if (isspace(content.at(n)))
			break ;
		n++;
	}

	std::string dest(content, start, n - start);
	std::string message(content, n + 2, std::string::npos);

	std::list<Chanel>::iterator chanIt = findChannel(dest);
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

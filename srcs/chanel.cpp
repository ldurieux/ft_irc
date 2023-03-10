#include "../includes/user.h"
#include "../includes/chanel.h"
#include <string>
#include <vector>

Chanel::Chanel(const std::string& name, User *creator) :
	_name(name)
{
	std::cout << "Channel " << name << " created by " << creator->getUsername() << "." << std::endl;
	_userOfChanel[creator] = OP;
}

Chanel::~Chanel() {}

const std::string Chanel::getName()
{
	return _name;
}

bool	Chanel::addUser(User *user)
{
	std::list<std::string>::iterator findIter = std::find(_banList.begin(), _banList.end(), user->getUsername());
	if (findIter != _banList.end())
		return false;
	_userOfChanel[user] = REGULAR;
	return true;
}

void Chanel::removeUser(User* user)
{
	std::map<User*, bool>::iterator it = _userOfChanel.begin();
	for(; it != _userOfChanel.end(); it++)
	{
		if (it->first == user)
		{
			_userOfChanel.erase(it);
			return;
		}
	}
}

bool	Chanel::promoteUser(User *op, User *target)
{
	if (_userOfChanel[op] == OP 
			&& _userOfChanel.find(target) != _userOfChanel.end()
			&& _userOfChanel[target] != OP)
	{
		_userOfChanel[target] = OP;
		return true;
	}
	return false;
}

bool	Chanel::demoteUser(User *op, User *target)
{
	if (_userOfChanel[op] == OP 
			&& _userOfChanel.find(target) != _userOfChanel.end()
			&& _userOfChanel[target] != REGULAR)
	{
		_userOfChanel[target] = REGULAR;
		return true;
	}
	return false;
}

bool Chanel::isBanned(const std::string& username)
{
	std::list<std::string>::iterator it;
	for (it = _banList.begin(); it != _banList.end(); ++it)
	{
		if (*it == username)
			return true;
	}
	return false;
}

bool	Chanel::isOp(User* user)
{
	if (_userOfChanel[user] == OP)
		return true;
	return false;
}

bool	Chanel::isInChannel(User* user)
{
	std::vector<User*> listUser = getUsers();
	std::vector<User*>::iterator it = listUser.begin();
	for(;it != listUser.end(); ++it)
		if (*it == user)
			return true;
	return false;
}

std::vector<User*> Chanel::getUsers() const
{
	std::vector<User*> res;

	std::map<User*, bool>::const_iterator it = _userOfChanel.begin();
	for (; it != _userOfChanel.end(); it++)
		res.push_back(it->first);
	return res;
}

std::size_t Chanel::userCount() const
{
	return _userOfChanel.size();
}


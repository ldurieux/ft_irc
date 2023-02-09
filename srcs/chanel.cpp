#include "../includes/user.h"
#include "../includes/chanel.h"
#include <string>

Chanel::Chanel(std::string name, User *creator) :
	_name(name)
{
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


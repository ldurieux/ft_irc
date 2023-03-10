#include "../includes/user.h"
#include "../includes/chanel.h"
#include <cstddef>
#include <string>

User::User(std::size_t id) : 
	_id(id),
	_nickname(),
	_username(),
	_authenticated(false)
{}

User::~User() {}

std::size_t User::getId()
{
	return _id;
}

void    User::setNickname(std::string nickname)
{
    _nickname = nickname;
}

void    User::setUsername(std::string username)
{
    _username = username;
}

const std::string& User::getUsername()
{
	return _username;
}

void    User::printInfo() const
{
    std::cout << "|Nickname: " << _nickname << " |Username: " << _username << "\n";
}

void	User::joinChanel(std::string nameChanel)
{
	_chanelOfUser.push_back(nameChanel);
}

std::string User::getNickname() const
{
	return _nickname;
}

bool User::authenticated() const
{
	if (_username.size() <= 0)
		return false;
	if (_nickname.size() <= 0)
		return false;
	return _authenticated;
}

void User::setAuthenticated(bool newAuthenticated)
{
	_authenticated = newAuthenticated;
}

#include "../includes/user.h"
#include "../includes/chanel.h"
#include <cstddef>
#include <string>

User::User(std::size_t id) : _id(id) {}

User::~User() {}

void    User::setNickname(std::string nickname)
{
    _nickname = nickname;
}

void    User::setUsername(std::string username)
{
    _username = username;
}

const std::string User::getUsername()
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

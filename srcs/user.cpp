#include "../includes/user.h"

void    User::setNickname(std::string nickname)
{
    _nickname = nickname;
}

void    User::setUsername(std::string username)
{
    _username = username;
}

void    User::printInfo() const
{
    std::cout << "|Nickname: " << _nickname << " |Username: " << _username << "\n";
}

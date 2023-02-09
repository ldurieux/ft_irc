#ifndef USER_H
# define USER_H

#include "baseserver.h"
#include <cstddef>
#include <list>
#include <string>

class Chanel;

class User {
public:
	User(std::size_t id);
	~User();
	const std::size_t			getId();
    void				setNickname(std::string nickname);
	const std::string	getUsername();
    void				setUsername(std::string username);
    void				printInfo() const;
	void				joinChanel(std::string nameChanel);
private:
	std::size_t			_id;
    std::string			_nickname;
    std::string			_username;
	std::list<std::string>	_chanelOfUser;

};

#endif

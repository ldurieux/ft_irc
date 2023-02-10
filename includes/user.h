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
	std::size_t			getId();
    void				setNickname(std::string nickname);
	const std::string&	getUsername();
    void				setUsername(std::string username);
    void				printInfo() const;
	void				joinChanel(std::string nameChanel);
	std::string getNickname() const;

	bool authenticated() const;
	void setAuthenticated(bool newAuthenticated);

private:
	std::size_t			_id;
    std::string			_nickname;
    std::string			_username;
	bool				_authenticated;
	std::list<std::string>	_chanelOfUser;

};

#endif

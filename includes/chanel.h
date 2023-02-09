#ifndef CHANEL_H
# define CHANEL_H

#include "user.h"
#include <map>
#include <string>
#include <algorithm>

# define OP true
# define REGULAR false

class User;

class Chanel {
public:
	Chanel(std::string name, User *creator);
	~Chanel();
	const std::string getName ();
	bool	addUser(User *user);
	bool	promoteUser(User *op, User *target);
	bool	demoteUser(User *op, User *target);
private:
	std::string				_name;
	std::list<std::string>		_banList;
	std::map<User*, bool>	_userOfChanel;
};

#endif // !CHANEL_H

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
	Chanel(const std::string& name, User *creator);
	~Chanel();

	const std::string getName ();
	bool	addUser(User *user);
	void	removeUser(User *user);
	bool	promoteUser(User *op, User *target);
	bool	demoteUser(User *op, User *target);

	bool	isBanned(const std::string& username);
	bool	isOp(User* user);
	bool	isInChannel(User* user);

	std::vector<User*> getUsers() const;
	std::size_t userCount() const;

private:
	std::string				_name;
	std::list<std::string>	_banList;
	std::map<User*, bool>	_userOfChanel;
};

#endif // !CHANEL_H

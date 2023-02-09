#ifndef IRCSERVER_H
#define IRCSERVER_H

#include "baseserver.h"
#include "chanel.h"
#include "user.h"
#include "chanel.h"
#include <cstddef>
#include <list>

class IrcServer : public BaseServer
{
public:
	IrcServer(const std::string& password) : BaseServer(password) {}

	std::list<Chanel>::iterator	findChannel(const std::string &name);
	std::list<User>::iterator	findUser(const std::size_t id);

////////////////////////////////////////////////////////////////////////////////
///                       COMMAND                                            ///
////////////////////////////////////////////////////////////////////////////////
	bool	joinChannel(User *user, const std::string &channel);

protected:
	virtual void onNewConnection(std::size_t id);
	virtual void onNewData(std::size_t id, const std::string& data);
	virtual void onClientDisconnect(std::size_t id);

private:
	std::list<User> _userList;
	std::list<Chanel> _channelList;
};

#endif // IRCSERVER_H

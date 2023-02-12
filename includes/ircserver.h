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
	std::list<User>::iterator	findUser(const std::string& name);

////////////////////////////////////////////////////////////////////////////////
///                       COMMAND                                            ///
////////////////////////////////////////////////////////////////////////////////
	bool	joinChannel(User *user, const std::string &channel);

protected:
	virtual void onNewConnection(std::size_t id);
	virtual void onNewData(std::size_t id, const std::string& data);
	virtual void onClientDisconnect(std::size_t id);

private:
	void onJoinChannel(User* user, const std::string& content);
	void onNick(User* user, const std::string& content);
	void onPass(User* user, const std::string& content);
	void onUser(User* user, const std::string& content);
	void onQuit(User* user, const std::string& content);
	void onPart(User* user, const std::string& content);
	void onPrivmsg(User* user, const std::string& content);
	void onNotice(User* user, const std::string& content);
	void onMode(User* user, const std::string& content);
	void onKick(User* user, const std::string& content);

	void onDisconnect(std::size_t id);

	std::string getMsgPrefix(User* user) const;

	std::list<User> _userList;
	std::list<Chanel> _channelList;
};

#endif // IRCSERVER_H

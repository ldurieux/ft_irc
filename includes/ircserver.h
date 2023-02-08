#ifndef IRCSERVER_H
#define IRCSERVER_H

#include "baseserver.h"

class IrcServer : public BaseServer
{
public:
	IrcServer(const std::string& password) : BaseServer(password) {}

protected:
	virtual void onNewConnection(std::size_t id);
	virtual void onNewData(std::size_t id, const std::string& data);
	virtual void onClientDisconnect(std::size_t id);

private:
};

#endif // IRCSERVER_H

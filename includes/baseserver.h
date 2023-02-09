#ifndef BASESERVER_H
#define BASESERVER_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>

#include <vector>
#include <poll.h>

class BaseServer
{
public:
	BaseServer(const std::string& password);

	bool listen_(int port);
	void run();

	const std::string& password() const { return _password; }

private:
	void incomingConnection();
	void incomingData(std::size_t i);

	std::size_t getPosFromId(std::size_t id);
	void sendData(int fd, const std::string& data);

protected:
	virtual void onNewConnection(std::size_t id) = 0;
	virtual void onNewData(std::size_t id, const std::string& data) = 0;
	virtual void onClientDisconnect(std::size_t id) = 0;

	void disconnectClient(std::size_t id);

	void sendTo(std::size_t id, const std::string& data);
	void broadcast(const std::string& data);
	void broadcastFrom(std::size_t id, const std::string& data);

private:
	std::string _password;

	sockaddr_in _socketServer;
	int			_fdServer;

	std::vector<pollfd>			_pollFds;
	std::vector<std::string>	_clientBuf;
};

#endif // BASESERVER_H

#ifndef BASECLIENT_H
#define BASECLIENT_H

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

#include <poll.h>

class BaseClient
{
public:
	BaseClient(const std::string& username, const std::string& nickname);

	bool connectToHost(const std::string& hostStr, int port);

	void setPassword(const std::string& password) { _password = password; }
	bool connected() const { return _connected; }

	void run();

private:
	bool incomingData();

protected:
	void sendData(const std::string& data);

	virtual void onNewData(const std::string& data) = 0;

private:
	bool		_connected;
	std::string _password;
	std::string _username;
	std::string _nickname;

	int			_fd;
	pollfd		_pollFd;
	std::string	_buf;
};

#endif // BASECLIENT_H

#ifndef SERVER_H
#define SERVER_H

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

class Server
{
public:
	Server(const std::string& password);

	bool listen_(int port);
	void run();

private:
	std::string _password;

	sockaddr_in _socketServer;
	int			_fdServer;
	std::vector<pollfd> _pollFds;
};

#endif // SERVER_H

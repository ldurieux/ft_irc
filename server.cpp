#include "server.h"

Server::Server(const std::string& password) :
	_password(password)
{
	memset((char*)&_socketServer, 0, sizeof(sockaddr_in));
}

bool Server::listen_(int port)
{
	_socketServer.sin_family = AF_INET;
	_socketServer.sin_addr.s_addr = htonl(INADDR_ANY);
	_socketServer.sin_port = htons(port);

	_fdServer = socket(AF_INET, SOCK_STREAM, 0);
	if (_fdServer < 0)
		return false;
	if (bind(_fdServer, (struct sockaddr*)&_socketServer, sizeof(_socketServer)) < 0)
		return false;

	pollfd tmp;
	tmp.fd = _fdServer;
	tmp.revents = 0;
	tmp.events = POLLIN;
	_pollFds.push_back(tmp);
	return listen(_fdServer, 3) == 0;
}

void Server::run()
{
	while (1)
	{
		std::size_t i;
		for(i = 0; i < _pollFds.size(); i++)
			if (poll(_pollFds.data() + i, 1, 5) != 0)
				break;
		if (i == _pollFds.size()) //no events
			continue;
		else if (i == 0) //new connection
		{
			sockaddr_in socketClient;
			socklen_t size = sizeof(sockaddr_in);
			int fdClient = accept(_fdServer, (sockaddr *)&socketClient, &size);
			if (fdClient < 0)
			{
				std::cout << "Couldn't open fd for new client" << std::endl;
				continue;
			}
			//set the fd as non blocking
			if (fcntl(fdClient, F_SETFL, fcntl(fdClient, F_GETFL) | O_NONBLOCK) == -1)
			{
				std::cout << "Couldn't set client fd as non-blocking" << std::endl;
				return ;
			}

			pollfd tmp;
			tmp.fd = fdClient;
			tmp.revents = 0;
			tmp.events = POLLIN;
			_pollFds.push_back(tmp);

			onNewConnection(tmp.fd);
		}
		else //client sent packet or disconnected
		{
			std::string data;
			char buf[1024];

			ssize_t bytes_received = recv(_pollFds[i].fd, buf, 1024, 0);

			//if there was not data on the first recv then it's a disconnected
			//otherwise it's a message
			if (bytes_received == 0)
			{
				onClientDisconnect(_pollFds[i].fd);
				_pollFds.erase(_pollFds.begin() + i);
				continue ;
			}
			else if (bytes_received == -1)
			{
				std::cout << "Couldn't read on fd " << _pollFds[i].fd << ", this shouldn't happen" << std::endl;
				continue;
			}
			do {
				data.append(buf, bytes_received);
			} while ((bytes_received = recv(_pollFds[i].fd, buf, 1024, 0)) != -1);

			onNewData(_pollFds[i].fd, data);
		}
	}
}

void Server::onNewConnection(std::size_t id)
{
	std::cout << '[' << id << "]new client !" << std::endl;
}

void Server::onNewData(std::size_t id, const std::string& data)
{
	std::cout << '[' << id << "]received: \'" << data << '\'' << std::endl;
}

void Server::onClientDisconnect(std::size_t id)
{
	std::cout << '[' << id << "]disconnected" << std::endl;
}

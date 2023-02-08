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
	tmp.events = 0;
	tmp.events = POLLIN;
	_pollFds.push_back(tmp);
	return listen(_fdServer, 3) == 0;
}

void Server::run()
{
	while (1)
	{
		if (poll(_pollFds.data(), _pollFds.size(), 5000) == 0)
			continue ;

		std::size_t i;
		for(i = 0; i < _pollFds.size(); i++)
			if (poll(_pollFds.data() + i, 1, 1) != 0)
				break;
		if (i == _pollFds.size())
		{
			std::cout << "shouldn't happen" << std::endl;
			continue;
		}
		if (i == 0) //new connection
		{
			sockaddr_in socketClient;
			socklen_t size = sizeof(sockaddr_in);
			int fdClient = accept(_fdServer, (sockaddr *)&socketClient, &size);
			if (fdClient < 0)
			{
				std::cout << "couldn't open fd for new client" << std::endl;
				continue;
			}

			pollfd tmp;
			tmp.fd = fdClient;
			tmp.events = 0;
			tmp.events = POLLIN;
			_pollFds.push_back(tmp);

			std::cout << '[' << tmp.fd << "]new client !" << std::endl;
		}
		else //client sent packet or disconnected
		{
			std::string data;
			char buf[1024];
			ssize_t bytes_received;
			while((bytes_received = recv(_pollFds[i].fd, buf, 1024, 0)))
			{
				if (bytes_received == -1)
					break;
				data.append(buf, bytes_received);
				if (bytes_received < 1024)
					break;
			}
			if (bytes_received == 0)
				onclientDisconnect(i);
			else
				std::cout << '[' << _pollFds[i].fd << "]received: \'" << data << '\'' << std::endl;
		}
	}
}

void Server::onclientDisconnect(std::size_t id)
{
	std::cout << '[' << _pollFds[id].fd << "]disconnected" << std::endl;
	_pollFds.erase(_pollFds.begin() + id);
}

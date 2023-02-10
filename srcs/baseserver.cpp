#include "baseserver.h"

BaseServer::BaseServer(const std::string& password) :
	_password(password)
{
	memset((char*)&_socketServer, 0, sizeof(sockaddr_in));
}

bool BaseServer::listen_(int port)
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

void BaseServer::run()
{
	std::size_t i;

	while (1)
	{
		//check if any socket has pending data
		for(i = 0; i < _pollFds.size(); i++)
			if (poll(_pollFds.data() + i, 1, 5) != 0)
				break;
		if (i == _pollFds.size()) //no events
			continue;
		else if (i == 0) //new connection
			incomingConnection();
		else //client sent packet or disconnected
			incomingData(i);
	}
}

void BaseServer::incomingConnection()
{
	sockaddr_in socketClient;
	socklen_t size = sizeof(sockaddr_in);
	int fdClient = accept(_fdServer, (sockaddr *)&socketClient, &size);
	if (fdClient < 0)
	{
		std::cout << "Couldn't open fd for new client" << std::endl;
		return;
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
	_clientBuf.push_back(std::string());

	onNewConnection(tmp.fd);
}

void BaseServer::incomingData(std::size_t i)
{
	std::string &data = _clientBuf[i - 1];
	char buf[1024];

	ssize_t bytes_received = recv(_pollFds[i].fd, buf, 1024, 0);

	if (bytes_received == -1)
	{
		std::cout << "Couldn't read on fd " << _pollFds[i].fd << ", this shouldn't happen" << std::endl;
		return;
	}
	//if there was not data on the first recv then it's a disconnect
	else if (bytes_received == 0)
	{
		onClientDisconnect(_pollFds[i].fd);
		close(_pollFds[i].fd);
		_pollFds.erase(_pollFds.begin() + i);
		return;
	}
	//otherwise it's a message, so read everything available
	do {
		data.append(buf, bytes_received);
	} while ((bytes_received = recv(_pollFds[i].fd, buf, 1024, 0)) != -1);

	std::string::iterator start = data.begin();
	for (std::string::iterator it = data.begin(); it != data.end(); ++it)
	{
		if (*it == '\n')
		{
			onNewData(_pollFds[i].fd, std::string(start, it));
			start = it + 1;
		}
	}

	//keep only data after the last '\n' on the client buffer
	data.erase(data.begin(), start);
}

std::size_t BaseServer::getPosFromId(std::size_t id)
{
	std::size_t res;
	for (res = 1; res < _pollFds.size(); res++)
		if ((std::size_t)_pollFds[res].fd == id)
			return res;
	throw std::invalid_argument("Invalid id");
}

void BaseServer::sendData(int fd, const std::string& data)
{
	std::string cpy = data + "\n";

	if (send(fd, cpy.data(), cpy.size(), 0) == -1)
		std::cout << __FUNCTION__ << " [" << fd << "] " << errno << std::endl;
}

void BaseServer::disconnectClient(std::size_t id)
{
	std::size_t i;
	try{ i = getPosFromId(id); }
	catch (std::exception& e) {
		std::cout << __FUNCTION__ << ' ' << e.what() << " [" << id << ']' << std::endl;
		return;
	}
	_pollFds.erase(_pollFds.begin() + i);
	_clientBuf.erase(_clientBuf.begin() + i - 1);
	std::cout << '[' << id << "]closed" << std::endl;
}

void BaseServer::sendTo(std::size_t id, const std::string& data)
{
	try{ getPosFromId(id); }
	catch (std::exception& e) {
		std::cout << __FUNCTION__ << ' ' << e.what() << " [" << id << ']' << std::endl;
		return;
	}

	sendData((int)id, data);
}

void BaseServer::broadcast(const std::string& data)
{
	for (std::size_t i = 1; i < _pollFds.size(); i++)
		sendData(_pollFds[i].fd, data);
}

void BaseServer::broadcastFrom(std::size_t id, const std::string& data)
{
	std::size_t pos;
	try{ pos = getPosFromId(id); }
	catch (std::exception& e) {
		std::cout << __FUNCTION__ << ' ' << e.what() << " [" << id << ']' << std::endl;
		return;
	}

	for (std::size_t i = 1; i < _pollFds.size(); i++)
		if (i != pos)
			sendData(_pollFds[i].fd, data);
}

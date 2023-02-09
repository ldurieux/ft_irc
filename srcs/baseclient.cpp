#include "baseclient.h"

BaseClient::BaseClient(const std::string& username, const std::string& nickname) :
	_username(username),
	_nickname(nickname)
{

}

bool BaseClient::connectToHost(const std::string& hostStr, int port)
{
	hostent* host = gethostbyname(hostStr.c_str());
	sockaddr_in sendSockAddr;
	memset((char*)&sendSockAddr, 0, sizeof(sendSockAddr));
	sendSockAddr.sin_family = AF_INET;
	sendSockAddr.sin_addr.s_addr =
		inet_addr(inet_ntoa(*(in_addr*)*host->h_addr_list));
	sendSockAddr.sin_port = htons(port);

	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd < 0)
	{
		std::cout << "Couldn't create socket file descriptor" << std::endl;
		return false;
	}
	if (connect(_fd, (sockaddr*) &sendSockAddr, sizeof(sendSockAddr)) < 0)
	{
		std::cout << "Couldn't connect to server" << std::endl;
		return false;
	}

	_pollFd.fd = _fd;
	_pollFd.revents = 0;
	_pollFd.events = POLLIN;

	return fcntl(_fd, F_SETFL, fcntl(_fd, F_GETFL) | O_NONBLOCK) >= 0;
}

void BaseClient::run()
{
	if (_password.size() > 0)
		sendData("PASS " + _password);
	sendData("NICK " + _nickname);
	sendData("USER " + _username + " 0 * :" + _username);

	while (1)
	{
		if (poll(&_pollFd, 1, 5000) == 0)
			continue;
		if (!incomingData())
		{
			std::cout << "Disconnected" << std::endl;
			return ;
		}
	}
}

bool BaseClient::incomingData()
{
	char buf[1024];

	ssize_t bytes_received = recv(_fd, buf, 1024, 0);

	if (bytes_received == -1)
	{
		std::cout << "Couldn't read on fd " << _fd << ", this shouldn't happen" << std::endl;
		return false;
	}
	//if there was not data on the first recv then it's a disconnect
	else if (bytes_received == 0)
	{
		close(_fd);
		return false;
	}
	//otherwise it's a message, so read everything available
	do {
		_buf.append(buf, bytes_received);
	} while ((bytes_received = recv(_fd, buf, 1024, 0)) != -1);

	std::string::iterator start = _buf.begin();
	for (std::string::iterator it = _buf.begin(); it != _buf.end(); ++it)
	{
		if (*it == '\n')
		{
			onNewData(std::string(start, it));
			start = it + 1;
		}
	}

	//keep only data after the last '\n' on the client buffer
	_buf.erase(_buf.begin(), start);

	return true;
}

void BaseClient::sendData(const std::string& data)
{
	std::cout << "<< '" << data << '\'' << std::endl;
	if (send(_fd, data.data(), data.size(), 0) == -1)
		std::cout << __FUNCTION__ << errno << std::endl;
	if (send(_fd, "\n", 1, 0) == -1)
		std::cout << __FUNCTION__ << errno << std::endl;
}

#include <string>
#include <stdexcept>
#include <iostream>
#include <limits>
#include "ircclient.h"

void parseArgs(std::string& host, int& port, std::string& password, int argc, char **argv)
{
	if (argc < 4)
		throw std::invalid_argument("Not enough arguments");
	if (argc > 4)
		throw std::invalid_argument("Too many arguments");

	host = argv[1];
	if (host.empty())
		throw std::invalid_argument("Empty host");
	password = argv[3];
	if (password.empty())
		throw std::invalid_argument("Empty password");

	const std::string portStr(argv[2]);
	port = 0;
	for (std::string::const_iterator it = portStr.begin(); it != portStr.end(); ++it)
	{
		if (!std::isdigit(*it))
			throw std::invalid_argument("Non numeric character in port");
		port = port * 10 + *it - '0';
		if (port > 65535)
			throw std::invalid_argument("Port too big");
	}
	if (port <= 0)
		throw std::invalid_argument("Port too small");
}

void usage()
{
	std::cout << "Usage: ./bot <host> <port> <password>" << std::endl;
}

int main(int argc, char **argv)
{
	std::string host;
	int port;
	std::string password;

	try {
		parseArgs(host, port, password, argc, argv);
	}
	catch (const std::exception &e) {
		std::cout << e.what() << std::endl;
		usage();
		return 1;
	}

	IrcClient client("ft_ircbot", "ft_ircbot");
	client.setPassword(password);
	if (!client.connectToHost(host, port))
	{
		std::cout << "Couldn't connect to host " << host << ":" << port << std::endl;
		return 1;
	}
	std::cout << "Connected to " << host << ':' << port << std::endl;

	client.run();

	return 0;
}

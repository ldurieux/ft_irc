#include <string>
#include <stdexcept>
#include <iostream>
#include <limits>
#include "ircserver.h"

void parseArgs(int& port, std::string& password, int argc, char **argv)
{
	if (argc < 3)
		throw std::invalid_argument("Not enough arguments");
	if (argc > 3)
		throw std::invalid_argument("Too many arguments");

	password = argv[2];
	if (password.empty())
		throw std::invalid_argument("Empty password");

	const std::string portStr(argv[1]);
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
	std::cout << "Usage: ./ircserver <port> <password>" << std::endl;
}

int main(int argc, char **argv)
{
	int port;
	std::string password;

	try {
		parseArgs(port, password, argc, argv);
	}
	catch (const std::exception &e) {
		std::cout << e.what() << std::endl;
		usage();
		return 1;
	}

	IrcServer server(password);
	if (!server.listen_(port))
	{
		std::cout << "Couldn't listen on port " << port << std::endl;
		return 1;
	}

	std::cout.setf(std::ios::unitbuf);
	server.run();

	return 0;
}

#include "../includes/ircserver.h"
#include "../includes/user.h"

void IrcServer::onNewConnection(std::size_t id)
{
	std::cout << '[' << id << "]new client !" << std::endl;
}

void IrcServer::onNewData(std::size_t id, const std::string& data)
{
	//std::cout << '[' << id << "]received: \'" << data << '\'' << std::endl;
	//Prendre les 4 premieres lettre de DATA
	User *user;
	user = new User();
	std::string type_of_request;
	std::string content;
	int i;
	for (i = 0; i < (int)data.length(); i++)
	{
		if (data.at(i) < 65 || data.at(i) > 90)
			break ;
	}
	type_of_request.assign(data, 0, i);
	content.append(data);
	content.erase(0, i);
	std::cout << '[' << id << "]received: \'" << data << '\'' << std::endl << std::endl;
	std::cout << " [" << id << "]Type_of_request: " << type_of_request << "\n";
	std::cout << " [" << id << "]Content: " << content << "\n";
	std::string instruction[4] = {"JOIN", "NICK", "PRIVMSG", "USER"};
	for (i = 0; i < 4; i++)
	{
		if (type_of_request == instruction[i])
			break ;
	}
	/* switch (i)
	{
		case 0:
			std::cout << " [" << id << "]JOIN DETECTED " << "\n";
			break ;
		case 1:
			std::cout << " [" << id << "]NICK DETECTED " << "\n";
			int n;
			while (n < (int)content.length() && isspace(content.at(n)))
			{
				n++;
			}
			content.erase(0, n);
			n = 0;
			while (n < (int)data.length())
			{
				if (isspace(content.at(n)))
					break ;
				n++;
			}
			content.assign(content, 0, n);
			user->setNickname (content);
			break ;
		case 2:
			std::cout << " [" << id << "]PRIVATE MSG DETECTED " << "\n";
			break ;
		case 3:
			std::cout << " [" << id << "]USER DETECTED " << "\n";
			int j;
			while (j < (int)content.length() && isspace(content.at(j)))
			{
				j++;
			}
			content.erase(0, j);
			j = 0;
			while (j < (int)data.length())
			{
				if (isspace(content.at(j)))
					break ;
				j++;
			}
			content.assign(content, 0, j);
			user->setUsername(content);
			break ;
		default:
			std::cout << " [" << id << "]UNKNOWN DETECTED " << "\n";
	} */
	user->printInfo();
	std::cout << "\n";
}

void IrcServer::onClientDisconnect(std::size_t id)
{
	std::cout << '[' << id << "]disconnected" << std::endl;
}

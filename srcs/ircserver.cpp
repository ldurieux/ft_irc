#include "../includes/ircserver.h"
#include "../includes/user.h"
#include <list>
#include <cstddef>
#include <string>

void IrcServer::onNewConnection(std::size_t id)
{
	_userList.push_back(User(id));
	std::cout << '[' << id << "]new client !" << std::endl;
}

void IrcServer::onNewData(std::size_t id, const std::string& data)
{
	//std::cout << '[' << id << "]received: \'" << data << '\'' << std::endl;
	//Prendre les 4 premieres lettre de DATA
	User user(id);
	std::string type_of_request;
	std::string content;
	std::string first_arg;
	std::string second_arg;
	std::string third_arg;
	int i;
	int n; // Pour le parsing sur le switch
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
	std::string instruction[9] = {"JOIN", "NICK", "PASS", "USER", "QUIT", "PART", "PRIVMSG", "NOTICE", "MODE"};
	for (i = 0; i < 9; i++)
	{
		if (type_of_request == instruction[i])
			break ;
	}
	n = 0;
	switch (i)
	{
		case 0: //Commande JOIN
			std::cout << " [" << id << "]JOIN DETECTED " << "\n";
			while (n < (int)content.length() && isspace(content.at(n)))
			{
				n++;
			}
			content.erase(0, n);
			n = 0;
			while (n < (int)content.length())
			{
				if (isspace(content.at(n)))
					break ;
				n++;
			}
			first_arg.assign(content, 0, n); //first_arg = Channel
			//joinChannel(|USER|, first_arg);
			//-------------DEBUG-----------------------
			std::cout << first_arg << "\n";
			//-----------------------------------------
			break ;
		case 1:
			std::cout << " [" << id << "]NICK DETECTED " << "\n"; //Commande NICK
			while (n < (int)content.length() && isspace(content.at(n)))
			{
				n++;
			}
			content.erase(0, n);
			n = 0;
			while (n < (int)content.length())
			{
				if (isspace(content.at(n)))
					break ;
				n++;
			}
			first_arg.assign(content, 0, n); //first_arg = Le nickname
			//ICI il faut changer le nickname du user dans la class
			//-------------DEBUG-----------------------
			std::cout << first_arg << "\n";
			//-----------------------------------------
			break ;
		case 2:
			std::cout << " [" << id << "]PASS DETECTED " << "\n"; //Commande PASS
			while (n < (int)content.length() && isspace(content.at(n)))
			{
				n++;
			}
			content.erase(0, n);
			n = 0;
			while (n < (int)content.length())
			{
				if (isspace(content.at(n)))
					break ;
				n++;
			}
			first_arg.assign(content, 0, n); //first_arg = Mot_de_Passe
			//-------------DEBUG-----------------------
			std::cout << first_arg << "\n";
			//-----------------------------------------
			//ICI il faut changer le Motdepasse du user dans la class
			break ;
		case 3:
			std::cout << " [" << id << "]USER DETECTED " << "\n"; //Commande USER
			while (n < (int)content.length() && isspace(content.at(n)))
			{
				n++;
			}
			content.erase(0, n);
			n = 0;
			while (n < (int)content.length())
			{
				if (isspace(content.at(n)))
					break ;
				n++;
			}
			first_arg.assign(content, 0, n); //first_arg = Username
			//ICI il faut definir le username dans la class
			//-------------DEBUG-----------------------
			std::cout << first_arg << "\n";
			//-----------------------------------------
			break ;
		case 4:
			std::cout << " [" << id << "]QUIT DETECTED " << "\n"; //Commande QUIT
			while (n < (int)content.length() && isspace(content.at(n)))
			{
				n++;
			}
			content.erase(0, n);
			n = 0;
			while (n < (int)content.length())
				n++;
			first_arg.assign(content, 0, n); //first_arg = MSG de depart
			//ICI il faut faire quitter le USER tout en affichant son message de depart
			//-------------DEBUG-----------------------
			std::cout << first_arg << "\n";
			//-----------------------------------------
			break ;
		case 5:
		{
			int nbr_channel = 0;
			std::cout << " [" << id << "]PART DETECTED " << "\n"; //Commande PART (leave)
			while (n < (int)content.length() && isspace(content.at(n)))
			{
				n++;
			}
			content.erase(0, n);
			n = 0;
			while (n < (int)content.length())
			{
				if (content.at(n) == ',' && content.at(n - 1) != ',')
					nbr_channel++;
				if (isspace(content.at(n)))
					break ;
				n++;
			}
			first_arg.assign(content, 0, n); //first_arg = Nom des channels quitters
			std::string channels[nbr_channel];
			n = 0;
			int ch = 0;
			while (n < (int)content.length())
			{
				if (content.at(n) == ',' && content.at(n - 1) != ',')
				{
					channels[ch].assign(content, 0, n);
					ch++;
					content.erase(0, n);
					n = -1;
				}
				if (isspace(content.at(n)))
					break ;
				n++;
			}
			//ICI il faut enlever les channel au USER
			//-------------DEBUG-----------------------
			std::cout << first_arg << "\n";
			int nbr = 0;
			while (nbr < nbr_channel)
			{
				std::cout << channels[nbr] << "\n";
			}
			//-----------------------------------------
			break ;
		}
		case 6:
			std::cout << " [" << id << "]PRIVMSG DETECTED " << "\n"; //COMMANDE PRIVMSG
			while (n < (int)content.length() && isspace(content.at(n)))
			{
				n++;
			}
			content.erase(0, n);
			n = 0;
			while (n < (int)content.length())
			{
				if (isspace(content.at(n)))
					break ;
				n++;
			}
			n += 2;
			first_arg.assign(content, 0, n); //Le USER
			second_arg.assign(content, n, std::string::npos); //Le MSG
			//ICI il faut envoyer le msg au user selectionner
			//-------------DEBUG-----------------------
			std::cout << first_arg << "\n";
			std::cout << second_arg << "\n";
			//-----------------------------------------
			break ;
		case 7:
			std::cout << " [" << id << "]NOTICE DETECTED " << "\n"; //Commande NOTICE
			while (n < (int)content.length() && isspace(content.at(n)))
			{
				n++;
			}
			content.erase(0, n);
			n = 0;
			while (n < (int)content.length())
			{
				if (isspace(content.at(n)))
					break ;
				n++;
			}
			first_arg.assign(content, 0, n); //first_arg = Channel ou USER a qui on envoye la notice
			content.erase(0, n);
			n = 0;
			while (n < (int)content.length())
				n++;
			second_arg.assign(content, 0, n); //second_arg = Le message a notifier (Mettre en couleur)
			//ICI envoyer le msg en couleur au channel ou user selectionner
			//-------------DEBUG-----------------------
			std::cout << first_arg << "\n";
			std::cout << second_arg << "\n";
			//-----------------------------------------
			break ;
		case 8:
			std::cout << " [" << id << "]MODE DETECTED " << "\n"; //Commande MODE
			while (n < (int)content.length() && isspace(content.at(n)))
			{
				n++;
			}
			content.erase(0, n);
			n = 0;
			while (n < (int)content.length())
			{
				if (isspace(content.at(n)))
					break ;
				n++;
			}
			first_arg.assign(content, 0, n); //first_arg = Channel sur lequel on met un user moderator
			content.erase(0, n);
			n = 0;
			while (n < (int)content.length())
			{
				if (isspace(content.at(n)))
					break ;
				n++;
			}
			second_arg.assign(content, 0, n); //second_arg = Le type d'action MODE effectuer (+/-[lettres])
			content.erase(0, n);
			n = 0;
			while (n < (int)content.length())
			{
				if (isspace(content.at(n)))
					break ;
				n++;
			}
			third_arg.assign(content, 0, n); //Le nom du USER que l'on met operator
			//ICI Mettre le USER selectionner OPERATOR sur le bon CHANNEL
			//-------------DEBUG-----------------------
			std::cout << first_arg << "\n";
			std::cout << second_arg << "\n";
			std::cout << third_arg << "\n";
			//-----------------------------------------
			break ;
		default:
	}
	user.printInfo();
	std::cout << "\n";
}

void IrcServer::onClientDisconnect(std::size_t id)
{
	std::cout << '[' << id << "]disconnected" << std::endl;
}


std::list<Chanel>::iterator	IrcServer::findChannel(const std::string &name)
{
	std::list<Chanel>::iterator it = _channelList.begin();
	for(; it != _channelList.end(); it++)
	{
		if (it->getName() == name)
			return it;
	}
	return _channelList.end();
}


std::list<User>::iterator	IrcServer::findUser(const std::size_t id)
{
	std::list<User>::iterator it = _userList.begin();
	for(; it != _userList.end(); it++)
	{
		if (it->getId() == id)
			return it;
	}
	return _userList.end();
}

////////////////////////////////////////////////////////////////////////////////
///                       COMMAND                                            ///
////////////////////////////////////////////////////////////////////////////////

bool	IrcServer::joinChannel(User *user, const std::string &channel)
{
	std::list<Chanel>::iterator it = findChannel(channel);
	if (it == _channelList.end())
	{
		_channelList.push_back(Chanel(channel, user));
		return true;
	}
	if (it->addUser(user) == true)
	{
		std::list<User>::iterator it_user = findUser(user->getId());
		it_user->joinChanel(channel);
		return true;
	}
	return false;
}

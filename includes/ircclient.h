#ifndef IRCCLIENT_H
#define IRCCLIENT_H

#include "baseclient.h"
#include <vector>

class IrcClient : public BaseClient
{
public:
	IrcClient(const std::string& username, const std::string& nickname);

protected:
	void onNewData(const std::string& data);

private:
	int getCmd(const std::string& data);

	void onPing(const std::string& data);
	void onPrivMsg(const std::string& data);

	std::vector<std::string> _cmds;
};

#endif // IRCCLIENT_H

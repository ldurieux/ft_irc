#ifndef IRCCLIENT_H
#define IRCCLIENT_H

#include "baseclient.h"

class IrcClient : public BaseClient
{
public:
	IrcClient(const std::string& username, const std::string& nickname);

protected:
	void onNewData(const std::string& data);

private:

};

#endif // IRCCLIENT_H

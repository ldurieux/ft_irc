#ifndef USER_H
# define USER_H

#include "baseserver.h"

class User {
public:
    void    setNickname(std::string nickname);
    void    setUsername(std::string username);
    void    printInfo() const;
protected:
private:
    std::string _nickname;
    std::string _username;

};

#endif
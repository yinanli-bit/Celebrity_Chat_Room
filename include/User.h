#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

class User {
public:
    User(const std::string &username, const std::string &password);
    std::string getUsername() const;
    bool verifyPassword(const std::string &password) const;

    // 静态方法
    static bool registerUser(const std::string &username, const std::string &password);
    static bool loginUser(const std::string &username, const std::string &password);

private:
    std::string username;
    std::string password;
};

#endif // USER_H

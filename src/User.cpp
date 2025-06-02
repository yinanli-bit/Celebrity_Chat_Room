#include "User.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

User::User(const std::string &username, const std::string &password)
    : username(username), password(password) {}

std::string User::getUsername() const {
    return username;
}

bool User::verifyPassword(const std::string &password) const {
    return this->password == password;
}

bool User::registerUser(const std::string &username, const std::string &password) {
    // 🔍 检查路径是否存在，如果不存在就创建
    if (!std::filesystem::exists("data")) {
        std::filesystem::create_directory("data");
    }

    std::ofstream file("data/users.txt", std::ios::app);
    if (!file.is_open()) {
        std::cerr << "⚠️  无法打开 data/users.txt 文件，请检查路径！" << std::endl;
        return false;
    }
    // ⚠️ 避免重复注册，先检查
    if (loginUser(username, password)) {
        std::cerr << "⚠️  用户已存在：" << username << std::endl;
        return false;
    }

    file << username << "," << password << "\n";
    file.close();
    std::cout << "user register: " << username << std::endl;
    return true;
}

bool User::loginUser(const std::string &username, const std::string &password) {
    std::ifstream file("data/users.txt");
    if (!file.is_open()) {
        std::cerr << "can't read data/users.txt !" << std::endl;
        return false;
    }

    std::string line, user, pass;
    while (getline(file, line)) {
        std::stringstream ss(line);
        getline(ss, user, ',');
        getline(ss, pass, ',');
        if (user == username && pass == password) {
            file.close();
            std::cout << "user login: " << username << std::endl;
            return true;
        }
    }
    file.close();
    std::cout << "wrong id or password: " << username << std::endl;
    return false;
}

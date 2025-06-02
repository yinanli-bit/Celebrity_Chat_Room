#include "httplib.h"
#include "User.h"
#include "ChatHandler.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace httplib;
using json = nlohmann::json;
namespace fs = std::filesystem;

// 全局变量来保存当前登录的用户名
std::string current_user;

// 配置文件路径
const std::string STATIC_DIR = "web";          // 源码中的静态文件目录
const std::string BUILD_STATIC_DIR = "web";    // 构建目录中的静态文件目录

std::string read_file(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return "";
    }
    std::string content((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    return content;
}

int main() {
    Server svr;

    // ======= 静态文件处理 =======
    // 设置静态文件目录（确保CMake已复制文件到构建目录）
    if (!svr.set_base_dir(BUILD_STATIC_DIR.c_str())) {
        std::cerr << "Failed to set base directory: " << BUILD_STATIC_DIR << std::endl;
        return 1;
    }

    // ======= 路由定义 =======

    // 首页（改用文件读取）
    svr.Get("/", [](const Request& req, Response& res) {
        std::string content = read_file(BUILD_STATIC_DIR + "/Welcome.html");
        if (!content.empty()) {
            res.set_content(content, "text/html");
        } else {
            res.status = 500;
            res.set_content("Internal Server Error", "text/plain");
        }
    });

    // 用户注册
    svr.Post("/register", [](const Request& req, Response& res) {
        try {
            auto jsonReq = json::parse(req.body);
            std::string username = jsonReq["username"];
            std::string password = jsonReq["password"];

            json response;
            response["success"] = User::registerUser(username, password);
            res.set_content(response.dump(), "application/json");
        } catch (const std::exception& e) {
            std::cerr << "Registration error: " << e.what() << std::endl;
            res.status = 400;
            res.set_content("Invalid Request", "text/plain");
        }
    });

    // 用户登录
    svr.Post("/login", [](const Request& req, Response& res) {
        try {
            auto jsonReq = json::parse(req.body);
            std::string username = jsonReq["username"];
            std::string password = jsonReq["password"];

            json response;
            if (User::loginUser(username, password)) {
                current_user = username;
                response["success"] = true;
            } else {
                response["success"] = false;
            }
            res.set_content(response.dump(), "application/json");
        } catch (const std::exception& e) {
            std::cerr << "Login error: " << e.what() << std::endl;
            res.status = 400;
            res.set_content("Invalid Request", "text/plain");
        }
    });

    // 聊天消息处理
    svr.Post("/chat", [](const Request& req, Response& res) {
        std::cout << "[INFO] /chat endpoint hit!\n";

        try {
            auto jsonReq = json::parse(req.body);
            std::string message = jsonReq["message"];
            std::cout << "Message received: " << message << std::endl;

            json response;
            response["reply"] = ChatHandler::sendToAgent(message);
            res.set_content(response.dump(), "application/json");
        } catch (const std::exception& e) {
            std::cerr << "Chat error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content("Internal Server Error", "text/plain");
        }
    });

    // ======= 启动服务器 =======
    std::cout << "Server is running on http://localhost:8080" << std::endl;
    if (!svr.listen("localhost", 8080)) {
        std::cerr << "Failed to start server!" << std::endl;
        return 1;
    }

    return 0;
}
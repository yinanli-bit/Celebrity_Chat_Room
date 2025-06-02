#include "ChatHandler.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <codecvt>
#include <iomanip>
#include <locale>
#include <string.h>
#include <nlohmann/json.hpp> // 需要包含JSON库


std::string ChatHandler::sendToAgent(const std::string &message) {
    // // ✅ 确保使用二进制模式写入原始字节
    // std::ofstream inputFile("input.txt", std::ios::binary);
    // inputFile.write(message.data(), message.size());
    // inputFile.close();

    // ✅ 打印调试信息
    std::cout << "write input.txt, long: " << message.size() << std::endl << std::flush;

    std::ofstream inputFile("D:/codes/TCproject_0_2/data/input.txt", std::ios::binary);
    if (!inputFile) {
        std::cerr << "can't open input.txt: " << strerror(errno) << std::endl << std::flush;
        return "input write fail";
    }
    inputFile.write(message.c_str(), message.size());
    inputFile.close();

    // 打印原始字节内容
    std::cout << "Raw bytes: ";
    for (char c : message) {
        std::cout << std::hex << (int)(unsigned char)c << " ";
    }
    std::cout << std::dec << std::endl << std::flush;

    // ✅ 验证文件是否生成
    std::ifstream test("D:/codes/TCproject_0_2/data/input.txt");
    if (!test) {
        std::cerr << "input.txt build fail" << std::endl;
    } else {
        std::cout << "input.txt successfully built" << std::endl;
    }

    // ✅ 使用绝对路径调用Python
    std::string scriptPath = "python D:/codes/TCproject_0_2/src/call_dashscope.py";
    int res = system(scriptPath.c_str());

    // ✅ 检查Python执行结果
    if (res != 0) {
        std::cerr << "Python脚本执行失败，错误码: " << res << std::endl;
        return "服务暂时不可用";
    }

    // ✅ 读取响应文件
    std::ifstream responseFile("D:/codes/TCproject_0_2/data/response.txt", std::ios::binary);
    if (!responseFile.is_open()) {
        std::cerr << "fail to open response.txt" << std::endl;
        return "get response fail";
    }

    std::stringstream buffer;
    buffer << responseFile.rdbuf();
    std::string response = buffer.str();
    responseFile.close();

    // ✅ 调试时暂时注释清理代码
    // std::remove("input.txt");
    // std::remove("response.txt");

    return response;
}
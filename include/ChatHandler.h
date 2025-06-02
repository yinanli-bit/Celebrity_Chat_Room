#ifndef CHATHANDLER_H
#define CHATHANDLER_H

#include <string>

class ChatHandler {
public:
    static std::string sendToAgent(const std::string &message);
};

#endif // CHATHANDLER_H

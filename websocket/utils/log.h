//
// Created by Jangit Lau on 2018/2/15.
//

#ifndef CLION_LOG_H
#define CLION_LOG_H


#include <string>

class log {
public:
    static void info(std::string reason, std::string moudle = "");
    static void warning(std::string reason, std::string moudle = "");
    static void error(std::string reason, std::string moudle = "");
};


#endif //CLION_LOG_H

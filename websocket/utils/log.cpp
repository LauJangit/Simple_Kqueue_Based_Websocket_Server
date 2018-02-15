//
// Created by Jangit Lau on 2018/2/15.
//

#include <iostream>
#include "log.h"


void log::info(std::string reason, std::string moudle){
    std::cout<<reason<<std::endl;
}

void log::warning(std::string reason, std::string moudle){
    std::cout<<reason<<std::endl;
}

void log::error(std::string reason, std::string moudle) {
    std::cout << reason << std::endl;
}
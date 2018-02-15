//
// Created by Jangit Lau on 2018/2/12.
//

#ifndef CLION_HANDSHAKE_INFO_STRUCT_H
#define CLION_HANDSHAKE_INFO_STRUCT_H

#include <string>
#include <vector>
#include "http_attr_struct.h"

typedef struct handshake_infomation_struture {
    std::string path;
    std::string host;
    std::string origin;
    std::string user_agent;
    std::vector<std::string> extension_list;
    std::vector<std::string> cookie_list;
    std::vector<http_attr> http_param_list;

}handshake_inf;

#endif //CLION_HANDSHAKE_INFO_STRUCT_H

//
// Created by Jangit Lau on 2018/2/12.
//

#ifndef CLION_HANDSHAKE_HANDLER_H
#define CLION_HANDSHAKE_HANDLER_H


#include <string>
#include <vector>

#include "../definition/byte.h"
#include "../definition/http_attr_struct.h"
#include "../definition/handshake_info_struct.h"

class handshake_handler {
    std::string header_raw_data = "";
    std::string magic_string="258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    std::string websocket_key = "";
public:
    int http_ret_code=101;
    handshake_inf handshake_info;
public:
    void append(std::string data);

    int do_prase();

    std::string get_response_string();

private:
    std::string trim(std::string str);

    void general_header_field_praser(http_attr &attr);

    void request_header_field_praser(http_attr &attr);
};


#endif //CLION_HANDSHAKE_HANDLER_H

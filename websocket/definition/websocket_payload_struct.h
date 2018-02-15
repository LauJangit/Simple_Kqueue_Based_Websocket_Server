//
// Created by Jangit Lau on 2018/2/13.
//

#ifndef CLION_WEBSOCKET_PAYLOAD_STRUCT_H
#define CLION_WEBSOCKET_PAYLOAD_STRUCT_H


#include <string>
#include <iostream>

typedef struct websocket_payload_struct {
    //
    websocket_payload_struct(std::string data, int mask, int payload_len);
    //
    websocket_payload_struct(std::string data);
    //
    std::string payload;
    //设置payload，并将其进行转换(获得请求时)
    void set_payload(std::string masked_payload, int mask, int payload_len);
    //设置payload，不进行转换(返回回应时)
    void set_payload(std::string masked_payload);
    //获取payload
    std::string get_payload();
    //获取payload(需要mask时)
    std::string get_payload(int mask);
}websocket_payload;


#endif //CLION_WEBSOCKET_PAYLOAD_STRUCT_H

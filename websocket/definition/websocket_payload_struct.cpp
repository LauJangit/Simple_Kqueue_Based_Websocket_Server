//
// Created by Jangit Lau on 2018/2/13.
//

#include "websocket_payload_struct.h"

websocket_payload_struct::websocket_payload_struct(std::string data,int mask,int payload_len) {
    this->set_payload(data, mask, payload_len);
}

websocket_payload_struct::websocket_payload_struct(std::string data) {
    this->set_payload(data);
}

//设置payload，并将其进行转换(获得请求时)
void websocket_payload_struct::set_payload(std::string masked_payload, int mask, int payload_len) {
    if (masked_payload.size() >= payload_len) {
        masked_payload = masked_payload.substr(0, payload_len);
        char cmask[4];
        memcpy(&cmask, &mask, 4);
        for (int i = 0; i < masked_payload.size(); i++) {
            this->payload.push_back(masked_payload[i] ^ cmask[i % 4]);
        }
    }
}

//设置payload，不进行转换(返回回应时)
void websocket_payload_struct::set_payload(std::string masked_payload) {
    this->payload = masked_payload;
}

//获取payload(获得请求时)
std::string websocket_payload_struct::get_payload() {
    return payload;
}

//获取payload，进行转换(返回回应时)
std::string websocket_payload_struct::get_payload(int mask) {
    std::string masked_payload;
    char cmask[4];
    memcpy(&cmask, &mask, 4);
    for (int i = 0; i < this->payload.size(); i++) {
        masked_payload.push_back(this->payload[i] ^ cmask[i % 4]);
    }
    return masked_payload;
}
//
// Created by Jangit Lau on 2018/2/15.
//

#include "websocket_base_handler.h"

void websocket_base_handler::send(std::string data,int data_type) {
    websocket_header new_header(true, false, false, false, data_type, data.size());
    websocket_payload new_payload(data);
    std::string send_data;
    send_data.append(new_header.get_raw_header(),new_header.get_header_length());
    send_data+=data;
    ::send(this->file_descriptor, send_data.c_str(), send_data.size(), 0);
}

void websocket_base_handler::shutdown(){
    std::cout<<"do shutdown"<<std::endl;
    websocket_header new_header(true, false, false, false, 0x8, 0);
    std::string send_data(new_header.get_raw_header(),new_header.get_header_length());
    ::send(this->file_descriptor, send_data.c_str(), send_data.size(), 0);
}

void websocket_base_handler::close(){
    ::close(file_descriptor);
}

void websocket_base_handler::init(const int fd,const struct sockaddr_in addr){
    file_descriptor=fd;
    memcpy(&this->client_addr,&addr, sizeof(addr));
}
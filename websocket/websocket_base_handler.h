//
// Created by Jangit Lau on 2018/2/12.
//

#ifndef CLION_WEBSOCKET_BASE_HANDLER_H
#define CLION_WEBSOCKET_BASE_HANDLER_H

#include <string.h>

#include <netinet/in.h>
#include <cmath>
#include <zconf.h>
#include <unistd.h>

#include "definition/handshake_info_struct.h"
#include "definition/websocket_header_struct.h"
#include "definition/websocket_payload_struct.h"

class websocket_base_handler {
protected:
    //
    int file_descriptor;
    //
    struct sockaddr_in client_addr;
    //
    virtual void on_open(handshake_inf&)=0;
    //
    virtual void on_close(websocket_header& header,websocket_payload& payload)=0;
    //
    virtual int on_message(websocket_header& header,websocket_payload& payload)=0;
    //
    virtual void on_error(websocket_header& header,websocket_payload& payload)=0;
    //
    void send(std::string data,int data_type);
    //
    void shutdown();
    //
    void close();
public:
    //
    void init(const int fd,const struct sockaddr_in addr);
};


#endif //CLION_WEBSOCKET_BASE_HANDLER_H

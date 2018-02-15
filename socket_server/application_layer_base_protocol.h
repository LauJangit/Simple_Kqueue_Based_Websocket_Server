//
// Created by Jangit Lau on 2018/2/11.
//

#ifndef CLION_BASE_PROTOCOL_H
#define CLION_BASE_PROTOCOL_H

#include <arpa/inet.h>

#include <unordered_map>

class application_layer_base_protocol{
protected:
    int file_descriptor;
    struct sockaddr_in client_addr;
    std::unordered_map<int, application_layer_base_protocol*>* map;
public:
    application_layer_base_protocol(int _file_descriptor,struct sockaddr_in& _client_addr){
        file_descriptor=_file_descriptor;
        client_addr=_client_addr;
    }

    //处理收到的数据 返回码 0:发送数据后继续监听 1:马上关闭连接 2:当发送完数据后关闭连接 3:不发送数据继续监听
    virtual int recv_data(std::string)=0;

    virtual std::string send_byte()=0;

    virtual ~application_layer_base_protocol(){
        map=NULL;
    }
};


#endif //CLION_BASE_PROTOCOL_H

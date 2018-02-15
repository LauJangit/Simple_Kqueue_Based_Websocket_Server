//
// Created by Jangit Lau on 2018/2/12.
//
#include "websocket/websocket_base_handler.h"
#include "websocket/definition/http_attr_struct.h"

class handler_example:public websocket_base_handler{
public:
    virtual void on_open(handshake_inf& inf){
        std::cout<<"in function on_open"<<std::endl;
        inf.http_param_list.push_back(http_attr("debug1","in function on_open"));
    }

    virtual void on_close(websocket_header& header,websocket_payload& payload){
        std::cout<<"in function on_close"<<std::endl;
    }

    virtual int on_message(websocket_header& header,websocket_payload& payload){
        std::cout<<"in function on_message: "<<payload.get_payload()<<std::endl;
        send("aaaaa",1);
        this->shutdown();
        return 0;
    }

    virtual void on_error(websocket_header& header,websocket_payload& payload){
        std::cout<<"in function on_error"<<std::endl;
    }
};
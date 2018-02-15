//
//  main.cpp
//  WebSocket
//
//  Created by Jangit Lau on 2018/2/5.
//  Copyright © 2018年 jangit.cn. All rights reserved.
//

#include "socket_server/socket_server.h"
#include "websocket/websocket.h"
#include "handler_example.cpp"

int main(int argc, const char * argv[]) {
    socket_server<websocket<handler_example>> server("0.0.0.0",10002,128,128);
    server.start();
    return 0;
}

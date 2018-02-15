//
// Created by Jangit Lau on 2018/2/11.
//

#ifndef CLION_SOCKET_SERVER_H
#define CLION_SOCKET_SERVER_H

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <algorithm>
#include <unordered_map>

#include "application_layer_base_protocol.h"
#include "../websocket/definition/byte.h"

template<class application_layer_base_protocol>
class socket_server {
private:
    std::string ip;              // IP地址
    int port;               // 绑定的端口
    int back_log;           // tcp中等待accept的数量
    int max_event_count;    // 一个kqueue中最大的文件描述符数量
    bool on_loop;      // 是否进行循环
    std::unordered_map<int, application_layer_base_protocol *> *map = NULL;  //处理的包
private:
    //停止循环
    void make_loop_stop(int signal) {
        this->on_loop = false;
    }

    //构建Socket
    int construct_socket(std::string _ip, int _port, int _back_log) {
        // 创建socket文件描述符
        int socket_file_descriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (socket_file_descriptor < 0) {
            std::cout << "获取Socket文件描述符失败" << std::endl;
            return -1;
        } else {
            std::cout << "socket_file_descriptor:" << socket_file_descriptor << std::endl;
        }

        // 构建地址
        struct sockaddr_in address;
        address.sin_family = AF_INET;
        inet_pton(AF_INET, _ip.c_str(), &(address.sin_addr));// 转换字符串形式IP到二进制形式
        address.sin_port = htons(_port);// 转换端口到对应格式

        // 绑定端口
        int bind_retcode = ::bind(socket_file_descriptor, (const struct sockaddr *) &address, sizeof(address));
        if (socket_file_descriptor < 0) {
            std::cout << "绑定端口失败" << std::endl;
            return -1;
        } else {
            std::cout << "bind_retcode:" << bind_retcode << std::endl;
        }

        // 监听端口
        int listen_retcode = listen(socket_file_descriptor, _back_log);
        if (socket_file_descriptor < 0) {
            std::cout << "监听端口失败" << std::endl;
            return -1;
        } else {
            std::cout << "listen_retcode:" << listen_retcode << std::endl;
        }
        return socket_file_descriptor;
    }

    // 初始化kevent
    int init_kqueue() {
        // 获取kqueue标识符
        int kqueue_indicator = kqueue();
        if (kqueue_indicator < 0) {
            std::cout << "获取kqueue标识符失败" << std::endl;
            return -1;
        } else {
            std::cout << "kqueue_indicator:" << kqueue_indicator << std::endl;
        }
        return kqueue_indicator;
    }

    // 注册kevent事件
    bool kevent_register(int _event_file_descriptor, int _kqueue_indicator) {
        struct kevent change;// 创建一个新的kevent
        EV_SET(&change, _event_file_descriptor, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);// 将新的事件放置于kevent中
        int ret = kevent(_kqueue_indicator, &change, 1, NULL, 0, NULL);// 注册kevent
        if (ret < 0) {
            std::cout << "注册kevent事件失败" << std::endl;
            return false;
        } else {
            std::cout << "kevent_ret_code:" << ret << std::endl;
        }
        return true;
    }

    //接受新的请求
    int on_accept(int _socket_file_descriptor, int _kqueue_indicator) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int new_file_descriptor = accept(_socket_file_descriptor, (struct sockaddr *) &client_addr, &client_addr_len);
        if (kevent_register(new_file_descriptor, _kqueue_indicator)) {
            (*map)[new_file_descriptor] = new application_layer_base_protocol(new_file_descriptor, client_addr);
            return new_file_descriptor;
        } else {
            return -1;
        }
    }

    int on_idle(int _socket_file_descriptor){
        std::cout<<"on idle"<<std::endl;
    }

    //处理已经被接受的客户端
    void on_process(int buffer_length, int client_file_descriptor) {
        byte recvChr[buffer_length];
        int recv_ret = (int) recv(client_file_descriptor, recvChr, buffer_length, 0);
        if (recv_ret >= 0) {
            application_layer_base_protocol *packet = (*map).at(client_file_descriptor);
            int ret_code = packet->recv_data(std::string(recvChr, buffer_length));
            switch (ret_code) {
                case 0: {
                    std::string send_data = packet->send_byte();
                    if (send_data.size() != 0) {
                        send(client_file_descriptor, send_data.c_str(), send_data.size(), 0);
                    }
                    break;
                }
                case 1: {
                    graceful_close(client_file_descriptor);
                    delete (*map).at(client_file_descriptor);
                    (*map).erase(client_file_descriptor);
                    break;
                }
                case 2: {
                    std::string send_data = packet->send_byte();
                    if (send_data.size() != 0) {
                        send(client_file_descriptor, send_data.c_str(), send_data.size(), 0);
                    }
                    graceful_close(client_file_descriptor);
                    delete (*map).at(client_file_descriptor);
                    (*map).erase(client_file_descriptor);
                    break;
                }
                case 3:
                    break;
                default:
                    break;
            }
        }
    }

    //处理kevent
    void handler(int _socket_file_descriptor, int _max_event_count, int _kqueue_indicator) {
        struct kevent events[_max_event_count];
        int event_counter = 0;
        while (on_loop) {
            event_counter = kevent(_kqueue_indicator, NULL, 0, events, _max_event_count, NULL);
            std::cout << "event_counter: " << event_counter << std::endl;
            if (event_counter > 0) {
                for (int i = 0; i < event_counter; i++) {
                    struct kevent event = events[i];
                    int event_data = (int) event.data;
                    int client_file_descriptor = (int) event.ident;
                    if (client_file_descriptor == _socket_file_descriptor) {
                        on_accept(_socket_file_descriptor, _kqueue_indicator);
                    }
                    if ((*map).find(client_file_descriptor) != (*map).end()) {
                        on_process(event_data, client_file_descriptor);
                    }
                }
            } else {
                on_idle(_socket_file_descriptor);
            }

        }
    }

    void graceful_close(int _socket_file_descriptor) {
        shutdown(_socket_file_descriptor, 1);
        ::close(_socket_file_descriptor);
    }

public:
    // 构造该类
    socket_server(std::string _ip, int _port, int _back_log, int _max_event_count) {
        ip = _ip;
        port = _port;
        back_log = _back_log;
        max_event_count = _max_event_count;
        on_loop = true;
        map = new std::unordered_map<int, application_layer_base_protocol *>();
    }

    //关闭该类
    ~socket_server() {
        delete map;
    }

    // 开始运行
    void start() {
        int socket_file_descriptor = construct_socket(ip, port, back_log);
        int kqueue_indicator = init_kqueue();
        if (!kevent_register(socket_file_descriptor, kqueue_indicator)) {
            return;
        }
        handler(socket_file_descriptor, max_event_count, kqueue_indicator);
        graceful_close(socket_file_descriptor);
    }
};



#endif //CLION_SOCKET_SERVER_H

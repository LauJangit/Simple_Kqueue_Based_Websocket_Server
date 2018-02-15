//
// Created by Jangit Lau on 2018/2/15.
//

#ifndef CLION_WEBSOCKET_H
#define CLION_WEBSOCKET_H

#endif //CLION_WEBSOCKET_H


#include "unistd.h"

#include "../socket_server/application_layer_base_protocol.h"
#include "handshake_handler/handshake_handler.h"
#include "definition/websocket_header_struct.h"
#include "definition/websocket_payload_struct.h"

template<class websocket_base_handler>
class websocket:public application_layer_base_protocol {
public:
    std::string recv_byte_data;
    //握手
    handshake_handler handshake_praser;
    //
    websocket_base_handler base_handler;
    //
    int websocket_status = 0; //0:完成初始化类 1:HTTP握手完成,已经建立连接

    //处理payload
    int payload_remain = 0;//payload的大小计数器
    websocket_header *header_handler = NULL;
    websocket_payload *payload_handler = NULL;

    websocket(int fd, struct sockaddr_in &addr) : application_layer_base_protocol(fd, addr) {
        base_handler.init(fd, addr);
    };

    virtual std::string send_byte() {
        if (websocket_status == 0) {
            std::string response_string = handshake_praser.get_response_string();
            websocket_status++; //标记握手成功
            return response_string;
        } else if (websocket_status == 1) {
            return "";
        } else {
            return "";
        }
    };

    virtual int recv_data(std::string data) {
        recv_byte_data += data;
        if (websocket_status == 0) {
            handshake_praser.append(recv_byte_data);
            auto http_end_pos = recv_byte_data.find("\r\n\r\n");
            if (http_end_pos != std::string::npos) {
                //如果检查到http头的结尾
                handshake_praser.do_prase();
                base_handler.on_open(handshake_praser.handshake_info);
                recv_byte_data = recv_byte_data.erase(0, http_end_pos + 4);
                if (handshake_praser.http_ret_code == 101) {
                    return 0; //二次握手后成功进入websocket状态
                } else {
                    return 2; //握手失败，回复后关闭连接
                }
            } else {
                return 0;
            }
        } else if (websocket_status == 1) {
            while (true) {
                if (payload_remain == 0) {
                    if (recv_byte_data.size() >= 2) {
                        if (header_handler == NULL) {
                            header_handler = new websocket_header(recv_byte_data);
                        } else {
                            byte optcode = header_handler->get_opcode();
                            delete header_handler;
                            header_handler = NULL;
                            header_handler = new websocket_header(recv_byte_data);
                            header_handler->set_opcode(optcode);
                        }
                        byte optcode = header_handler->get_opcode();
                        switch (optcode) {
                            case 0x1:
                            case 0x2:
                                break;
                            case 0x8:
                                base_handler.on_close(*header_handler, *payload_handler);
                                return 1;
                            case 0x9:
                                //Ping Frame
                                send_PONG();
                                break;
                            case 0xA:
                                //Pong Frame
                                break;
                            default:
                                base_handler.on_error(*header_handler, *payload_handler);
                                return 1;
                        }
                        recv_byte_data.erase(0, header_handler->get_header_length());
                        payload_remain = header_handler->get_PayloadLen();
                    } else {
                        break;
                    }
                } else if (payload_remain > 0) {
                    if (recv_byte_data.size() >= payload_remain) {
                        if (payload_handler == NULL) {
                            payload_handler = new websocket_payload(recv_byte_data,
                                                                    header_handler->get_MaskingKey(),
                                                                    payload_remain);
                        } else {
                            payload_handler->set_payload(recv_byte_data, header_handler->get_MaskingKey(),
                                                         payload_remain);
                        }
                        recv_byte_data.erase(0, payload_remain);
                        payload_remain = 0;
                        if (header_handler->get_FIN() == 1) {
                            byte optcode = header_handler->get_opcode();
                            int ret_code;
                            switch (optcode) {
                                case 0x1:
                                case 0x2:
                                    ret_code = base_handler.on_message(*header_handler, *payload_handler);
                                    break;
                                case 0x8:
                                case 0x9:
                                case 0xA:
                                    break;
                                default:
                                    base_handler.on_error(*header_handler, *payload_handler);
                                    return 1;
                            }
                            delete payload_handler;
                            payload_handler = NULL;
                            delete header_handler;
                            header_handler = NULL;
                        }
                    } else {
                        return 3;
                    }
                }
            }
            return 3;
        } else {
            return 1;
        }
    };

    virtual void on_idle() {

    }

    virtual ~websocket() {
        if (header_handler == NULL) {
            delete header_handler;
            header_handler = NULL;
        }
        if (payload_handler == NULL) {
            delete payload_handler;
            payload_handler = NULL;
        }
        application_layer_base_protocol::~application_layer_base_protocol();
    }

private:
    void send_PING() {
        websocket_header new_header(true, false, false, false, 0x9, 0);
        std::string send_data(new_header.get_raw_header(), new_header.get_header_length());
        ::send(this->file_descriptor, send_data.c_str(), send_data.size(), 0);
    }

    void send_PONG() {
        websocket_header new_header(true, false, false, false, 0xA, 0);
        std::string send_data(new_header.get_raw_header(), new_header.get_header_length());
        ::send(this->file_descriptor, send_data.c_str(), send_data.size(), 0);
    }
};

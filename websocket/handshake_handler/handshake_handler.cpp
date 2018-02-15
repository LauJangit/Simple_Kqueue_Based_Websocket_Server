//
// Created by Jangit Lau on 2018/2/12.
//

#include "handshake_handler.h"
#include "../utils/hash_function.h"

void handshake_handler::append(std::string data) {
    auto header_data=data.substr(0,data.find("\r\n\r\n")+4);
    header_raw_data.append(header_data);
}

int handshake_handler::do_prase(){
    std::vector<http_attr> params_list;
    bool insert_into_value=false;
    for(int i=0;i<header_raw_data.size();i++){
        char chr=header_raw_data[i];
        if(chr==':'){
            insert_into_value=true;
        }
        if(chr=='\r'){
            continue;
        }
        if(chr=='\n'||params_list.empty()){
            insert_into_value=false;
            params_list.push_back(http_attr());
            if(chr!='\n'){
                params_list.back().append_name(chr);
            }
        }else{
            if(!insert_into_value){
                params_list.back().append_name(chr);
            }else{
                params_list.back().append_value(chr);
            }
        }
    }
    for(auto it=params_list.begin();it!=params_list.end();it++){
        it->attr_value.erase(it->attr_value.begin());
        if(it==params_list.begin()){
            general_header_field_praser(*it);
        }else{
            request_header_field_praser(*it);
        }
    }
}

void handshake_handler::general_header_field_praser(http_attr& attr) {
    std::string header_attr = attr.attr_name;
    auto http_version_itr = header_attr.rfind(" HTTP/1.1");
    if (http_version_itr != std::string::npos) {
        header_attr.erase(header_attr.begin() + http_version_itr, header_attr.end());
    } else {
        http_ret_code = 400;
        return;
    }
    if (header_attr.find("GET") == 0 || header_attr.find("get") == 0) {
        header_attr.erase(header_attr.begin(), header_attr.begin() + 3);
    } else {
        http_ret_code = 405;
        return;
    }
    this->handshake_info.path = header_attr;
}

void handshake_handler::request_header_field_praser(http_attr& attr) {
    std::string attr_name = trim(attr.attr_name);
    std::string attr_value = trim(attr.attr_value);
    if (attr_name.compare("Host") == 0) {
        this->handshake_info.host = attr_value;
    } else if (attr_name.compare("Connection") == 0) {
        if (attr_value.find("Upgrade") == std::string::npos
            && attr_value.find("upgrade") == std::string::npos
            && attr_value.find("UPGRADE") == std::string::npos) {
            http_ret_code = 400;
            std::cout << "Connection" << " Error" << std::endl;
            return;
        }
    } else if (attr_name.compare("Upgrade") == 0) {
        if (attr_value.find("websocket") == std::string::npos && attr_value.find("WEBSOCKET") == std::string::npos) {
            http_ret_code = 400;
            std::cout << "Upgrade" << " Error" << std::endl;
            return;
        }
    } else if (attr_name.compare("Origin") == 0) {
        this->handshake_info.origin = attr_value;
    } else if (attr_name.compare("Sec-WebSocket-Version") == 0) {
        if (atoi(attr_value.c_str()) != 13) {
            http_ret_code = 426;
            std::cout << "Sec-WebSocket-Version" << " Error" << std::endl;
            return;
        }
    } else if (attr_name.compare("User-Agent") == 0) {
        this->handshake_info.user_agent = attr_value;
    } else if (attr_name.compare("Sec-WebSocket-Key") == 0) {
        this->websocket_key = attr_value;
    } else if (attr_name.compare("Cookie") == 0) {
        if (attr_value.find(";") != std::string::npos) {
            while (attr_value.find(";")!=std::string::npos) {
                auto itr = attr_value.find(";");
                std::string cookie = trim(attr_value.substr(0, itr));
                this->handshake_info.cookie_list.push_back(cookie);
                attr_value = attr_value.erase(0, itr + 1);
            }
        }
        this->handshake_info.cookie_list.push_back(trim(attr_value));
    } else if (attr_name.compare("Sec-WebSocket-Extensions") == 0) {
        if (attr_value.find(";") != std::string::npos) {
            while (attr_value.find(";") != std::string::npos) {
                auto itr = attr_value.find(";");
                std::string cookie = trim(attr_value.substr(0, itr));
                this->handshake_info.extension_list.push_back(cookie);
                attr_value = attr_value.erase(0, itr + 1);
            }
        }
        this->handshake_info.extension_list.push_back(trim(attr_value));
    }
}

std::string handshake_handler::trim(std::string str)
{
    if (str.empty()){
        return str;
    }
    str.erase(0,str.find_first_not_of(" "));
    str.erase(str.find_last_not_of(" ") + 1);
    return str;
}

std::string handshake_handler::get_response_string() {
    std::vector<http_attr> attr_list;
    if (http_ret_code == 101) {
        std::string new_key = hash_function::to_binary_sha1(websocket_key + this->magic_string);
        attr_list.push_back(http_attr("Upgrade", "websocket"));
        attr_list.push_back(http_attr("Connection", "Upgrade"));
        attr_list.push_back(http_attr("Sec-WebSocket-Accept", hash_function::to_base64(new_key)));
    } else {

    }
    std::string content = "";
    switch (http_ret_code) {
        case 101:
            content += "HTTP/1.1 101 Switching Protocols\r\n";
            break;
        case 400:
            content += "HTTP/1.1 400 Bad Request\r\n";
            break;
        case 405:
            content += "HTTP/1.1 405 Method Not Allowed\r\n";
            break;
        case 426:
            content += "HTTP/1.1 426 Upgrade Required\r\n";
            break;
        default:
            content += "HTTP/1.1 500 Internal Server Error\r\n";
            break;
    }
    for(auto attr:this->handshake_info.http_param_list){
        attr_list.push_back(attr);
    }
    attr_list.push_back(http_attr("Server", "Jangit`s WebSocket Server"));
    for (auto attr : attr_list) {
        content += attr.to_string();
    }
    content += "\r\n";
    return content;
}
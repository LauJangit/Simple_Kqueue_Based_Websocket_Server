//
// Created by Jangit Lau on 2018/2/11.
//

#include "http_attr_struct.h"

void http_attr_struct::append_name(char chr){
    attr_name+=chr;
}

void http_attr_struct::append_value(char chr){
    attr_value+=chr;
}

http_attr_struct::http_attr_struct(std::string _attr_name,std::string _attr_value){
    attr_name=_attr_name;
    attr_value=_attr_value;
}

std::string http_attr_struct::to_string(){
    return ((attr_name.append(": ")).append(attr_value)).append("\r\n");
}
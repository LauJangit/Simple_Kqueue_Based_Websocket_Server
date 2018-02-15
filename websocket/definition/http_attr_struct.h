//
// Created by Jangit Lau on 2018/2/11.
//

#ifndef CLION_HTTP_ATTR_H
#define CLION_HTTP_ATTR_H

#include <iostream>

typedef struct http_attr_struct{
    //single attribute name
    std::string attr_name;
    //single attribute value
    std::string attr_value;
    //
    void append_name(char chr);
    //
    void append_value(char chr);
    //
    http_attr_struct()= default;
    //
    http_attr_struct(std::string _attr_name,std::string _attr_value);
    //
    std::string to_string();
}http_attr;

#endif //CLION_HTTP_ATTR_H

//
// Created by Jangit Lau on 2018/2/9.
//

#ifndef CLION_HASH_FUNCTION_H
#define CLION_HASH_FUNCTION_H


#include <iostream>
#include <string>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

class hash_function {
public:
    //clear datd to sha1
    static std::string to_sha1(std::string clear_data);

    static std::string to_binary_sha1(std::string clear_data);

    //clear data to base64
    static std::string to_base64(std::string clear_data);

};

#endif //CLION_HASH_FUNCTION_H

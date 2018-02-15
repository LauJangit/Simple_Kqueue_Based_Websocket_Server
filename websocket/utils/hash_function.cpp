//
// Created by Jangit Lau on 2018/2/15.
//

#include "hash_function.h"

std::string hash_function::to_sha1(std::string clear_data){
    char *clear_data_seq= const_cast<char *>(clear_data.c_str());
    unsigned char temp[SHA_DIGEST_LENGTH];
    char buf[SHA_DIGEST_LENGTH*2];

    memset(buf, 0x0, SHA_DIGEST_LENGTH*2);
    memset(temp, 0x0, SHA_DIGEST_LENGTH);

    SHA1((unsigned char *)clear_data_seq, strlen(clear_data_seq), temp);

    for (int i=0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf((char*)&(buf[i*2]), "%02x", temp[i]);
    }
    return buf;
}

std::string hash_function::to_binary_sha1(std::string clear_data){
    char *clear_data_seq= const_cast<char *>(clear_data.c_str());
    unsigned char temp[SHA_DIGEST_LENGTH];

    memset(temp, 0x0, SHA_DIGEST_LENGTH);
    SHA1((unsigned char *)clear_data_seq, strlen(clear_data_seq), temp);

    return std::string((char*)temp,SHA_DIGEST_LENGTH);
}

//clear data to base64
std::string hash_function::to_base64(std::string clear_data) {
    unsigned char* buffer= const_cast<unsigned char *>((unsigned char *)clear_data.c_str());
    int buffer_length=clear_data.length();
    BIO *bio=NULL;
    BIO *b64=NULL;
    BUF_MEM *bufferPtr=NULL;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Ignore newlines - write everything in one line
    BIO_write(bio, buffer, buffer_length);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);
    BIO_set_close(bio, BIO_NOCLOSE);
    BIO_free_all(bio);
    return std::string((*bufferPtr).data,bufferPtr->length);
}
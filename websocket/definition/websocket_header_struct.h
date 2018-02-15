//
// Created by Jangit Lau on 2018/2/12.
//

#ifndef CLION_WEBSOCKET_HEADER_STRUCT_H
#define CLION_WEBSOCKET_HEADER_STRUCT_H


#include <cstring>

#include "byte.h"

typedef struct websocket_header_struct {
private:
    byte header[14];
    int payload_len_len = 0;
public:
    //FIN(byte)                              // 1 Bit
    byte get_FIN();
    void set_FIN(byte FIN);

    //RSV1(byte)                             // 1 Bit
    byte get_RSV1();
    void set_RSV1(byte RSV1);

    //RSV2(byte)                             // 1 Bit
    byte get_RSV2();
    void set_RSV2(byte RSV2);

    //RSV3(byte)                             // 1 Bit
    byte get_RSV3();
    void set_RSV3(byte RSV3);

    //opcode(byte)                           // 4 Bit
    byte get_opcode();
    void set_opcode(byte opcode);

    //MASK(byte)                             // 1 Bit
    bool has_MASK();
    void set_MASK(bool have_mask);

    //PayloadLen(byte)                       // 7 Bit
    //ExtendedPayloadLen(unsigned long long) // 64 Bit/16Bit/(Option)
    unsigned long long get_PayloadLen();
    void set_PayloadLen(unsigned long long PayloadLen);

    //MaskingKey(int)                        // 32 Bit/(Option)
    int get_MaskingKey();
    void set_MaskingKey(int MaskingKey);

    //constructor (for receive)
    websocket_header_struct(std::string data);
    //constructor (for send)
    websocket_header_struct(bool FIN, bool RSV1, bool RSV2, bool RSV3, byte optcode, unsigned long long payload_len);

    //get the header's size after analysis header
    int get_header_length();

    //get header's bytes after header constructed
    byte *get_raw_header();


}websocket_header;


#endif //CLION_WEBSOCKET_HEADER_STRUCT_H

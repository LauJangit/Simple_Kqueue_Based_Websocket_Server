//
// Created by Jangit Lau on 2018/2/12.
//

#include <iostream>
#include "websocket_header_struct.h"


//FIN(byte)                              // 1 Bit
byte websocket_header_struct::get_FIN() {
    byte FIN = (header[0] & 128) >> 7;
    return FIN;
}

void websocket_header_struct::set_FIN(byte FIN) {
    if (FIN == 0) {
        header[0] = header[0] & 127;
    } else {
        header[0] = header[0] | 128;
    }
}

//RSV1(byte)                             // 1 Bit
byte websocket_header_struct::get_RSV1() {
    byte RSV1 = (header[0] & 64) >> 6;
    return RSV1;
}

void websocket_header_struct::set_RSV1(byte RSV1) {
    if (RSV1 == 0) {
        header[0] = header[0] & 191;
    } else {
        header[0] = header[0] | 64;
    }
}

//RSV2(byte)                             // 1 Bit
byte websocket_header_struct::get_RSV2() {
    byte RSV2 = (header[0] & 32) >> 5;
    return RSV2;
}

void websocket_header_struct::set_RSV2(byte RSV2) {
    if (RSV2 == 0) {
        header[0] = header[0] & 223;
    } else {
        header[0] = header[0] | 32;
    }
}

//RSV3(byte)                             // 1 Bit
byte websocket_header_struct::get_RSV3() {
    byte RSV3 = (header[0] & 16) >> 4;
    return RSV3;
}

void websocket_header_struct::set_RSV3(byte RSV3) {
    if (RSV3 == 0) {
        header[0] = header[0] & 239;
    } else {
        header[0] = header[0] | 16;
    }
}

//opcode(byte)                           // 4 Bit
byte websocket_header_struct::get_opcode() {
    byte opcode = (header[0] & 15);
    return opcode;
}

void websocket_header_struct::set_opcode(byte opcode) {
    if (opcode > 16) {
        return;
    }
    header[0] = (header[0] & 240) | (opcode & 15);
}

//MASK(byte)                             // 1 Bit
bool websocket_header_struct::has_MASK() {
    byte MASK = (header[1] & 128) >> 7;
    return MASK == 1;
}

void websocket_header_struct::set_MASK(bool have_mask){
    if(have_mask){
        header[1] = (header[1] & 127) | 128;
    }else{
        header[1] = header[1] & 127;
    }
}

//PayloadLen(byte)                       // 7 Bit
//ExtendedPayloadLen(unsigned long long) // 64 Bit/16Bit/(Option)
unsigned long long websocket_header_struct::get_PayloadLen() {
    byte main_payload_len = (byte) (header[1] & 127);
    if (main_payload_len == 126) {
        unsigned short len = ((unsigned char) header[2] << 8) | ((unsigned char) header[3]);
        return len;
    } else if (main_payload_len == 127) {
        unsigned long long len = ((unsigned char) header[2] << 56) | ((unsigned char) header[3] << 48) |
                                 ((unsigned char) header[4] << 40) | ((unsigned char) header[5] << 32) |
                                 ((unsigned char) header[6] << 24) | ((unsigned char) header[7] << 16) |
                                 ((unsigned char) header[8] << 8) | ((unsigned char) header[9]);
        return len;
    } else {
        return main_payload_len;
    }
}

void websocket_header_struct::set_PayloadLen(unsigned long long PayloadLen) {
    if (PayloadLen < 126) {
        byte len = (byte) PayloadLen;
        header[1] = len & 0xFF;
        payload_len_len = 0;
    } else if (PayloadLen <= 65536) {
        header[1] = 126 & 0xFF;
        unsigned short len = PayloadLen;
        header[2] = (len >> 8) & 0xFF;
        header[3] = len & 0xFF;
        payload_len_len = 2;
    } else {
        header[1] = 127 & 0xFF;
        unsigned long long len = PayloadLen;
        header[2] = (len >> 56) & 0xFF;
        header[3] = (len >> 48) & 0xFF;
        header[4] = (len >> 40) & 0xFF;
        header[5] = (len >> 32) & 0xFF;
        header[6] = (len >> 24) & 0xFF;
        header[7] = (len >> 16) & 0xFF;
        header[8] = (len >> 8) & 0xFF;
        header[9] = len & 0xFF;
        payload_len_len = 8;
    }
}

//MaskingKey(int)                        // 32 Bit/(Option)
int websocket_header_struct::get_MaskingKey() {
    if (this->has_MASK()) {
        int MaskingKey = 0;
        memcpy(&MaskingKey, &header[2 + payload_len_len], sizeof(int));
        return MaskingKey;
    } else {
        return 0;
    }
}

void websocket_header_struct::set_MaskingKey(int MaskingKey) {
    header[1] = (header[1] & 127) | 128;
    memcpy(&header[2 + payload_len_len], &MaskingKey, sizeof(unsigned int));
}

//constructor (for receive)
websocket_header_struct::websocket_header_struct(std::string data) {
    memcpy(&(this->header), data.c_str(), 14);
    int main_payload_len = (byte) (header[1] & 127);
    if (main_payload_len < 126) {
        payload_len_len = 0;
    } else if (main_payload_len == 126) {
        payload_len_len = 2;
    } else if (main_payload_len == 127) {
        payload_len_len = 8;
    }
}

//constructor (for send)
websocket_header_struct::websocket_header_struct(bool FIN, bool RSV1, bool RSV2, bool RSV3, byte optcode, unsigned long long payload_len) {
    if (FIN) { this->set_FIN(1); } else { this->set_FIN(0); }
    if (RSV1) { this->set_RSV1(1); } else { this->set_RSV1(0); }
    if (RSV1) { this->set_RSV2(1); } else { this->set_RSV2(0); }
    if (RSV1) { this->set_RSV3(1); } else { this->set_RSV3(0); }
    this->set_opcode(optcode);
    this->set_PayloadLen(payload_len);
    this->set_MASK(false);
}

//get the header's size after analysis header
int websocket_header_struct::get_header_length() {
    get_MaskingKey();
    if (has_MASK()) {
        return 6 + payload_len_len;
    } else {
        return 2 + payload_len_len;
    }
}

//get header's bytes after header constructed
byte* websocket_header_struct::get_raw_header() {
    return this->header;
}
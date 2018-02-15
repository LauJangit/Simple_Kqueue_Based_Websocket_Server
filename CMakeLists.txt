cmake_minimum_required(VERSION 3.7)
project(Clion)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES websocket/definition/byte.h websocket/definition/http_attr_struct.cpp websocket/definition/http_attr_struct.h socket_server/application_layer_base_protocol.h socket_server/socket_server.h websocket/utils/hash_function.h main.cpp websocket/websocket.h websocket/definition/websocket_header_struct.cpp websocket/definition/websocket_header_struct.h websocket/websocket_base_handler.h websocket/websocket_base_handler.cpp handler_example.cpp websocket/handshake_handler/handshake_handler.cpp websocket/handshake_handler/handshake_handler.h websocket/definition/handshake_info_struct.h websocket/definition/websocket_payload_struct.cpp websocket/definition/websocket_payload_struct.h websocket/utils/log.h websocket/utils/log.cpp websocket/utils/log.h websocket/utils/hash_function.cpp websocket/websocket_base_handler.cpp)
#set(SOURCE_FILES main.cpp)
set(INC_DIR include/)
set(LINK_DIR libs/)

include_directories(${INC_DIR})
link_directories(${LINK_DIR})
link_libraries(ssl crypto)

add_executable(Clion ${SOURCE_FILES})
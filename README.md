# Simple_Kqueue_Based_Websocket_Server

使用C++编写的WebSocket服务端
- 使用Kqueue作为异步IO架构
- 使用Clion编译，可以运行在MacOS上
- 使用了OpenSSL作为计算SHA1以及BASE64的库

目前部分功能没有实现:
- 不支持拓展(如deflate)
- 不支持定时发送Ping包(因为kqueue模型在未接收到数据时是阻塞的)

所有用于处理websocket协议的类都需要继承websocket/websocket_base_handler.h中的websocket_base_handler类，才能处理Websocket协议

其中handler_example.cpp中的handler_example类是标准的范例

以handler_example.cpp为例，需要继承四个函数:on_open/on_close/on_error/on_message

- 函数1: virtual void on_open(handshake_inf&);
在Websocket握手时会被调用，用于处理握手返回的包，譬如在这里告诉客户端支持基于deflate的压缩功能

- 函数2: virtual void on_close(websocket_header&,websocket_payload&);
在Websocket接受到关闭数据包时会被调用，可以用来在关闭时移除用户数据

- 函数3: virtual int on_message(websocket_header&,websocket_payload&);
在Websocket接收到数据时会被调用，只有在此方法下可以调用close()以及send()函数用于关闭websocket连接以及发送数据
返回值定义: 1:马上关闭连接 3:不发送数据继续监听

- 函数4: virtual void on_error(websocket_header&,websocket_payload&);
在Websocket出错时，该函数会被调用

#pragma once

#ifndef __OKE_CLIENT_HPP_
#define __OKE_CLIENT_HPP_

#include <cstdint>
#include<string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>


#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
#endif
#ifdef __LINUX__
#include <unistd.h>
#include <arpa/inet.h>
#endif

namespace OneKeyEngine
{
namespace Internet
{
class Client
{
#ifdef _WIN32
    SOCKET client_socket;
#endif
#ifdef __LINUX__
    int client_socket;
#endif
    struct sockaddr_in server_address;
    std::vector<unsigned char>buffer;
public:
    bool update=false;
    bool link=false;
    Client(const std::string& address,u_short port);
    ~Client();
    std::string connect_server(const std::string& address,u_short port);
    void break_link();
    int send(const char* data, int dataSize);
    std::vector<unsigned char>& receive();
    std::vector<unsigned char>& get_buffer();
};

#ifdef _WIN32
Client::Client(const std::string& address, u_short port) {
    if(address.empty())
        return;
    connect_server(address,port);
}

Client::~Client() {
    break_link();
}

inline std::string Client::connect_server(const std::string &address, u_short port)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed" << std::endl;
        return "WSAStartup failed";
    }

    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket == INVALID_SOCKET)
    {
        std::cerr << "Error creating socket" << std::endl;
        WSACleanup();
        return "Error creating socket";
    }

    u_long mode = 1; // 1 to enable non-blocking socket
    ioctlsocket(client_socket, FIONBIO, &mode);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    if (inet_pton(AF_INET, address.c_str(), &server_address.sin_addr) <= 0)
    {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return "Invalid address/ Address not supported";
    }

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR)
    {
        if (WSAGetLastError() == WSAEWOULDBLOCK)
        {
            fd_set writeSet;
            FD_ZERO(&writeSet);
            FD_SET(client_socket, &writeSet);

            timeval timeout;
            timeout.tv_sec = 5; // 设置超时时间为5秒
            timeout.tv_usec = 0;

            if (select(0, NULL, &writeSet, NULL, &timeout) > 0)
            {
                int optval;
                int optlen = sizeof(optval);
                getsockopt(client_socket, SOL_SOCKET, SO_ERROR, (char *)&optval, &optlen);
                if (optval != 0)
                {
                    std::cerr << "Error in delayed connection" << std::endl;
                    closesocket(client_socket);
                    WSACleanup();
                    return "Error in delayed connection";
                }
            }
            else
            {
                std::cerr << "Connection timeout" << std::endl;
                closesocket(client_socket);
                WSACleanup();
                return "Connection timeout";
            }
        }
        else
        {
            std::cerr << "Error connecting to server" << std::endl;
            closesocket(client_socket);
            WSACleanup();
            return "Error connecting to server";
        }
    }

    mode = 0; // 0 to disable non-blocking socket
    ioctlsocket(client_socket, FIONBIO, &mode);
    link=true;
    return "connect success";
}

inline void Client::break_link()
{
    closesocket(client_socket);
    WSACleanup();
    link=false;
}

int Client::send(const char* data, int dataSize) {
    return ::send(client_socket, static_cast<const char*>(data), dataSize, 0);
}

std::vector<unsigned char>& Client::receive() {
    update=false;
	buffer.resize(1024);
	auto ret=recv(client_socket,(char*)buffer.data(),buffer.size(),0);
	if (ret <= 0) {
		std::cout << "接收服务器数据失败" << std::endl;
	}
    else
    {
        update=true;
    }
    return buffer;
}
inline std::vector<unsigned char> &Client::get_buffer()
{
    update=false;
    return buffer;
}
#endif

#ifdef __LINUX__
Client::Client(const std::string& address, u_short port) {
    // 创建套接字
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址和端口
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    inet_pton(AF_INET, address.c_str(), &server_address.sin_addr);

    // 连接到服务器
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Error connecting to server" << std::endl;
        close(client_socket);
        exit(EXIT_FAILURE);
    }
}

Client::~Client() {
    // 关闭套接字
    close(client_socket);
}

void Client::send(const void* data, unsigned int dataSize) {
    ::send(client_socket, data, dataSize, 0);
}

std::vector<unsigned char>& Client::receive() {
    char buffer[1024];
    int bytes_received;

    // 接收文件大小
    bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    buffer[bytes_received] = '\0';
    int file_size = std::stoi(buffer);

    // 发送确认接收大小的消息
    ::send(client_socket, "ACK", 3, 0);

    // 清空接收缓冲区
    this->buffer.clear();
    this->buffer.reserve(file_size);

    // 接收文件内容
    int total_received = 0;
    while (total_received < file_size) {
        bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        this->buffer.insert(this->buffer.end(), buffer, buffer + bytes_received);
        total_received += bytes_received;
    }

    this->update = true;
    return this->buffer;
}
#endif

    
} // namespace Internet

} // namespace OneKeyEngine
#endif
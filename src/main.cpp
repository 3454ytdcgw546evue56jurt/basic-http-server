#include <stdio.h>
#include <stdlib.h>
#if defined(_WIN32)
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <ws2spi.h>
    #include <Windows.h>
#elif defined(__linux__)
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <sys/ioctl.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
#endif
#include <unistd.h>
#include <cerrno>
#include <thread>
#include "http.h"
#include <cstring>
#include <vector>

using namespace std;

#if defined(_WIN32)
    SOCKET HTTPSocket;
#elif defined(__linux__)
    int HTTPSocket;
#endif

void HTTP_server_clean_up()
{
    close(HTTPSocket);
    printf("Server closed \n");
}

int main(int argc,char **args)
{
    printf("Starting web server \n");
    std::atexit(HTTP_server_clean_up);
    #if defined(_WIN32)
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);    
        HTTPSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    #elif defined(__linux__)
        HTTPSocket = socket(AF_INET, SOCK_STREAM, 0);
    #endif

    #if defined(__linux__)
        if(HTTPSocket < 0)
        {
            const char *error_message = strerror(HTTPSocket);

            printf("Error opening server socket:%s, code: %d\n",error_message,HTTPSocket);
            exit(HTTPSocket);
        }
    #elif defined(_WIN32)
        if (HTTPSocket == INVALID_SOCKET) 
        {
            printf("Error opening server socket:%s, code: %d\n",WSAGetLastError(),HTTPSocket);
            exit(HTTPSocket);
        }
    #endif

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    #if defined(__linux__)
        int bind_result = bind(HTTPSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    #elif defined(_WIN32)
        int bind_result = bind(HTTPSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    #endif
    #if defined(__linux__)
        if(bind_result < 0)
        {
            const char *error_message = strerror(HTTPSocket);

            printf("Error binding server socket:%s, code: %d\n",error_message,bind_result);
            exit(bind_result);
        }
    #elif defined(_WIN32)
        if(HTTPSocket == INVALID_SOCKET) 
        {
            printf("Error binding server socket:%s, code: %d\n",WSAGetLastError(),bind_result);
            exit(bind_result);
        }
    #endif

    int lisetn_res = listen(HTTPSocket, 1);
    #if defined(__linux__)
        if(lisetn_res < 0)
        {
            printf("Error listening on a port: %s, code:%d\n",strerror(lisetn_res), lisetn_res);
            exit(lisetn_res);
        }
    #elif defined(_WIN32)
        if(HTTPSocket < 0)
        {
            printf("Error listening on a port: %s, code:%d\n",WSAGetLastError(),bind_result);
            exit(bind_result);
        }
    #endif

    //Initilising HTTP utilities found in http.h
    Init_HTTP_Utils();

    //TODO CLI switch for minimal http version of config

    printf("Web server started \n");
    while(1)
    {
        //Every client gets it's own thread.
        //TODO make a config/CLI option to made it instead spawn a new process
        int clientSocket = accept(HTTPSocket, nullptr, nullptr);
        if(clientSocket >= 0)
        {
            printf("Client connection... \n");
            std::thread thread(Server_http_thread, clientSocket);
            thread.detach();
        }
    }
}
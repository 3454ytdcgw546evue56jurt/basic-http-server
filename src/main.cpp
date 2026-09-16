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

void HTTP_server_clean_up()
{
    //TODO server cleanup
}

int main(int argc,char **args)
{
    printf("Starting web server \n");
    std::atexit(HTTP_server_clean_up);
    #if defined(_WIN32)
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);    
        SOCKET HTTPSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    #elif defined(__linux__)
        int HTTPSocket = socket(AF_INET, SOCK_STREAM, 0);
    #endif

    #if defined(__linux__)
        if(HTTPSocket < 0)
        {
            const char *error_message = "Unknown error";
            switch(HTTPSocket)
            {
                case EACCES:
                {
                    error_message = "EACCES, Permission denied";
                }
                break;
                case EAFNOSUPPORT:
                {
                    error_message = "EAFNOSUPPORT, Address family not supported";
                }
                break;
                case EINVAL:
                {
                    error_message = "EINVAL, Invalid argument";
                }
                break;
                case EMFILE:
                {
                    error_message = "EMFILE, Process has too many open file descriptors";
                }
                break;
                case ENFILE:
                {
                    error_message = "ENFILE, System has too many open files";
                }
                break;
                case ENOBUFS:
                {
                    error_message = "ENOBUFS, Insufficient buffer resources";
                }
                break;
                case ENOMEM:
                {
                    error_message = "ENOMEM, Insufficient memory";
                }
                break;
                case EPROTONOSUPPORT:
                {
                    error_message = "EPROTONOSUPPORT, Protocol not supported";
                }
                break;
                case EPROTOTYPE:
                {
                    error_message = "EPROTOTYPE, Socket type incompatible with protocol";
                }
                break;
                #if defined(__linux__)
                    case ESOCKTNOSUPPORT:
                    {
                        error_message = "ESOCKTNOSUPPORT, Socket type not supported";
                    }
                #endif
                default:
                {
                    error_message = "Unknown error";
                }
                break;
            }

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
            const char *error_message = "uknown error";
            switch(bind_result)
            {
                case EACCES:
                {
                    error_message = "EACCES, Permission denied";
                }
                break;
                case EADDRINUSE:
                {
                    error_message = "EADDRINUSE, Address/port is already in use or Another socket is already using the address/port";
                }
                break;
                case EADDRNOTAVAIL:
                {
                    error_message = "EADDRNOTAVAIL, Address doesnt exist on this machine";
                }
                break;
                case EBADF:
                {
                    error_message = "EBADF, Invalid socket file descriptor";
                }
                break;
                case EINVAL:
                {
                    error_message = "EINVAL, Invalid argument / socket already bound";
                }
                break;
                case ENOTSOCK:
                {
                    error_message = "ENOTSOCK, sockfd isn't a socket";
                }
                break;
                case EAFNOSUPPORT:
                {
                    error_message = "EAFNOSUPPORT, Address family isn't supported";
                }
                break;
                case EFAULT:
                {
                    error_message = "EFAULT	Address points outside accessible memory";
                }
                break;
                default:
                {
                    error_message = "Unknown error";
                }
                break;
            }

            close(HTTPSocket);
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
    if(lisetn_res < 0)
    {
        //TODO error handling
        exit(lisetn_res);
    }

    //Initilising HTTP utilities found in http.h
    Init_HTTP_Utils();

    printf("Web server started \n");
    while(1)
    {
        //Every client gets it's own thread.
        //TODO make a config option to made it instead spawn a new process
        int clientSocket = accept(HTTPSocket, nullptr, nullptr);
        if(clientSocket >= 0)
        {
            printf("Client connection... \n");
            std::thread thread(Server_http_thread, clientSocket);
            thread.detach();
        }
    }

    close(HTTPSocket);
}
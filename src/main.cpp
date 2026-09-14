#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>
#include <thread>
#include "http.h"
#include <cstring>
#include <vector>

using namespace std;

int main(int argc,char **args)
{
    printf("Starting web server \n");
    int HTTPSocket = socket(AF_INET, SOCK_STREAM, 0);
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
            case ESOCKTNOSUPPORT:
            {
                error_message = "ESOCKTNOSUPPORT, Socket type not supported";
            }
            default:
            {
                error_message = "Unknown error";
            }
            break;
        }

        printf("Error opening server socket:%s, code: %d\n",error_message,HTTPSocket);
        exit(HTTPSocket);
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    int bind_result = bind(HTTPSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if(bind_result < 0)
    {
        const char *error_message = "uknown error";
        
        switch(HTTPSocket)
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
        printf("Error binding server socket:%s, code: %d\n",error_message,HTTPSocket);
        exit(HTTPSocket);
    }

    int lisetn_res = listen(HTTPSocket, 1);
    if(lisetn_res < 0)
    {
        //TODO error handling
        exit(lisetn_res);
    }

    Init_HTTP_Utils();

    printf("Web server started \n");
    while(1)
    {
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
#include <stdio.h>
#include <stdlib.h>
#if defined(_WIN32)
    #include <winsock2.h>
    #include <ws2tcpip.h>
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
#include <string>

using namespace std;

struct HTTP_request_type
{
    HTTP_request_type_enums Enum;
    const char *name;

    HTTP_request_type(HTTP_request_type_enums _Enum,const char *_name)
    {
        Enum =_Enum;
        name = _name;
    }
};
vector<HTTP_request_type> RequestTypes;

void Init_HTTP_Utils()
{
    RequestTypes.push_back(HTTP_request_type(GET,"GET"));
    RequestTypes.push_back(HTTP_request_type(POST,"POST"));
    RequestTypes.push_back(HTTP_request_type(PUT,"PUT"));
    RequestTypes.push_back(HTTP_request_type(PATCH,"PATCH"));
    RequestTypes.push_back(HTTP_request_type(DELETE_,"DELETE"));
};

HTTP_request Parse_HTTP_request(char Data[],int Request_size)
{
    HTTP_request Parsed_request = HTTP_request();

    //If anyone had better idea i'm all ears
    //C and C++ don't have a good newline implementation
    int header_line_size = 0;
    header_line_size = strchr(Data, '\n')-Data;
    printf("header_line_size %d \n",header_line_size);

    char Header[header_line_size+1];

    memcpy(Header,Data,header_line_size);
    //I'm not shooting my self in the foot this time.
    Header[header_line_size+1] = 0;

    return Parsed_request;
}

void Server_http_thread(int connection_socket)
{
    while(1)
    {
        #if defined(__linux__)
            int Data_buffer_size = 0;
            ioctl(connection_socket, FIONREAD, &Data_buffer_size);
        #elif defined(_WIN32)
            u_long Data_buffer_size = 0;
            ioctlsocket(connection_socket, FIONREAD, &Data_buffer_size);
        #endif
        
        char Data_buffer[Data_buffer_size];
        int data_size = recv(connection_socket, Data_buffer, Data_buffer_size, 0);

        if(data_size > 0)
        {
            HTTP_request request = Parse_HTTP_request(Data_buffer,Data_buffer_size);

            const char* server_response =
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html; charset=UTF-8\r\n"
                        "Content-Length: 24\r\n"
                        "Connection: close\r\n"
                        "\r\n"
                        "<h1>Hello from C++!</h1>";
            send(connection_socket,server_response, strlen(server_response), 0); 

            printf("Received data:\n %s \n",Data_buffer);
        }
        else if(data_size < 0)
        {
            //TODO error handling
        }
        else if(data_size == 0)
        {
            printf("Client disconnected... \n");
            close(connection_socket);
            return;
        }
    }
}

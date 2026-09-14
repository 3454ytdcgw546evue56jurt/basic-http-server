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


struct HTTP_request_type
{
    HTTP_request_type_enums Enum;
    char *name;

    HTTP_request_type(HTTP_request_type_enums _Enum,const char *_name)
    {
        Enum =_Enum;
        name = name;
    }
};
vector<HTTP_request_type> RequestTypes;

void Init_HTTP_Utils()
{
    RequestTypes.push_back(HTTP_request_type(GET,"GET"));
    RequestTypes.push_back(HTTP_request_type(POST,"POST"));
    RequestTypes.push_back(HTTP_request_type(PUT,"PUT"));
    RequestTypes.push_back(HTTP_request_type(PATCH,"PATCH"));
    RequestTypes.push_back(HTTP_request_type(DELETE,"DELETE"));
};

HTTP_request Parse_HTTP_request(char Data[],int Request_size)
{
    HTTP_request Parsed_request = HTTP_request();

    char header_space_check = 0;
    int header_space_check_index = 0;
    while(space_check != *" ")
    {
        header_space_check = Data[header_space_check_index];

        if(header_space_check_index > Request_size)
        {
            //TODO Error out.
            //Either this is not HTTP or connection is fucked.
        }
        header_space_check_index++;
    }

    return Parsed_request;
}

void Server_http_thread(int connection_socket)
{
    while(1)
    {
        int Data_buffer_size = 0;
        ioctl(connection_socket, FIONREAD, &Data_buffer_size);
        
        char Data_buffer[Data_buffer_size];
        int data_size = recv(connection_socket, Data_buffer, Data_buffer_size, 0);

        HTTP_request request = Parse_HTTP_request(Data_buffer,Data_buffer_size);

        const char* server_response =
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/html; charset=UTF-8\r\n"
                    "Content-Length: 24\r\n"
                    "Connection: close\r\n"
                    "\r\n"
                    "<h1>Hello from C++!</h1>";
        send(connection_socket,server_response, strlen(server_response), 0);

        if(data_size > 0)
        {
            printf("Received data:\n %s \n",Data_buffer);
        }
        else if(data_size < 0)
        {
            //TODO error handling
        }
        else if(data_size == 0)
        {
            printf("Client disconnected... \n");
            return;
        }
    }
}

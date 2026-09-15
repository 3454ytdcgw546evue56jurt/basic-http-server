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

    //getting the request_type
    int RequestTypes_size = RequestTypes.size();
    const char *request_type_searched = nullptr;
    char *request_type_checked = nullptr;
    HTTP_request_type_enums HTTP_request_type_enum = INVALID;
    for(int i =0;i<RequestTypes_size;i++)
    {
        request_type_searched = RequestTypes.at(i).name;

        request_type_checked = strstr(Header, request_type_searched);

        if(request_type_checked != nullptr)
        {
            HTTP_request_type_enum = RequestTypes.at(i).Enum;
        }
    }
    Parsed_request.type = HTTP_request_type_enum;

    //getting the version
    char *version;
    version = strstr(Header, "HTTP/")+5;
    char *major_minor_version_seperator;
    major_minor_version_seperator = strstr(Header, ".")+1;
    
    //parsin the major version
    int major_version_char_size = major_minor_version_seperator-version;
    char major_version_char[major_version_char_size];
    memccpy(major_version_char,version,major_version_char_size,1);
    int major_version = atoi(major_version_char);

    //parsing the minor version
    int minor_version_char_size = strlen(major_minor_version_seperator);
    char minor_version_char[minor_version_char_size];
    memccpy(minor_version_char,major_minor_version_seperator,minor_version_char_size,1);
    int minor_version = atoi(minor_version_char);

    Parsed_request.http_major_version = major_version;
    Parsed_request.http_minor_version = minor_version;
    //doing this way more memory and cpu eficiente and easier
    //Win for everyone

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

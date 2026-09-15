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
#include "Utils.h"
#include <cstring>
#include <vector>
#include <string>

using namespace std;

vector<HTTP_request_type> RequestTypes;
vector<content_type> content_types;

void Init_HTTP_Utils()
{
    RequestTypes.push_back(HTTP_request_type(GET,"GET"));
    RequestTypes.push_back(HTTP_request_type(POST,"POST"));
    RequestTypes.push_back(HTTP_request_type(PUT,"PUT"));
    RequestTypes.push_back(HTTP_request_type(PATCH,"PATCH"));
    RequestTypes.push_back(HTTP_request_type(DELETE_,"DELETE"));

    //Type application:
    content_types.push_back(content_type(application_java_archive,"application/java-archive"));
    content_types.push_back(content_type(application_EDI_X12,"application/EDI-X12")); 
    content_types.push_back(content_type(application_EDIFACT,"application/EDIFACT"));
    content_types.push_back(content_type(application_javascript,"application/javascript (obsolete)"));
    content_types.push_back(content_type(application_octet_stream, "application/octet-stream"));
    content_types.push_back(content_type(application_ogg, "application/ogg"));
    content_types.push_back(content_type(application_pdf, "application/pdf"));
    content_types.push_back(content_type(application_xhtml_xml, "application/xhtml+xml"));
    content_types.push_back(content_type(application_x_shockwave_flash, "application/x-shockwave-flash"));
    content_types.push_back(content_type(application_json, "application/json"));
    content_types.push_back(content_type(application_ld_json, "application/ld+json"));
    content_types.push_back(content_type(application_xml, "application/xml"));
    content_types.push_back(content_type(application_zip, "application/zip"));
    content_types.push_back(content_type(application_x_www_form_urlencoded, "application/x-www-form-urlencoded"));

    //Type audio:
    content_types.push_back(content_type(audio_mpeg, "audio/mpeg"));
    content_types.push_back(content_type(audio_x_ms_wma, "audio/x-ms-wma"));
    content_types.push_back(content_type(audio_vnd_rn_realaudio, "audio/vnd.rn-realaudio"));
    content_types.push_back(content_type(audio_x_wav, "audio/x-wav"));

    //Type image:
    content_types.push_back(content_type(image_gif, "image/gif"));
    content_types.push_back(content_type(image_jpeg, "image/jpeg"));
    content_types.push_back(content_type(image_png, "image/png"));
    content_types.push_back(content_type(image_tiff, "image/tiff"));
    content_types.push_back(content_type(image_vnd_microsoft_icon, "image/vnd.microsoft.icon"));
    content_types.push_back(content_type(image_x_icon, "image/x-icon"));
    content_types.push_back(content_type(image_vnd_djvu, "image/vnd.djvu"));
    content_types.push_back(content_type(image_svg_xml, "image/svg+xml"));

    //Type multipart:
    content_types.push_back(content_type(multipart_mixed, "multipart/mixed"));
    content_types.push_back(content_type(multipart_alternative, "multipart/alternative"));
    content_types.push_back(content_type(multipart_related, "multipart/related (using by MHTML (HTML mail).)"));
    content_types.push_back(content_type(multipart_form_data, "multipart/form-data"));

    //Type text:
    content_types.push_back(content_type(text_css, "text/css"));
    content_types.push_back(content_type(text_csv, "text/csv"));
    content_types.push_back(content_type(text_event_stream, "text/event-stream"));
    content_types.push_back(content_type(text_html, "text/html"));
    content_types.push_back(content_type(text_javascript, "text/javascript"));
    content_types.push_back(content_type(text_plain, "text/plain"));
    content_types.push_back(content_type(text_xml, "text/xml"));

    //Type video:
    content_types.push_back(content_type(video_mpeg, "video/mpeg"));
    content_types.push_back(content_type(video_mp4, "video/mp4"));
    content_types.push_back(content_type(video_quicktime, "video/quicktime"));
    content_types.push_back(content_type(video_x_ms_wmv, "video/x-ms-wmv"));
    content_types.push_back(content_type(video_x_msvideo, "video/x-msvideo"));
    content_types.push_back(content_type(video_x_flv, "video/x-flv"));
    content_types.push_back(content_type(video_webm, "video/webm"));

    //Type vnd:
    content_types.push_back(content_type(application_vnd_android_package_archive, "application/vnd.android.package-archive"));
    content_types.push_back(content_type(application_vnd_oasis_opendocument_text, "application/vnd.oasis.opendocument.text"));
    content_types.push_back(content_type(application_vnd_oasis_opendocument_spreadsheet, "application/vnd.oasis.opendocument.spreadsheet"));
    content_types.push_back(content_type(application_vnd_oasis_opendocument_presentation, "application/vnd.oasis.opendocument.presentation"));
    content_types.push_back(content_type(application_vnd_oasis_opendocument_graphics, "application/vnd.oasis.opendocument.graphics"));
    content_types.push_back(content_type(application_vnd_ms_excel, "application/vnd.ms-excel"));
    content_types.push_back(content_type(application_vnd_openxmlformats_officedocument_spreadsheetml_sheet, "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"));
    content_types.push_back(content_type(application_vnd_ms_powerpoint, "application/vnd.ms-powerpoint"));
    content_types.push_back(content_type(application_vnd_openxmlformats_officedocument_presentationml_presentation, "application/vnd.openxmlformats-officedocument.presentationml.presentation"));
    content_types.push_back(content_type(application_msword, "application/msword"));
    content_types.push_back(content_type(application_vnd_openxmlformats_officedocument_wordprocessingml_document, "application/vnd.openxmlformats-officedocument.wordprocessingml.document"));
    content_types.push_back(content_type(application_vnd_mozilla_xul_xml, "application/vnd.mozilla.xul+xml"));
};

HTTP_request Parse_HTTP_request(char Data[],int Request_size)
{
    printf("Parsing HTTP request \n");
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
    
    //Reading the meta data
    int curr_metadata_line_index = 1;
    char *curr_metadata_line = Get_line(Data,curr_metadata_line_index);

    while(curr_metadata_line != nullptr)
    {
        curr_metadata_line = Get_line(Data,curr_metadata_line_index);
        curr_metadata_line_index++;

        printf("curr_metadata_line %s \n",curr_metadata_line);
    }

    printf("HTTP request parsed\n");

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

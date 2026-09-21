#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
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
    char *current_http_line = Get_line(Data,1);
    HTTP_request Parsed_request = HTTP_request();

    int header_line_size = strlen(current_http_line);

    //getting the request_type
    int RequestTypes_size = RequestTypes.size();
    const char *request_type_searched = nullptr;
    char *request_type_checked = nullptr;
    HTTP_request_type_enums HTTP_request_type_enum = INVALID;
    for(int i =0;i<RequestTypes_size;i++)
    {
        request_type_searched = RequestTypes.at(i).name;

        request_type_checked = strstr(current_http_line, request_type_searched);

        if(request_type_checked != nullptr)
        {
            HTTP_request_type_enum = RequestTypes.at(i).Enum;
            break;
        }
    }
    Parsed_request.type = HTTP_request_type_enum;
    //parsing the requested file
    int Requested_file_start = 0;
    int Requested_file_end = 0;
    int Requested_file_size = 0;
    char *Requested_file = nullptr;
    if(request_type_checked != nullptr)
    {
        Requested_file_start = Char_at(request_type_checked,'/')+1;
        Requested_file_size = Char_at(current_http_line,' ',Requested_file_start);

        Requested_file = (char *)malloc(Requested_file_size+1);
        
        for(int i = 0;i<Requested_file_size;i++)
        {
            Requested_file[i] = request_type_checked[Requested_file_start+i];
        }

        Requested_file[Requested_file_size] = 0x0;
    }

    Parsed_request.path = Requested_file;

    if(strlen(Parsed_request.path) == 0)
    {
        Parsed_request.path = "index.html";
    }

    //getting the version
    char *version = strstr(current_http_line, "HTTP/")+5;
    
    //parsin the major version
    int major_version_char_size = Char_at(version,'.');
    char major_version_char[major_version_char_size];
    memccpy(major_version_char,version,major_version_char_size,1);
    int major_version = atoi(major_version_char);

    //parsing the minor version
    int minor_version_char_size = strlen(version)-(major_version_char_size);
    char minor_version_char[minor_version_char_size];
    memccpy(minor_version_char,&version[major_version_char_size+1],minor_version_char_size,1);
    int minor_version = atoi(minor_version_char);

    Parsed_request.http_major_version = major_version;
    Parsed_request.http_minor_version = minor_version;
    //To lazy to make it better
    
    //Reading the meta data
    free(current_http_line);
    int current_http_line_index = 2;

    while(current_http_line != nullptr)
    {
        current_http_line = Get_line(Data,current_http_line_index);
        current_http_line_index++;

        if(current_http_line != nullptr)
        {
            //TODO
            //FIXME
            //curr_metadata_line Gets cutoff and turns into urr_metadata_line until the last one
            //Also the last two are as follows empty and garbage data.
            //Possible issue with Get_line
            printf("curr_metadata_line %s \n",current_http_line);
            Parsed_request.meta_datas.push_back(current_http_line);
        }
    }

    printf("HTTP request parsed\n");

    return Parsed_request;
}

int curr_connection_socket;
int min_major_ver = 1;
int min_minor_ver = 1;

void Server_http_thread_cleanup()
{
    printf("Client disconnected... \n");
    close(curr_connection_socket);
}

void End_http_thread(int code)
{
    Server_http_thread_cleanup();
    #if defined(_WIN32)
        ExitThread(code);
    #elif defined(__linux__)
        pthread_exit(nullptr);
    #endif
}

void Server_http_thread(int connection_socket)
{
    std::atexit(Server_http_thread_cleanup);
    curr_connection_socket = connection_socket;

    while(1)
    {
        #if defined(__linux__)
            int Data_buffer_size = 0;
            ioctl(curr_connection_socket, FIONREAD, &Data_buffer_size);
        #elif defined(_WIN32)
            u_long Data_buffer_size = 0;
            ioctlsocket(curr_connection_socket, FIONREAD, &Data_buffer_size);
        #endif
        
        char Data_buffer[Data_buffer_size];
        int data_size = recv(curr_connection_socket, Data_buffer, Data_buffer_size, 0);

        if(data_size > 0)
        {
            HTTP_request request = Parse_HTTP_request(Data_buffer,Data_buffer_size);

            if(request.http_major_version < min_major_ver || request.http_minor_version < min_minor_ver)
            {
                const char* server_response_505 = "HTTP/1.1 505 HTTP Version Not Supported\r\nContent-Length: 0\r\nConnection: close";

                send(curr_connection_socket,server_response_505, strlen(server_response_505), 0);
                printf("Requested http version: %d.%d minimal: %d.%d\n",
                                                                        request.http_major_version,
                                                                        request.http_minor_version,
                                                                        min_major_ver,
                                                                        min_minor_ver);
                End_http_thread(0);
            }

            switch(request.type)
            {
                case GET:
                {
                    char *Requested_data = File_text_load(request.path);
                    char *accepted_content_types = nullptr;
                    char *meta_data_name = nullptr;
                    int accepted_content_type_size = request.meta_datas.size();

                    //For now this is how wwe sand bakc accepted content types
                    //TODO Check contenty types before loading and parsing a file
                    for(int i = 0;i<accepted_content_type_size;i++)
                    {
                        meta_data_name = Get_Metadata_name(request.meta_datas.at(i));
                        if(meta_data_name == nullptr)
                        {
                            meta_data_name = "";
                        }

                        if(strcmp(meta_data_name,"Accept") == 0)
                        {
                            accepted_content_types = Get_Metadata_contents(request.meta_datas.at(i));
                            break;
                        }
                        else if(accepted_content_types != nullptr)
                        {
                            free(accepted_content_types);
                            accepted_content_types = nullptr;
                        }

                        free(meta_data_name);
                    }

                    if(Requested_data == nullptr)
                    {
                        const char* server_response_404 = "HTTP/1.1 404 Resource not found\r\nContent-Length: 0\r\nConnection: close";

                        send(curr_connection_socket,server_response_404, strlen(server_response_404), 0);
                        printf("File not %s found... \n",request.path);
                        End_http_thread(0);
                    }

                    int Requested_data_size = strlen(Requested_data);

                    //TODO some connection wanna stay open
                    const char* server_response_template = 
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: %s\r\n"
                        "Content-Length: %d\r\n"
                        "Connection: close\r\n"
                        "\r\n"
                        "%s";
                    
                    int server_response_size = snprintf(nullptr,0,server_response_template,accepted_content_types,Requested_data_size,Requested_data);
                    char *server_response = (char *) malloc(server_response_size+1);
                    snprintf(server_response,server_response_size,server_response_template,accepted_content_types,Requested_data_size,Requested_data);
                    server_response[server_response_size] = 0x0;
                    
                    send(curr_connection_socket,server_response, strlen(server_response), 0);
                    free(server_response); 
                    free(accepted_content_types);

                    End_http_thread(0);
                }
                break;
                default:
                {
                    const char* server_response_500 = "HTTP/1.1 500 Internal server error\r\nContent-Length: 0\r\nConnection: close";
                    send(curr_connection_socket,server_response_500, strlen(server_response_500), 0);

                    //getting the request type name
                    int RequestTypes_size = RequestTypes.size();
                    const char *RequestType_Name = "Invalid";
                    HTTP_request_type_enums HTTP_request_type_enum = INVALID;
                    for(int i =0;i<RequestTypes_size;i++)
                    {
                        if(request.type == RequestTypes.at(i).Enum)
                        {
                            RequestType_Name = RequestTypes.at(i).name;
                            break;
                        }
                    }

                    printf("Invalid  or uniplemented request %s\n",RequestType_Name);
                    End_http_thread(-1);
                }
                break;
            }
        }
        else if(data_size < 0)
        {
            const char* server_response_500 = "HTTP/1.1 500 Internal server error\r\nContent-Length: 0\r\nConnection: close";
            send(curr_connection_socket,server_response_500, strlen(server_response_500), 0);

            #if defined(__linux__)
                if(data_size < 0)
                {
                    printf("Error receving a packet: %s, code:%d\n",strerror(curr_connection_socket), curr_connection_socket);
                }
            #elif defined(_WIN32)
                if(data_size < 0)
                {
                    printf("Error receving a packet: %s, code:%d\n",WSAGetLastError(),curr_connection_socket);
                }
            #endif
            End_http_thread(-1);
        }
        else if(data_size == 0)
        {
            End_http_thread(0);
        }
    }
}

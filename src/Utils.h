#pragma once
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <filesystem>

using namespace std;

//Standart c library doesn't have this basic function ¯\_(ツ)_/¯
//Dumb as fuck
char *Get_line(char *Text,unsigned int line)
{
    if(Text == nullptr)
    {
        return nullptr;
    }

    int Text_lenght = strlen(Text)+1;
    int current_line = 0;
    int line_start = 0;
    int line_end = 0;

    for(int i = 0;i<Text_lenght;i++)
    {
        if(current_line == line)
        {
            break;
        }

        if(Text[i] == '\n' || Text[i] == '\r\n' || Text[i] == 0x0)
        {
            //hacky but works
            line_start = line_end;
            
            if(line_start != 0)
            {
                line_start = line_end+1;
            }
            
            current_line++;
            line_end = i;
        }
    }

    if(current_line != line)
    {
        return nullptr;
    }

    int line_size = line_end - line_start;
    if(line_size <= 0)
    {
        return nullptr;
    }
    char *new_line = (char *) malloc(line_size+1);
    
    for(int i = 0;i<line_size;i++)
    {
        new_line[i] = Text[line_start+i];
    }
    
    new_line[line_size] = 0x0;

    return new_line;
}

//Thi might exists in standart libary, but i made it my self becose why the fuck not.
int Char_at(char *Text,char character)
{
    int Text_lenght = strlen(Text)+1;

    for(int i = 0;i<Text_lenght;i++)
    {
        if(Text[i] == character)
        {
            return i;
        }
    }
    
    return -1;
}

int Char_at(char *Text,char character,int offset)
{
    int Text_lenght = strlen(Text)+1;

    for(int i = 0;i<Text_lenght;i++)
    {
        if(Text[offset+i] == character)
        {
            return i;
        }
    }
    
    return -1;
}

char *File_text_load(char *FileName)
{
    FILE* Project_file;
    Project_file = fopen(FileName, "r");

    if(Project_file == NULL)
    {
        return NULL;
    }

    int Project_file_size = std::filesystem::file_size(FileName);
    char *file_contents = (char*) malloc(Project_file_size);
    fread(file_contents,Project_file_size,1,Project_file);

    fclose(Project_file);

    return file_contents;
}

char *Get_Metadata_contents(char *Metadata)
{
    int contents_start = Char_at(Metadata,': ')+1;
    char curr_contents_char = 0x1;
    int contents_start_size = 0;

    while(curr_contents_char != 0x0)
    {
        curr_contents_char = Metadata[contents_start+contents_start_size];
        contents_start_size++;
    }
    
    if(contents_start_size <= 0)
    {
        return nullptr;
    }

    char *contents = (char *)malloc(contents_start_size+1);
    for(int i = 0;i<contents_start_size;i++)
    {
        contents[i] = Metadata[contents_start+i];
    }
    contents[contents_start_size] = 0x0;

    return contents;
}

char *Get_Metadata_name(char *Metadata)
{
    int contents_namesize = Char_at(Metadata,': ')-1;

    if(contents_namesize <= 0)
    {
        return nullptr;
    }

    char *contents = (char *)malloc(contents_namesize+1);
    for(int i = 0;i<contents_namesize;i++)
    {
        contents[i] = Metadata[i];
    }
    contents[contents_namesize] = 0x0;
    
    return contents;
}

char *file_get_type(char *filename)
{
    int filename_length = strlen(filename);
    int file_type_start = 0;
    
    for(int i = filename_length;i>0;i--)
    {
        if(filename[i] == '.')
        {
            file_type_start = i+1;
            break;
        }
    }

    int file_type_size = filename_length-file_type_start;

    if(file_type_size <= 0)
    {
        return nullptr;
    }

    char *file_type = (char *) malloc(file_type_size+1);
    for(int i = 0;i<file_type_size;i++)
    {
        file_type[i] = filename[file_type_start+i];
    }

    file_type[file_type_size] = 0x0;

    return file_type;
}
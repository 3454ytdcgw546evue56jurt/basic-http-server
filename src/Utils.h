#pragma once
#include <cstring>
#include <cstdlib>
#include <cstdio>

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
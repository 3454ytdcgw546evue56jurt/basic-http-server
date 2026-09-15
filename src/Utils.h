#pragma once
#include <cstring>

//Standart c library doesn't have this basic function ¯\_(ツ)_/¯
//Dumb as fuck
char *Get_line(char *Text,int line)
{
    if(Text == nullptr)
    {
        return nullptr;
    }

    int Current_line = 0;
    char current_Text_char = Text[0];
    int current_Text_char_index = 0;

    while(current_Text_char != 0 && Current_line >= line)
    {
        current_Text_char = Text[current_Text_char_index];

        if(current_Text_char == '\n')
        {
            Current_line++;
        }

        current_Text_char_index++;
    }

    //Incase the line we were searching was to high
    if(Current_line != line)
    {
        return  nullptr;
    }

    return &Text[current_Text_char_index];
}
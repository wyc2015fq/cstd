# linux终端下实现的conio.h中的getch()功能函数 - Koma Hub - CSDN博客
2018年09月15日 17:57:02[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：164

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
char getch_from_terminal()
{
	FILE *input;
    FILE *output;
    struct termios initial_settings, new_settings;
    int inputString;
    input = fopen("/dev/tty", "r");
    output = fopen("/dev/tty", "w");
    if(!input || !output)
    {
        fprintf(stderr, "Unable to open /dev/tty\n");
        exit(1);
    }
    tcgetattr(fileno(input), &initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    new_settings.c_lflag &= ~ISIG;
    if(tcsetattr(fileno(input), TCSANOW, &new_settings) != 0)
    {
        fprintf(stderr, "Could not set attributes\n");
    }
    while(inputString != 'q')
    {
        do
        {
            inputString = fgetc(input);
        }while(inputString == '\n' || inputString == '\r');
        return (char )inputString;
    }
    tcsetattr(fileno(input), TCSANOW, &initial_settings);
}
```
转自：[https://blog.csdn.net/Timsley/article/details/51424068](https://blog.csdn.net/Timsley/article/details/51424068)

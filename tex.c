#include <stdio.h>
#include <unistd.h>
#include <termios.h>

void RawmodeEnable(){
    {
        struct termios rawmode;

        tcgetattr(STDIN_FILENO, &rawmode);

        rawmode.c_lflag &= ~(ECHO);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &rawmode);
        /* data */
    };
    
}

int main(){

    RawmodeEnable();
    char i;
    while ( read(STDIN_FILENO, &i, 1) == 1 && i != 'q');
    return 0;
}
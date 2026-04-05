#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>


struct termios orig_termios;
void RawmodeDisable() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void RawmodeEnable(){
    {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(RawmodeDisable);

    struct termios rawmode = orig_termios;
    rawmode.c_iflag &= ~(IXON); // Disable Ctrl + S and Q
    rawmode.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG); // disable Ctrl + C, Z and V signals to prevent program kill

        tcsetattr(STDIN_FILENO, TCSAFLUSH, &rawmode);
    };
    
}

int main(){

    RawmodeEnable();
    char c;
    while ( read(STDIN_FILENO, &c, 1) == 1 && c != 'q'){
        if (iscntrl(c)){
            printf("%d\n", c);
        }
        else{
            printf("%d ('%i')", c, c);
        }
    }
    return 0;
}
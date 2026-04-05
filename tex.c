// Library Includes //

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>

// Data //

struct termios orig_termios;

// Terminal //

void die(const char *s) {
  perror(s);
  exit(1);
}

void RawmodeDisable() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");
}

void RawmodeEnable(){
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(RawmodeDisable);

    struct termios rawmode = orig_termios;

    rawmode.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON); // Disable Ctrl + S and Q , Ctrl + M
    rawmode.c_oflag &= ~(OPOST);
    rawmode.c_cflag |= (CS8);
    rawmode.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG); // disable Ctrl + C, Z and V signals to prevent program kill
    rawmode.c_cc[VMIN] = 0;
    rawmode.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &rawmode) == -1) die("tcsetattr");
    }

// Init //

int main(){

    RawmodeEnable();
    while (1) {
        char c = '\0';
        if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read");
        if (iscntrl(c)){
            printf("%d\r\n", c);
        }
        if(c == 'q') break;
    }
    return 0;
}
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

static int rows, cols;

static bool get_terminal_size(int *rows, int *cols)
{
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        fprintf(stderr, "ERROR: failed to get terminal window size\n");
        return false;
    }
    printf("%u x %u\n", w.ws_row, w.ws_col);
    printf("ws_xpixel: %u\n", w.ws_xpixel);
    printf("ws_ypixel: %u\n", w.ws_ypixel);
    *rows = w.ws_row;
    *cols = w.ws_col;
    return true;
}

static void handle_winch(int sig)
{
    printf("\x1b[2J"); // Clear entire screen
    printf("\x1b[H");  // Move cursor to top-left
    get_terminal_size(&rows, &cols);
}

static void handle_int(int sig)
{
    printf("\x1b[0m");   // Restore Select Graphic Rendition (SGR) attributes
    printf("\x1b[2J");   // Clear entire screen
    printf("\x1b[H");    // Move cursor to top-left
    printf("\x1b[?25h"); // Show cursor
    fflush(stdout);
    _exit(0);
}

int main(void)
{
    pid_t pid = getpid();
    printf("PID: %d\n", pid);

    struct sigaction sa = {0};
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handle_winch;
    if (sigaction(SIGWINCH, &sa, NULL) == -1) {
        fprintf(stderr, "ERROR: failed to set signal handler\n");
        return 1;
    }

    sa.sa_handler = handle_int;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        fprintf(stderr, "ERROR: failed to set signal handler\n");
        return 1;
    }

    get_terminal_size(&rows, &cols);

    struct termios oldt, newt;
    if (tcgetattr(STDIN_FILENO, &oldt) == -1) {
        fprintf(stderr, "ERROR: Unable to get terminal attributes\n");
        return 1;
    }

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO | ISIG);
    newt.c_cc[VMIN]  = 0; // Minimum number of characters to read
    newt.c_cc[VTIME] = 1; // Timeout in deciseconds for noncanonical read
    if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) == -1) {
        fprintf(stderr, "ERROR: Unable to set terminal attributes\n");
        return 1;
    }

    printf("\x1b[?25l"); // Hide cursor
    printf("\x1b]0;Pong\x07");

    int x = 0;
    int y = 0;
    while (true) {
        // Poll input
        char c;
        ssize_t res = read(STDIN_FILENO, &c, 1);
        if (res == 1) {
            if (c == 0x1b) {
                printf("0x%x", c);
                while (read(STDIN_FILENO, &c, 1) == 1) {
                    printf(" 0x%x", c);
                }
                printf("\n");
            } else {
                if (c == 0x03)      printf("INTR  Ctrl-C\n");
                else if (c == 0x1c) printf("QUIT  Ctrl-\\\n");
                else if (c == 0x1a) printf("SUSP  Ctrl-Z\n");
                else if (c == 0x19) printf("DSUSP Ctrl-Y\n");
                else {
                    printf("read: %c 0x%x\n", c, c);
                    if (c == 'q') break;
                    if (c == 'h') x -= 1;
                    if (c == 'l') x += 1;
                    if (c == 'j') y += 1;
                    if (c == 'k') y -= 1;
                }
            }
        }

        // Render
        printf("\x1b[2J"); // Clear entire screen
        printf("\x1b[H");  // Move cursor to top-left

        // Clear background
        uint8_t r = 10;
        uint8_t g = 10;
        uint8_t b = 10;
        printf("\x1b[48;2;%u;%u;%um", r, g, b); // Set background color
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                printf(" ");
            }
        }

        printf("\x1b[48;2;%u;%u;%um", 111, 111, 111); // Set background color
        printf("\x1b[%u;%uH", y+1, x+1); // Move cursor to row: y, col: x
        for (int row = 0; row < 10; row++) {
            for (int col = 0; col < 10; col++) {
                printf(" ");
            }
            printf("\x1b[%u;%uH", y+1+row, x+1); // Move cursor to next row
        }

        fflush(stdout);
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    handle_int(0);

    return 0;
}

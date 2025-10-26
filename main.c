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

    struct termios oldt;
    if (tcgetattr(STDIN_FILENO, &oldt) == -1) {
        fprintf(stderr, "ERROR: Unable to get terminal attributes\n");
        return 1;
    }
    printf("termios:\n");
    printf("  c_iflag: 0x%x\n", oldt.c_iflag);
    printf("  c_oflag: 0x%x\n", oldt.c_oflag);
    printf("  c_cflag: 0x%x\n", oldt.c_cflag);
    printf("  c_lflag: 0x%x\n", oldt.c_lflag); // IEXTEN|ECHOKE|ECHOCTL|ECHOK|ECHOE|ECHO|ICANON|ISIG
    printf("  ISIG:   0x%x\n", ISIG);
    printf("  ICANON: 0x%x\n", ICANON);
    printf("  XCASE:  0x%x\n", XCASE);
    printf("  ECHO:   0x%x\n", ECHO);
    printf("  ECHOE:  0x%x\n", ECHOE);
    printf("  ECHOK:  0x%x\n", ECHOK);
    printf("  ECHONL: 0x%x\n", ECHONL);
    printf("  ECHOCTL:0x%x\n", ECHOCTL);
    printf("  ECHOPRT:0x%x\n", ECHOPRT);
    printf("  ECHOKE: 0x%x\n", ECHOKE);
    printf("  FLUSHO: 0x%x\n", FLUSHO);
    printf("  NOFLSH: 0x%x\n", NOFLSH);
    printf("  TOSTOP: 0x%x\n", TOSTOP);
    printf("  PENDIN: 0x%x\n", PENDIN);
    printf("  IEXTEN: 0x%x\n", IEXTEN);
    exit(0);

    printf("\x1b[?25l"); // Hide cursor
    printf("\x1b]0;New title\x07");

    while (1) {
        printf("\x1b[2J"); // Clear entire screen
        printf("\x1b[H");  // Move cursor to top-left

        // Clear background
        uint8_t r = 80;
        uint8_t g = 0;
        uint8_t b = 100;
        printf("\x1b[48;2;%u;%u;%um", r, g, b); // Set background color
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                printf(" ");
            }
        }

        printf("\x1b[48;2;%u;%u;%um", 0, 100, 0); // Set background color
        printf("\x1b[%u;%uH", 0, 0); // Move cursor to next row
        for (int row = 0; row < 10; row++) {
            for (int col = 0; col < 10; col++) {
                printf(" ");
            }
            printf("\x1b[%u;%uH", row, 0); // Move cursor to next row
        }

        fflush(stdout);
        sleep(1);
    }

    return 0;

    //printf("Hello");
    //printf("\x1b[%uD", 5); // Move cursor 5 cells back
    //printf("Good bye\n");

    //printf("\x1b[%u;%uH", 10, 10); // Move cursor to row 10, column 10
    //printf("Hello\n");

    //printf("\x1b[0J"); // Clear from cursor to end of the screen
    //printf("\x1b[1J"); // Clear from cursor to beginning of the screen
    //printf("\x1b[2J"); // Clear entire screen
    //printf("\x1b[3J"); // Clear entire screen and delete lines saved in scrollback buffer

    //printf("\x1b[%u;%uH", 1, 1); // Move cursor to row 1, column 1

    for (int color = 0; color <= 7; color++) {
        printf("ESC[3%um: ", color);
        printf("\x1b[3%umHello\x1b[0m\n", color); // Set foreground color
    }

    for (int color = 0; color <= 7; color++) {
        printf("ESC[4%um: ", color);
        printf("\x1b[4%umHello\x1b[0m\n", color); // Set background color
    }

    //printf("\x1b[999;999H");
    //printf("\x1b[6n"); // Reports the cursor position (CPR) by transmitting ESC[n;mR, where n is the row and m the column

    //printf("\x1b[38;2;255;0;0mHello\n\x1b[0m");

    return 0;
}

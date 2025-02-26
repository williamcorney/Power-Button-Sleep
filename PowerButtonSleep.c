#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

// Function to check if a key is pressed without blocking
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);    // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);  // Put the character back if one is read
        return 1;
    }

    return 0;
}

int main() {
    // Command to stream logs and filter for power button events
    char *command = "log stream --predicate 'eventMessage contains \"power button pressed\"'";

    // Start the log stream process
    FILE *log_stream = popen(command, "r");
    if (log_stream == NULL) {
        perror("Failed to start log stream");
        return 1;
    }

    char line[1024];

    // Small delay to ensure we flush any previous logs
    sleep(2);  // Delay for 2 seconds before processing logs to avoid old events

    printf("Monitoring for power button press...\n");

    // Continuously read the log stream and look for power button events
    while (fgets(line, sizeof(line), log_stream)) {
        // Check if the line contains the power button press event
        if (strstr(line, "power button pressed,") != NULL) {
            printf("Power button pressed detected! Starting countdown...\n");

            // Start a 5-second countdown
            int countdown = 5;
            while (countdown > 0) {
                printf("Sleep in %d seconds...\n", countdown);
                sleep(1);
                countdown--;

                // Check if a key is pressed to skip the sleep
                if (kbhit()) {
                    printf("Key press detected, skipping sleep.\n");
                    break;
                }
            }

            // If countdown reaches 0, sleep the system
            if (countdown == 0) {
                printf("Sleeping system...\n");
                system("pmset sleepnow");  // Command to sleep the system
            }
        }
    }

    // Close the log stream process
    fclose(log_stream);
    return 0;
}

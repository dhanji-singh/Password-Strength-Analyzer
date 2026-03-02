#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>
#include <stdlib.h>

// Function to analyze password strength
int analyzePassword(const char *pwd) {
    int len = strlen(pwd);
    int hasUpper = 0;
    int hasLower = 0;
    int hasDigit = 0;
    int hasSpecial = 0;

    // Character diversity check
    for (int i = 0; i < len; i++) {
        if (isupper(pwd[i])) {
            hasUpper = 1;
        }
        else if (islower(pwd[i])) {
            hasLower = 1;
        }
        else if (isdigit(pwd[i])) {
            hasDigit = 1;
        }
        else {
            hasSpecial = 1;
        }
    }

    // Weak pattern detection
    const char *weakPatterns[] = {"123", "password", "qwerty", "abc"};
    int numPatterns = sizeof(weakPatterns) / sizeof(weakPatterns[0]);

    for (int i = 0; i < numPatterns; i++) {
        if (strstr(pwd, weakPatterns[i])) {
            return 0; // Weak
        }
    }

    // Strength classification
    int diversity = hasUpper + hasLower + hasDigit + hasSpecial;

    if (len < 6 || diversity < 2) {
        return 0; // Weak
    }
    else if (len < 10 || diversity < 3) {
        return 1; // Moderate
    }
    else {
        return 2; // Strong
    }
}

void drawBox(int y, int x, int height, int width) {
    mvaddch(y, x, ACS_ULCORNER);
    mvaddch(y, x + width - 1, ACS_URCORNER);
    mvaddch(y + height - 1, x, ACS_LLCORNER);
    mvaddch(y + height - 1, x + width - 1, ACS_LRCORNER);

    for (int i = 1; i < width - 1; i++) {
        mvaddch(y, x + i, ACS_HLINE);
        mvaddch(y + height - 1, x + i, ACS_HLINE);
    }

    for (int i = 1; i < height - 1; i++) {
        mvaddch(y + i, x, ACS_VLINE);
        mvaddch(y + i, x + width - 1, ACS_VLINE);
    }
}

void drawStrengthBar(int y, int x, int strength) {
    mvprintw(y, x, "Strength: ");
    
    int barWidth = 20;
    int filledWidth = (strength + 1) * barWidth / 3;
    
    addch('[');
    for (int i = 0; i < barWidth; i++) {
        if (i < filledWidth) {
            if (strength == 0) attron(COLOR_PAIR(1)); // Red
            else if (strength == 1) attron(COLOR_PAIR(2)); // Yellow
            else attron(COLOR_PAIR(3)); // Green
            addch('=');
            attroff(COLOR_PAIR(1) | COLOR_PAIR(2) | COLOR_PAIR(3));
        } else {
            addch(' ');
        }
    }
    addch(']');
}

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // Initialize color pairs
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);       // Weak
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);    // Moderate
    init_pair(3, COLOR_GREEN, COLOR_BLACK);     // Strong
    init_pair(4, COLOR_CYAN, COLOR_BLACK);      // Title
    init_pair(5, COLOR_WHITE, COLOR_BLACK);     // Normal

    int maxX, maxY;
    getmaxyx(stdscr, maxY, maxX);

    char password[100] = "";
    int pos = 0;
    int ch;

    while (1) {
        clear();

        // Draw main box
        drawBox(2, 5, maxY - 4, maxX - 10);

        // Title
        attron(COLOR_PAIR(4) | A_BOLD);
        mvprintw(3, (maxX - 30) / 2, "PASSWORD STRENGTH ANALYZER");
        attroff(COLOR_PAIR(4) | A_BOLD);

        // Instructions
        mvprintw(5, 8, "Enter your password (Press ENTER to analyze, ESC to quit):");
        
        // Password input display (masked)
        mvprintw(7, 8, "Password: ");
        for (int i = 0; i < pos; i++) {
            addch('*');
        }

        // Draw password input box
        drawBox(6, 7, 3, maxX - 14);

        // Analyze password if entered
        if (pos > 0) {
            int strength = analyzePassword(password);

            // Draw strength bar
            mvprintw(10, 8, "Strength Level:");
            drawStrengthBar(11, 8, strength);

            // Draw result
            mvprintw(13, 8, "Result: ");
            if (strength == 0) {
                attron(COLOR_PAIR(1) | A_BOLD);
                printw("WEAK PASSWORD");
                attroff(COLOR_PAIR(1) | A_BOLD);
                mvprintw(15, 8, "Feedback: Add uppercase, numbers, or special characters");
            }
            else if (strength == 1) {
                attron(COLOR_PAIR(2) | A_BOLD);
                printw("MODERATE PASSWORD");
                attroff(COLOR_PAIR(2) | A_BOLD);
                mvprintw(15, 8, "Feedback: Increase length or add more character types");
            }
            else {
                attron(COLOR_PAIR(3) | A_BOLD);
                printw("STRONG PASSWORD");
                attroff(COLOR_PAIR(3) | A_BOLD);
                mvprintw(15, 8, "Feedback: Great! This is a strong password");
            }

            // Criteria check
            mvprintw(17, 8, "Criteria Check:");
            mvprintw(18, 10, "Length >= 6 chars: %s", strlen(password) >= 6 ? "[YES]" : "[NO]");
            mvprintw(19, 10, "Has uppercase: %s", 0 ? "[YES]" : "[NO]");
            mvprintw(20, 10, "Has lowercase: %s", 0 ? "[YES]" : "[NO]");
            mvprintw(21, 10, "Has digits: %s", 0 ? "[YES]" : "[NO]");
            mvprintw(22, 10, "Has special chars: %s", 0 ? "[YES]" : "[NO]");

            // Check actual criteria
            int hasUpper = 0, hasLower = 0, hasDigit = 0, hasSpecial = 0;
            for (int i = 0; i < strlen(password); i++) {
                if (isupper(password[i])) hasUpper = 1;
                else if (islower(password[i])) hasLower = 1;
                else if (isdigit(password[i])) hasDigit = 1;
                else hasSpecial = 1;
            }

            mvprintw(18, 40, "Length >= 6 chars: %s", strlen(password) >= 6 ? "[YES]" : "[NO]");
            mvprintw(19, 40, "Has uppercase: %s", hasUpper ? "[YES]" : "[NO]");
            mvprintw(20, 40, "Has lowercase: %s", hasLower ? "[YES]" : "[NO]");
            mvprintw(21, 40, "Has digits: %s", hasDigit ? "[YES]" : "[NO]");
            mvprintw(22, 40, "Has special chars: %s", hasSpecial ? "[YES]" : "[NO]");
        }

        // Footer
        mvprintw(maxY - 2, 8, "Controls: Type password | ENTER: Analyze | BACKSPACE: Delete | ESC: Quit");

        refresh();

        // Input handling
        ch = getch();

        if (ch == 27) { // ESC
            break;
        }
        else if (ch == '\n') {
            // Enter key - analysis happens on display
        }
        else if (ch == KEY_BACKSPACE || ch == 127) {
            if (pos > 0) {
                pos--;
                password[pos] = '\0';
            }
        }
        else if (ch >= 32 && ch <= 126) { // Printable characters
            if (pos < 99) {
                password[pos] = ch;
                pos++;
                password[pos] = '\0';
            }
        }
    }

    endwin();
    printf("\nPassword analysis complete. Thank you for using Password Strength Analyzer!\n");
    return 0;
}

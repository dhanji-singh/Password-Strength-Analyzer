#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>
#include <stdlib.h>

/* ============================================================================
 * PASSWORD STRENGTH ANALYZER - TUI Interface
 * ============================================================================
 * A professional terminal UI for analyzing and providing feedback on password
 * strength based on industry-standard criteria.
 */

/* Constants and Configuration */
#define MAX_PASSWORD_LENGTH     128
#define MIN_WEAK_LENGTH         6
#define MIN_MODERATE_LENGTH     10
#define WEAK_PATTERNS_COUNT     4
#define BOX_MARGIN              5
#define BOX_MIN_HEIGHT          4
#define COLOR_WEAK              1
#define COLOR_MODERATE          2
#define COLOR_STRONG            3
#define COLOR_TITLE             4
#define COLOR_NORMAL            5
#define COLOR_SUCCESS           6
#define ASCII_PRINTABLE_MIN     32
#define ASCII_PRINTABLE_MAX     126

/* Password Criteria Structure */
typedef struct {
    int length;
    int has_upper;
    int has_lower;
    int has_digit;
    int has_special;
    int diversity_count;
} PasswordCriteria;

/* Weak pattern detection array */
static const char *WEAK_PATTERNS[] = {
    "123", "password", "qwerty", "abc", 
    "111", "222", "aaa", "admin"
};

/* ============================================================================
 * Helper Functions for Password Analysis
 * ============================================================================ */

/**
 * Extract and evaluate password criteria
 * @param pwd Password string to analyze
 * @param criteria Pointer to criteria structure to populate
 */
void evaluate_password_criteria(const char *pwd, PasswordCriteria *criteria) {
    if (!pwd || !criteria) return;
    
    criteria->length = strlen(pwd);
    criteria->has_upper = 0;
    criteria->has_lower = 0;
    criteria->has_digit = 0;
    criteria->has_special = 0;
    
    for (int i = 0; i < criteria->length; i++) {
        if (isupper(pwd[i])) criteria->has_upper = 1;
        else if (islower(pwd[i])) criteria->has_lower = 1;
        else if (isdigit(pwd[i])) criteria->has_digit = 1;
        else criteria->has_special = 1;
    }
    
    criteria->diversity_count = criteria->has_upper + criteria->has_lower + 
                                criteria->has_digit + criteria->has_special;
}

/**
 * Check if password contains weak patterns
 * @param pwd Password string to check
 * @return 1 if weak pattern found, 0 otherwise
 */
int contains_weak_pattern(const char *pwd) {
    if (!pwd) return 0;
    
    for (int i = 0; i < WEAK_PATTERNS_COUNT; i++) {
        if (strstr(pwd, WEAK_PATTERNS[i]) != NULL) {
            return 1;
        }
    }
    return 0;
}

/**
 * Classify password strength (0=Weak, 1=Moderate, 2=Strong)
 * @param criteria Pointer to criteria structure
 * @return Strength level (0, 1, or 2)
 */
int classify_password_strength(const PasswordCriteria *criteria) {
    if (!criteria) return 0;
    
    /* Weak classification */
    if (criteria->length < MIN_WEAK_LENGTH || criteria->diversity_count < 2) {
        return 0;
    }
    
    /* Moderate classification */
    if (criteria->length < MIN_MODERATE_LENGTH || criteria->diversity_count < 3) {
        return 1;
    }
    
    /* Strong classification */
    return 2;
}

/**
 * Main password analysis function
 * @param pwd Password to analyze
 * @param criteria Pointer to store analysis results
 * @return Strength level (0=Weak, 1=Moderate, 2=Strong)
 */
int analyze_password(const char *pwd, PasswordCriteria *criteria) {
    if (!pwd || !criteria) return 0;
    
    /* Extract criteria */
    evaluate_password_criteria(pwd, criteria);
    
    /* Check for weak patterns */
    if (contains_weak_pattern(pwd)) {
        return 0;
    }
    
    /* Classify strength */
    return classify_password_strength(criteria);
}

/* ============================================================================
 * UI Drawing Functions
 * ============================================================================ */

/**
 * Draw a box with ASCII art
 * @param y Y coordinate
 * @param x X coordinate
 * @param height Box height
 * @param width Box width
 */
void draw_box(int y, int x, int height, int width) {
    if (height < 2 || width < 2) return;
    
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

/**
 * Draw visual strength indicator bar
 * @param y Y coordinate
 * @param x X coordinate
 * @param strength Strength level (0, 1, or 2)
 */
void draw_strength_bar(int y, int x, int strength) {
    if (strength < 0 || strength > 2) return;
    
    mvprintw(y, x, "Strength: ");
    
    int bar_width = 20;
    int filled_width = (strength + 1) * bar_width / 3;
    int color_pair = (strength == 0) ? COLOR_WEAK : 
                     (strength == 1) ? COLOR_MODERATE : COLOR_STRONG;
    
    addch('[');
    for (int i = 0; i < bar_width; i++) {
        if (i < filled_width) {
            attron(COLOR_PAIR(color_pair));
            addch('=');
            attroff(COLOR_PAIR(color_pair));
        } else {
            addch(' ');
        }
    }
    addch(']');
}

/**
 * Display password strength result with feedback
 * @param y Y coordinate
 * @param x X coordinate
 * @param strength Strength level
 */
void display_strength_result(int y, int x, int strength) {
    mvprintw(y, x, "Result: ");
    
    switch (strength) {
        case 0:
            attron(COLOR_PAIR(COLOR_WEAK) | A_BOLD);
            printw("WEAK PASSWORD");
            attroff(COLOR_PAIR(COLOR_WEAK) | A_BOLD);
            mvprintw(y + 2, x, "Feedback: Use uppercase, lowercase, digits, and symbols");
            break;
        case 1:
            attron(COLOR_PAIR(COLOR_MODERATE) | A_BOLD);
            printw("MODERATE PASSWORD");
            attroff(COLOR_PAIR(COLOR_MODERATE) | A_BOLD);
            mvprintw(y + 2, x, "Feedback: Increase length to 10+ chars or add special chars");
            break;
        case 2:
            attron(COLOR_PAIR(COLOR_STRONG) | A_BOLD);
            printw("STRONG PASSWORD");
            attroff(COLOR_PAIR(COLOR_STRONG) | A_BOLD);
            mvprintw(y + 2, x, "Feedback: Excellent choice! Strong password detected");
            break;
    }
}

/**
 * Display password criteria checklist with actual values
 * @param y Y coordinate
 * @param x X coordinate
 * @param criteria Password criteria structure
 */
void display_criteria_check(int y, int x, const PasswordCriteria *criteria) {
    if (!criteria) return;
    
    mvprintw(y, x, "Criteria Check:");
    mvprintw(y + 1, x + 2, "Length >= 6 chars:    %%s", 
             criteria->length >= MIN_WEAK_LENGTH ? "✓ [YES]" : "✗ [NO] ");
    mvprintw(y + 2, x + 2, "Has uppercase (A-Z):  %%s", 
             criteria->has_upper ? "✓ [YES]" : "✗ [NO] ");
    mvprintw(y + 3, x + 2, "Has lowercase (a-z):  %%s", 
             criteria->has_lower ? "✓ [YES]" : "✗ [NO] ");
    mvprintw(y + 4, x + 2, "Has digits (0-9):     %%s", 
             criteria->has_digit ? "✓ [YES]" : "✗ [NO] ");
    mvprintw(y + 5, x + 2, "Has special chars:    %%s", 
             criteria->has_special ? "✓ [YES]" : "✗ [NO] ");
    mvprintw(y + 6, x + 2, "Character diversity:  %%d/4 types", 
             criteria->diversity_count);
}

/* ============================================================================
 * Main Program
 * ============================================================================ */

int main(void) {
    /* Initialize ncurses */
    initscr();
    
    /* Verify color support */
    if (!has_colors()) {
        endwin();
        fprintf(stderr, "Error: Terminal does not support colors\n");
        return 1;
    }
    
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    
    /* Initialize color pairs */
    start_color();
    init_pair(COLOR_WEAK, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_MODERATE, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_STRONG, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_TITLE, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_NORMAL, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_SUCCESS, COLOR_GREEN, COLOR_BLACK);
    
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    
    char password[MAX_PASSWORD_LENGTH] = "";
    int pos = 0;
    int ch;
    
    while (1) {
        clear();
        
        /* Draw main box */
        if (max_y > BOX_MIN_HEIGHT && max_x > 20) {
            draw_box(2, BOX_MARGIN, max_y - 4, max_x - 10);
        }
        
        /* Title */
        attron(COLOR_PAIR(COLOR_TITLE) | A_BOLD);
        mvprintw(3, (max_x - 30) / 2, "PASSWORD STRENGTH ANALYZER");
        attroff(COLOR_PAIR(COLOR_TITLE) | A_BOLD);
        
        /* Instructions */
        mvprintw(5, BOX_MARGIN + 3, "Enter your password to analyze its strength:");
        
        /* Password input display (masked) */
        mvprintw(7, BOX_MARGIN + 3, "Password: [");
        for (int i = 0; i < pos; i++) {
            addch('*');
        }
        mvprintw(7, BOX_MARGIN + 3 + 12 + pos, "]");
        mvprintw(8, BOX_MARGIN + 3, "Length: %%d/%%d characters", pos, MAX_PASSWORD_LENGTH - 1);
        
        /* Analyze password if entered */
        if (pos > 0) {
            PasswordCriteria criteria;
            int strength = analyze_password(password, &criteria);
            
            /* Display strength bar */
            mvprintw(10, BOX_MARGIN + 3, "Strength Level:");
            draw_strength_bar(11, BOX_MARGIN + 3, strength);
            
            /* Display result with feedback */
            display_strength_result(13, BOX_MARGIN + 3, strength);
            
            /* Display criteria checklist */
            display_criteria_check(17, BOX_MARGIN + 3, &criteria);
        }
        
        /* Footer with instructions */
        mvprintw(max_y - 2, BOX_MARGIN + 3, 
                 "Controls: Type | BACKSPACE: Delete | ENTER: Clear | ESC: Quit");
        
        refresh();
        
        /* Input handling */
        ch = getch();
        
        if (ch == 27) { /* ESC */
            break;
        }
        else if (ch == '\n') { /* ENTER - Clear password */
            pos = 0;
            password[0] = '\0';
        }
        else if (ch == KEY_BACKSPACE || ch == 127) { /* BACKSPACE */
            if (pos > 0) {
                pos--;
                password[pos] = '\0';
            }
        }
        else if (ch >= ASCII_PRINTABLE_MIN && ch <= ASCII_PRINTABLE_MAX) { 
            /* Printable characters */
            if (pos < MAX_PASSWORD_LENGTH - 1) {
                password[pos] = ch;
                pos++;
                password[pos] = '\0';
            }
        }
    }
    
    endwin();
    printf("\n✓ Password analysis complete. Thank you for using Password Strength Analyzer!\n");
    return 0;
}
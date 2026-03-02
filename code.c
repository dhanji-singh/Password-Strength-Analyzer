#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

int main() {
    char pwd[100];

    printf("Enter password: ");
    scanf("%99s", pwd); // safer input

    int result = analyzePassword(pwd);

    if (result == 0) {
        printf("Weak Password\n");
    }
    else if (result == 1) {
        printf("Moderate Password\n");
    }
    else {
        printf("Strong Password\n");
    }

    return 0;
}
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_DEGREE 2
#define SIZE (MAX_DEGREE + 1)
#define PRODUCT_SIZE (2 * MAX_DEGREE + 1)
#define MAX_COEFF_DIGITS 6
#define LINE_LEN 100

enum Choice { ADD = 1, SUBTRACT, DIVIDE, MULTIPLY, FACTORS, EXIT };

int readLine(char *line) {
    if (fgets(line, LINE_LEN, stdin) == NULL) return 0;

    char *newline = strchr(line, '\n');
    if (newline != NULL) {
        *newline = '\0';
        return 1;
    }
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
    return -1;
}

int parsePoly(const char *text, double coeff[SIZE]) {
    for (int k = 0; k < SIZE; k++) coeff[k] = 0;
    int i = 0, terms = 0;

    while (1) {
        while (isspace((unsigned char)text[i])) i++;
        if (text[i] == '\0') break;

        int sign = 1;
        if (text[i] == '+' || text[i] == '-') {
            if (text[i] == '-') sign = -1;
            i++;
            while (isspace((unsigned char)text[i])) i++;
        } else if (terms > 0) {
            return 0;
        }

        int number = 0, digits = 0;
        while (isdigit((unsigned char)text[i])) {
            number = number * 10 + (text[i] - '0');
            digits++;
            i++;
            if (digits > MAX_COEFF_DIGITS) return 0;
        }

        int power = 0;
        if (text[i] == 'x') {
            if (digits == 0) number = 1;
            power = 1;
            i++;
            if (text[i] == '^') {
                i++;
                if (!isdigit((unsigned char)text[i])) return 0;
                power = text[i] - '0';
                i++;
            }
        } else if (digits == 0) {
            return 0;
        }

        if (power > MAX_DEGREE) return 0;
        coeff[power] += sign * number;
        terms++;
    }
    return terms > 0;
}

int degree(const double coeff[], int size) {
    for (int k = size - 1; k > 0; k--) {
        if (coeff[k] != 0) return k;
    }
    return 0;
}

void printPoly(const double coeff[], int size) {
    int first = 1;
    for (int k = size - 1; k >= 0; k--) {
        if (coeff[k] == 0) continue;

        if (first) {
            if (coeff[k] < 0) printf("-");
        } else {
            printf(coeff[k] < 0 ? " - " : " + ");
        }

        double value = fabs(coeff[k]);
        if (k == 0 || value != 1) printf("%g", value);
        if (k >= 1) printf("x");
        if (k >= 2) printf("^%d", k);
        first = 0;
    }
    if (first) printf("0");
}

void addPoly(const double a[SIZE], const double b[SIZE], double result[SIZE]) {
    for (int k = 0; k < SIZE; k++) result[k] = a[k] + b[k];
}

void subtractPoly(const double a[SIZE], const double b[SIZE], double result[SIZE]) {
    for (int k = 0; k < SIZE; k++) result[k] = a[k] - b[k];
}

void multiplyPoly(const double a[SIZE], const double b[SIZE], double result[PRODUCT_SIZE]) {
    for (int k = 0; k < PRODUCT_SIZE; k++) result[k] = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            result[i + j] += a[i] * b[j];
        }
    }
}

void printRootFactor(double root) {
    printf("(x %c %g)", root < 0 ? '+' : '-', fabs(root));
}

void printFactors(const double coeff[SIZE]) {
    int deg = degree(coeff, SIZE);

    if (coeff[deg] == 0) {
        printf("Zero polynomial has no factors.\n");
    } else if (deg == 0) {
        printf("Constant, nothing to factor.\n");
    } else if (deg == 1) {
        printf("Root: x = %g\n", -coeff[0] / coeff[1]);
    } else {
        double a = coeff[2], b = coeff[1], c = coeff[0];
        double discriminant = b * b - 4 * a * c;

        if (discriminant >= 0) {
            double root1 = (-b + sqrt(discriminant)) / (2 * a);
            double root2 = (-b - sqrt(discriminant)) / (2 * a);
            printf("Factors: ");
            if (a != 1) printf("%g", a);
            printRootFactor(root1);
            printRootFactor(root2);
            printf("\n");
        } else {
            double real = -b / (2 * a) + 0.0;
            double imaginary = sqrt(-discriminant) / (2 * fabs(a));
            printf("No real factors. Complex roots: x = %g + %gi, x = %g - %gi\n",
                   real, imaginary, real, imaginary);
        }
    }
}

void runChoice(int choice, const double a[SIZE], const double b[SIZE]) {
    double result[PRODUCT_SIZE];

    if (choice == ADD) {
        addPoly(a, b, result);
        printf("Sum: ");
        printPoly(result, SIZE);
        printf("\n");
    } else if (choice == SUBTRACT) {
        subtractPoly(a, b, result);
        printf("Difference: ");
        printPoly(result, SIZE);
        printf("\n");
    } else if (choice == DIVIDE) {
        // Division is not supported: one polynomial cannot be divided by another
        // polynomial in general (the result is not always a polynomial).
        // It only works for special cases, like dividing by (x + 1).
        printf("Division of two polynomials is not supported.\n");
    } else if (choice == MULTIPLY) {
        multiplyPoly(a, b, result);
        printf("Product: ");
        printPoly(result, PRODUCT_SIZE);
        printf("\n");
    } else if (choice == FACTORS) {
        printf("First polynomial:\n");
        printFactors(a);
        printf("Second polynomial:\n");
        printFactors(b);
    }
}

int readPoly(const char *prompt, double coeff[SIZE]) {
    char line[LINE_LEN];
    while (1) {
        printf("%s", prompt);
        int status = readLine(line);
        if (status == 0) return 0;
        if (status == 1 && parsePoly(line, coeff)) return 1;
        printf("Error: enter a quadratic like 5x^2 + 10x - 6\n");
    }
}

int main() {
    double first[SIZE], second[SIZE];

    if (!readPoly("Enter first polynomial: ", first)) return 1;
    if (!readPoly("Enter second polynomial: ", second)) return 1;

    while (1) {
        printf("\n1. Addition\n2. Subtraction\n3. Division\n4. Multiplication\n5. Finding factors\n6. Exit\nChoice: ");

        char line[LINE_LEN], extra;
        int choice;
        if (readLine(line) == 0) break;
        if (sscanf(line, "%d %c", &choice, &extra) != 1 || choice < ADD || choice > EXIT) {
            printf("Invalid choice. Enter a number from 1 to 6.\n");
            continue;
        }
        if (choice == EXIT) break;

        runChoice(choice, first, second);
    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int isVowel(char c) {
    c = (char)tolower((unsigned char)c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

char* encodeName(char *input) {
    char *out = (char *)malloc(strlen(input) + 1);
    char *w = out;
    for (char *p = input; *p; p++) {
        if (w > out) {
            char prev = *(w - 1);
            if (tolower((unsigned char)*p) == tolower((unsigned char)prev)) continue; /* Rule 1 */
            if (isVowel(*p) && isVowel(prev)) continue;                               /* Rule 2 */
        }
        *w++ = (char)tolower((unsigned char)*p);                                      /* Rule 3 */
    }
    *w = '\0';
    return out;
}

int** allocateMatrix(int n) {
    int **m = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) m[i] = (int *)malloc(5 * sizeof(int));
    return m;
}

int computeTotal(int *row) {
    int s = 0;
    for (int j = 0; j < 5; j++) s += *(row + j);
    return s;
}

float computeAvg(int *row) {
    return computeTotal(row) / 5.0f;
}

int highestSubject(int *row) {
    int best = 0;
    for (int j = 1; j < 5; j++)
        if (*(row + j) > *(row + best)) best = j;
    return best;
}

void printResult(char **names, int **marks, int n) {
    for (int i = 0; i < n; i++)
        printf("%s %d %.2f %d\n", names[i], computeTotal(marks[i]),
               computeAvg(marks[i]), highestSubject(marks[i]));
}

int main(void) {
    int n;
    if (scanf("%d", &n) != 1 || n <= 0) return 0;
    char **names = (char **)malloc(n * sizeof(char *));
    int **marks = allocateMatrix(n);
    char buf[1024];
    for (int i = 0; i < n; i++) {
        scanf("%1023s", buf);
        names[i] = encodeName(buf);
        for (int j = 0; j < 5; j++) scanf("%d", &marks[i][j]);
    }
    printResult(names, marks, n);
    for (int i = 0; i < n; i++) { free(names[i]); free(marks[i]); }
    free(names); free(marks);
    return 0;
}

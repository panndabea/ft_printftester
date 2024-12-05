#include <stdio.h>
#include <string.h>
#include <unistd.h> // Für POSIX-Funktionen

// Deklaration von ft_printf
extern int ft_printf(const char *, ...);

void test_printf() {
    char buffer1[1024], buffer2[1024];
    int ret1, ret2;

    const char *test_cases[] = {
        "Hello, World!",
        "Integer: %d",
        "String: %s",
        "Character: %c",
        "Hexadecimal: %x",
        "Pointer: %p",
        "Percent: %%",
        "Multiple: %d, %s, %c",
        NULL
    };

    struct {
        int i;
        const char *s;
        char c;
        unsigned int u;
        void *p;
    } args = {42, "42 School", 'A', 0x42, &args};

    for (int i = 0; test_cases[i] != NULL; i++) {
        // Standard printf
        memset(buffer1, 0, sizeof(buffer1));
        ret1 = snprintf(buffer1, sizeof(buffer1), test_cases[i],
                        args.i, args.s, args.c, args.u, args.p);

        // ft_printf
        memset(buffer2, 0, sizeof(buffer2));
        FILE *fp = fopen("ft_printf_output.txt", "w+");
        if (!fp) {
            perror("Failed to open temp file");
            return;
        }
        int old_stdout = dup(1);
        dup2(fileno(fp), 1);
        ret2 = ft_printf(test_cases[i], args.i, args.s, args.c, args.u, args.p);
        fflush(stdout);
        dup2(old_stdout, 1);
        close(old_stdout);
        rewind(fp);
        fread(buffer2, 1, sizeof(buffer2) - 1, fp);
        fclose(fp);

        // Ergebnis ausgeben
        printf("Test %d:\n", i + 1);
        printf("  Format: '%s'\n", test_cases[i]);
        printf("  printf:    [%s] (return: %d)\n", buffer1, ret1);
        printf("  ft_printf: [%s] (return: %d)\n", buffer2, ret2);

        if (strcmp(buffer1, buffer2) != 0 || ret1 != ret2) {
            printf("  \033[31mFAIL\033[0m\n");
        } else {
            printf("  \033[32mPASS\033[0m\n");
        }
    }
}

int main() {
    test_printf();
    return 0;
}

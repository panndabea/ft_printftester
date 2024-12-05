#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>

// Deklaration deiner ft_printf Funktion
typedef int (*ft_printf_t)(const char *, ...);

// Funktion zum Vergleich von printf und ft_printf
void test_printf(ft_printf_t ft_printf) {
    char buffer1[1024], buffer2[1024];
    int ret1, ret2;

    // Testfälle
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

    // Argumente für die Testfälle
    struct {
        int i;
        const char *s;
        char c;
        unsigned int u;
        void *p;
    } args = {42, "42 School", 'A', 0x42, &args};

    for (int i = 0; test_cases[i] != NULL; i++) {
        // Test 1: Standard printf in buffer1
        memset(buffer1, 0, sizeof(buffer1));
        ret1 = snprintf(buffer1, sizeof(buffer1), test_cases[i],
                        args.i, args.s, args.c, args.u, args.p);

        // Test 2: ft_printf in buffer2
        memset(buffer2, 0, sizeof(buffer2));
        int pipe_fd[2];
        pipe(pipe_fd);
        int stdout_fd = dup(STDOUT_FILENO);
        dup2(pipe_fd[1], STDOUT_FILENO);

        ret2 = ft_printf(test_cases[i],
                         args.i, args.s, args.c, args.u, args.p);
        fflush(stdout);
        read(pipe_fd[0], buffer2, sizeof(buffer2));
        dup2(stdout_fd, STDOUT_FILENO);

        // Ergebnis ausgeben
        printf("Test %d:\n", i + 1);
        printf("  Format: '%s'\n", test_cases[i]);
        printf("  printf: [%s] (return: %d)\n", buffer1, ret1);
        printf("  ft_printf: [%s] (return: %d)\n", buffer2, ret2);

        // Vergleich
        if (strcmp(buffer1, buffer2) != 0 || ret1 != ret2) {
            printf("  \033[31mFAIL\033[0m\n");
        } else {
            printf("  \033[32mPASS\033[0m\n");
        }
        close(pipe_fd[0]);
        close(pipe_fd[1]);
    }
}

int main() {
    // Lade die ft_printf Implementierung
    void *handle = dlopen("./libftprintf.a", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Failed to load ft_printf: %s\n", dlerror());
        return 1;
    }

    ft_printf_t ft_printf = dlsym(handle, "ft_printf");
    if (!ft_printf) {
        fprintf(stderr, "Failed to locate ft_printf: %s\n", dlerror());
        dlclose(handle);
        return 1;
    }

    // Tests durchführen
    test_printf(ft_printf);

    // Bibliothek schließen
    dlclose(handle);
    return 0;
}

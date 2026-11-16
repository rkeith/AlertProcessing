#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *data;
    size_t size;
} Document;

static int load_document(const char *path, Document *document)
{
    FILE *file = fopen(path, "rb");
    long length;
    char *data;
    size_t size;

    if (file == NULL) {
        fprintf(stderr, "Cannot open '%s': %s\n", path, strerror(errno));
        return 0;
    }
    if (fseek(file, 0, SEEK_END) != 0 || (length = ftell(file)) < 0 ||
        fseek(file, 0, SEEK_SET) != 0) {
        fprintf(stderr, "Cannot determine file size for '%s'.\n", path);
        fclose(file);
        return 0;
    }
    size = (size_t)length;
    if (size == (size_t)-1) {
        fprintf(stderr, "File '%s' is too large.\n", path);
        fclose(file);
        return 0;
    }
    data = malloc(size + 1);
    if (data == NULL) {
        fprintf(stderr, "Cannot allocate memory for '%s'.\n", path);
        fclose(file);
        return 0;
    }
    if (fread(data, 1, size, file) != size || ferror(file)) {
        fprintf(stderr, "Cannot read '%s'.\n", path);
        free(data);
        fclose(file);
        return 0;
    }
    if (fclose(file) != 0) {
        fprintf(stderr, "Cannot close '%s': %s\n", path, strerror(errno));
        free(data);
        return 0;
    }
    data[size] = '\0';
    free(document->data);
    document->data = data;
    document->size = size;
    return 1;
}

static void print_help(void)
{
    puts("Commands:\n"
         "  help          Show available commands\n"
         "  info          Show the loaded file and its size\n"
         "  reload        Reload the file from disk\n"
         "  quit / exit   Close the application");
}

int main(int argc, char **argv)
{
    const char *path = argc == 2 ? argv[1] : "aaJsonDemo.json";
    Document document = {NULL, 0};
    char line[256];
    int result = EXIT_SUCCESS;

    if (argc > 2) {
        fprintf(stderr, "Usage: %s [json-file]\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (!load_document(path, &document)) {
        return EXIT_FAILURE;
    }

    printf("AlertProcessing\nLoaded '%s' (%zu bytes).\n", path, document.size);
    print_help();
    for (;;) {
        char *command;
        char *end;

        printf("alert> ");
        fflush(stdout);
        if (fgets(line, sizeof(line), stdin) == NULL) {
            if (ferror(stdin)) {
                fputs("Cannot read terminal input.\n", stderr);
                result = EXIT_FAILURE;
            }
            break;
        }
        if (strchr(line, '\n') == NULL) {
            int ch = getchar();
            if (ch != EOF && ch != '\n') {
                do {
                    ch = getchar();
                } while (ch != EOF && ch != '\n');
                fputs("Command is too long (maximum 255 characters).\n", stderr);
                continue;
            }
        }
        command = line;
        while (isspace((unsigned char)*command)) {
            ++command;
        }
        end = command + strlen(command);
        while (end > command && isspace((unsigned char)end[-1])) {
            --end;
        }
        *end = '\0';

        if (*command == '\0') {
            continue;
        }
        if (strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0) {
            break;
        }
        if (strcmp(command, "help") == 0) {
            print_help();
        } else if (strcmp(command, "info") == 0) {
            printf("File: %s\nBytes loaded: %zu\n", path, document.size);
        } else if (strcmp(command, "reload") == 0) {
            if (load_document(path, &document)) {
                printf("Reloaded '%s' (%zu bytes).\n", path, document.size);
            } else {
                fputs("Reload failed; the previously loaded data is unchanged.\n", stderr);
            }
        } else {
            fprintf(stderr, "Unknown command '%s'. Type 'help' for commands.\n", command);
        }
    }

    free(document.data);
    puts("\nGoodbye.");
    return result;
}

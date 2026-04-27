#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

#define DICT_DB_PATH "./data/comppiled/fra-eng.db"

#define ORIGINAL_LEN 32
#define TRANSLATED_LEN 32

typedef struct
{
    char original[ORIGINAL_LEN];
    char translated[TRANSLATED_LEN];
} __attribute__((packed)) dict_entry_t;

static inline char* get_next_arg(int* argc, char*** argv)
{
    if ( *argc == 0 )
    {
        fprintf(stderr, "missing argument.\n");
        exit(EXIT_FAILURE);
    }

    char* arg = **argv;
    --(*argc);
    ++(*argv);

    return arg;
}

int main(int argc, char** argv)
{
    get_next_arg(&argc, &argv);
    const char* word_prefix = get_next_arg(&argc, &argv);

    int dict_fd = open(DICT_DB_PATH, O_RDONLY);
    struct stat dict_stat;
    fstat(dict_fd, &dict_stat);
    
    void* data = mmap(NULL, dict_stat.st_size, PROT_READ, MAP_PRIVATE, dict_fd, 0);


    return 0;
}

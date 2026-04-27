#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define DICT_DB_PATH "./data/compiled/fra-eng.db"

#define ORIGINAL_LEN 32
#define TRANSLATED_LEN 96

typedef struct
{
    char original[ORIGINAL_LEN];
    char translated[TRANSLATED_LEN];
} __attribute__((packed)) dict_entry_t;

static inline char* get_next_arg(int* argc, char*** argv);
static inline int prefix_compare(const void* _keyword, const void* _dict_entry);

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

static inline int prefix_compare(const void* _keyword, const void* _dict_entry)
{
    const char* keyword = _keyword;
    const char* dict_entry = _dict_entry;

    return strncmp(keyword, dict_entry, strlen(keyword));
}

int main(int argc, char** argv)
{
    get_next_arg(&argc, &argv);
    const char* word_prefix = get_next_arg(&argc, &argv);

    int dict_fd = open(DICT_DB_PATH, O_RDONLY);

    if ( dict_fd == -1 )
    {
        printf("error: failed to open the dictionary file\n");
        exit(EXIT_FAILURE);
    }

    struct stat dict_stat;
    fstat(dict_fd, &dict_stat);
   
    size_t entries_count = dict_stat.st_size / sizeof(dict_entry_t);
    dict_entry_t* data = mmap(NULL, dict_stat.st_size, PROT_READ, MAP_PRIVATE, dict_fd, 0);

    dict_entry_t* result = bsearch(word_prefix, data, entries_count, sizeof(dict_entry_t), prefix_compare);

    if ( result == NULL )
    {
        printf("no results\n");
    }

    else
    {
        printf("%s: %s\n", result->original, result->translated);
    }


    return 0;
}

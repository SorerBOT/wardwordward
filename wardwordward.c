#include <stdio.h>
#include <stdint.h>

#define ANSI_ESCAPE "\033"
#define BUFF_SIZE 256

static inline void draw_box(size_t x, size_t y, size_t width, size_t height);

static inline void draw_box(size_t x, size_t y, size_t width, size_t height)
{
    printf(ANSI_ESCAPE "[%zu;%zuH┌", y, x);
    for (int i = 0; i < width - 2; i++) printf("─");
    printf("┐");

    for (int i = 1; i < height - 1; i++) {
        printf(ANSI_ESCAPE "[%zu;%zuH│", y + i, x);
        printf(ANSI_ESCAPE "[%zu;%zuH│", y + i, x + width - 1);
    }

    printf(ANSI_ESCAPE "[%zu;%zuH└", y + height - 1, x);
    for (int i = 0; i < width - 2; i++) printf("─");
    printf("┘");
    
    fflush(stdout);
}

char buff[BUFF_SIZE];

int main()
{
    printf(ANSI_ESCAPE "[2J");
    draw_box(10, 5, 100, 20);
    printf(ANSI_ESCAPE "[6;12HType here: "); 

    scanf("%s", buff);

    return 0;
}

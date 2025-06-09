//
// Created by Lenovo on 6/9/2025.
//

#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

extern char _end;  // Defined by the linker script
static char *heap_end;

void *_sbrk(ptrdiff_t incr) {
    if (heap_end == 0) {
        heap_end = &_end;
    }

    char *prev_heap_end = heap_end;
    heap_end += incr;

    return (void *)prev_heap_end;
}

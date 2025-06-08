#include <sys/types.h>
#include <errno.h>

// Minimal _sbrk stub to satisfy linker if malloc-like functions are not used
caddr_t _sbrk(int incr) {
    errno = ENOMEM;
    return (caddr_t)-1;
}

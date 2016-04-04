#include <stddef.h>

long asmb(long a, long b[], long c[], size_t n) {
    size_t i;
    for (i=0; i<n; i++) {
        a = (a*b[i])/c[i];
    }
    return a;
}

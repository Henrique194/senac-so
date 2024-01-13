#include <stdlib.h>
#include <stdio.h>
#include <sys/utsname.h>

typedef struct filelump_struct
{
    int		filepos;
    int		size;
    char	name[8];

} filelump_t;

int hi;

int main() {
    filelump_t tmp;
    tmp.size = 12;

    return EXIT_SUCCESS;
}

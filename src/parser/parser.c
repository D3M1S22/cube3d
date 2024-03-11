#include "../cud3d.h"

void    parse_file(char *file)
{
    int fd;

    fd = open(file, O_RDONLY, 0);
    if (!fd)
        error("error opening file");
}
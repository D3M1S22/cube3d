#include "./cud3d.h"

void    error(char *msg)
{
    printf("%s\n", msg);
    exit(0);
}

int	main(int argc, char **argv)
{
    if (argc != 2)
        error("wrong arguments");
    check_name(argv[1]);
}
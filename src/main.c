#include "./cud3d.h"

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("./test.txt", O_RDONLY, 0);
	if (fd == -1)
	{
		perror("Error opening file");
		return (1);
	}

	line = get_next_line(fd);
	while (line)
	{
		printf("%s\n", line);
		free(line);
		line = get_next_line(fd);
	}

	close(fd);

	return (0);
}
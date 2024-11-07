#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 4096

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		fprintf (stderr, "please use one string as an agument");
		return (1);
	}
	
	char	*search_char = av[1];
	size_t	char_len = strlen(search_char);
	if (char_len == 0)
	{
		fprintf (stderr, "please, search sting shouldn't be empty\n");
		return (1);
	}
	char	*buffer = (char *)malloc(BUFFER_SIZE);
	if (buffer == NULL)
	{
		perror ("error: malloc failed\n");
		return (1);
	}
	ssize_t	bytes_read;
	ssize_t i;
	while ((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0)
	{
		i = 0;
		while (i < bytes_read)
		{
			char	*pos = memmem (buffer + i, bytes_read - i, search_char, char_len);
			if (pos != NULL)
			{
				if (write (STDOUT_FILENO, buffer + i, pos - (buffer + i)) == -1)
				{
					perror ("error: write failed\n");
					free (buffer);
					return (1);
				}
				size_t	j = 0;
				while (j < char_len)
				{
					if (write (STDOUT_FILENO, "*", 1) == -1)
					{
						perror ("error: write failed\n");
						free (buffer);
						return (1);
					}
					j++;
				}
				i = pos - buffer + char_len;
			}
			else
			{
				if (write (STDOUT_FILENO, buffer + i, bytes_read - i) == -1)
				{
					perror ("error: write failed");
					free (buffer);
					return (1);
				}
				break;
			}
		}
	}
	if (bytes_read == -1)
	{
		perror ("error: read failed");
		free (buffer);
		return (1);
	}
	free (buffer);
	return (0);
}

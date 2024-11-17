#include <fcntl.h>      // for open
#include <stdio.h>      // for printf
# include "get_next_line.h"

char	*ft_strchr(char *s, int c)
{
	int i = 0;
		while (s[i])
		{
			if (s[i] == c)
				return s + i;
			i++;
		}
		return NULL;
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	while (n--)
		((char*)dest)[n] = ((char*)src)[n];
	return dest;
}

size_t	ft_strlen(char *s)
{
	size_t ret = 0;
	if  (!s)
		return 0;
	while (*s++)
	{
		ret++;
	}
	return (ret);
}

int	str_append_mem(char **s1, char *s2, size_t size2)
{
	size_t size1 = ft_strlen(*s1);
	char *tmp = malloc(size2 + size1 + 1);
	if (!tmp)
		return 0;
	ft_memcpy (tmp, *s1, size1);
	ft_memcpy (tmp + size1, s2, size2);
	tmp[size1 + size2] = '\0';
	free(*s1);
	*s1 = tmp;
	return 1;
}

int	str_append_str(char **s1, char *s2)
{
	return str_append_mem(s1, s2, ft_strlen(s2));
}

void	*ft_memmove (void *dest, const void *src, size_t n)
{
	if (dest == src)
		return dest;
	if (dest > src)
		return ft_memcpy(dest, src, n);
	else
	{
		char	*d = (char *)dest;
		const char	*s = (const char *)src;
		size_t	i;
		i = n;
		while (i > 0)
		{
			d[i - 1] = s[i - 1];
			i--;
		}
	}
	
	return dest;
}

char	*get_next_line(int fd)
{
	static char b[BUFFER_SIZE + 1] = "";
	char *ret = NULL;
	char *tmp;
	
	while (!(tmp  = ft_strchr(b, '\n'))) {
		if (!str_append_str(&ret, b))
			return NULL;
		int read_ret = read(fd, b, BUFFER_SIZE);
		if (read_ret == -1)
		{
			free (ret);
			return NULL;
		}
		if (read_ret == 0)
		{
			if  (b[0] != '\0')
			{
				if (!str_append_str(&ret, b))
				{
					free (ret);
					return NULL;
				}
				b[0] = '\0';
				return ret;
			}
			free (ret);
			return NULL;
		}
		b[read_ret] = '\0';
	}
	size_t	len_to_cpy = tmp - b + 1;
	if (!str_append_mem(&ret, b, len_to_cpy))
	{
		free(ret);
		return NULL;
	}
	ft_memmove(b, b + len_to_cpy, ft_strlen(b + len_to_cpy) + 1);
	if (b[0] == '\0')
		b[0] = '\0';
	return ret;
}

int	main(void)
{
	int fd = open("test.txt", O_RDONLY);  // Open file in read-only mode
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    char *line;
    while ((line = get_next_line(fd)) != NULL) {
        printf("%s", line);  // Print each line returned by get_next_line
        free(line);          // Free the line after printing
    }

    close(fd);  // Close the file descriptor
    return 0;
}
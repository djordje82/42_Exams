/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 17:57:40 by mestefan          #+#    #+#             */
/*   Updated: 2024/11/20 20:48:53 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define BUF_SIZE 4096

void *cus_memmem(const void *haystack, size_t haystack_len, const void *needle, size_t needle_len)
{
    if (needle_len == 0)
        return (void *)haystack;
	
    if (haystack_len < needle_len)
        return NULL;

    const unsigned char *h = haystack;
    const unsigned char *n = needle;
    size_t limit = haystack_len - needle_len + 1;

    size_t	i = 0;
    while (i < limit)
    {
        if (h[i] == n[0] && !memcmp(h + i, n, needle_len))
            return (void *)(h + i);
	++i;
    }
    return NULL;
}

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		fprintf (stderr, "please, put down only one string\n");
		return (1);
	}
	char	*str_search = av[1];
	size_t	search_len = strlen(str_search);
	if (search_len == 0)
	{
		fprintf (stderr, "please, search string shouldn't be empty\n");
		return (1);
	}
	char	*buffer = (char *)malloc(BUF_SIZE);
	if (buffer == NULL)
	{
		perror ("error: malloc failed\n");
		return (1);
	}
	ssize_t	bytes_read;
	ssize_t	i;
	while ((bytes_read = read (STDIN_FILENO, buffer, BUF_SIZE)) > 0)
	{
		i = 0;
		while (i < bytes_read)
		{
			char	*pos = cus_memmem (buffer + i, bytes_read - i, str_search, search_len);
			if (pos != NULL)
			{
				if (write (STDOUT_FILENO, buffer + i, pos - (buffer + i)) == -1)
				{
					perror ("error: write failed\n");
					free (buffer);
					return (1);
				}
				size_t	j = 0;
				while (j < search_len)
				{
					if (write (STDOUT_FILENO, "*", 1) == -1)
					{
						perror ("error: write failed\n");
						free (buffer);
						return (1);
					}
					j++;
				}
				i = pos - buffer + search_len;
			}
			else
			{
				if (write (STDOUT_FILENO, buffer + i, bytes_read - i) == -1)
				{
					perror ("error: write failed\n");
					free (buffer);
					return (1);
				}
				break ;
			}
		}
	}
	if (bytes_read == -1)
	{
		perror ("error: read failed\n");
		free (buffer);
		return (1);
	}
	free (buffer);
	return (0);
}


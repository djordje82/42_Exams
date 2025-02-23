#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int	ft_strlen(char *s)
{
	int	len;
	
	len = 0;
	while (*s++)
		len++;
	return (len);
}

void	ft_swap(char *a, char *b)
{
	char	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	sort(char *s)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	len = ft_strlen(s);
	while (i < len)
	{
		j = 0;
		while (j < len - i - 1)
		{
			if (s[j] > s[j + 1])
				ft_swap(&s[j], &s[j + 1]);
			j++;
		}
		i++;
	}	
}

void	premute(char *s, int left, int right)
{
	int	i;

	if (left == right)
	{
		puts(s);
		return ;
	}
	i = left;
	bool visit[256] = {false};
	while (i <= right)
	{
		if (visit[(unsigned char)s[i]])
			continue;
		visit[(unsigned char)s[i]] = true;
		ft_swap(&s[left], &s[i]);
		premute(s, left + 1, right);
		ft_swap(&s[left], &s[i]);
		i++;
	}
}

int	main(int ac, char **av)
{
	char	*s;
	
	if (ac != 2)
	{
		puts("I need a string\n");
		return (1);
	}
	s = av[1];
	sort(s);
	premute(s, 0, ft_strlen(s) - 1);
	return (0);
}

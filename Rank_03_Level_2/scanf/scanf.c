#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>

void	skip_input_space()
{
	char	c;
	while ((c = fgetc(stdin)) != EOF)
	{
		if (!isspace(c))
		{
			ungetc(c, stdin);
			break;
		}
	}
}

int	scan_char(va_list ap)
{
	char	*ch = va_arg(ap, char *);
	int		c = fgetc(stdin);
	if (c == EOF)
		return (-1);
	*ch = (char)c;
	return (1);
}

int	scan_int(va_list ap)
{
	int	*num = va_arg(ap, int *);
	int	c;
	int	sign = 1;
	int	result = 0;

	skip_input_space();
	c = fgetc(stdin);
	if (c == '-')
	{
		sign = -1;
		c = fgetc(stdin);
	}
	else if (c == '+')
		c = fgetc(stdin);
	if (!isdigit(c))
	{
		ungetc(c, stdin);
		return (-1);
	}
	while (isdigit(c))
	{
		result = result * 10 + (c - '0');
		c = fgetc(stdin);
	}
	ungetc(c, stdin);
	*num = result * sign;
	return (1);
}

int	scan_string(va_list ap)
{
	char	*str = va_arg(ap, char *);
	int		c;

	skip_input_space();
	while ((c = fgetc(stdin)) != EOF)
	{
		if (isspace(c))
		{
			ungetc(c, stdin);
			break;
		}
		*str++ = (char)c;
	}
	*str = '\0';
	if (c == EOF)
		return (-1);
	return (1);
}

int receive_input(const char **format, va_list ap)
{
	switch (**format)
	{
		case 'c':
			return (scan_char(ap));
		case 'd':
			skip_input_space();
			return (scan_int(ap));
		case 's':
			skip_input_space();
			return (scan_string(ap));
		default:
			return (-1);
	}
}

int ft_scanf(const char *format, ...)
{
	int	result = 0;
	va_list	ap;
	va_start(ap, format);
	const char *cur = format;
	
	while (*cur != '\0')
	{
		if (*cur == '%')
		{
			cur++;
			if (receive_input(&cur, ap) == -1)
				break ;
			else
				result++;
		}
		cur++;
	}
	va_end(ap);
	return (result);
}

int main() {
   int number;
    char str[100];  // Large enough buffer for string input

    // Passing pointers to allow `ft_scanf` to store input into these variables
    ft_scanf("%d", &number);
    printf("Read integer: %d\n", number);

    ft_scanf("%s", str);
    printf("Read string: %s\n", str);
    return 0;
}

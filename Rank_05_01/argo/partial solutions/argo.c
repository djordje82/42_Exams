
#include "argo.h"

int	g_error = 0;
int g_error_no_key = 0;

int	peek(FILE *stream)
{
	int	c;

	c = getc(stream);
	ungetc(c, stream);
	return (c);
}

void unexpected(FILE *stream) {
	if (peek(stream) != EOF)
		printf("Unexpected token '%c'\n", peek(stream));
	else
		printf("Unexpected end of input\n");
}

int	accept(FILE *stream, char c)
{
	if (peek(stream) == c)
	{
		(void)getc(stream);
		return (1);
	}
	return (0);
}

int	expect(FILE *stream, char c)
{
	if (accept(stream, c))
	{
		return (1);
	}
	if ((!g_error_no_key))
		unexpected(stream);
	return (0);
}

// add your code here


bool	parse_json(json *dst, FILE *stream);

bool	parse_integer(json *dst, FILE *stream)
{
	int	sign = 1;
	int	result = 0;

	if (accept(stream, '-'))
	{
		sign = -1;
		if (peek(stream) == EOF)
		{
			ungetc('-', stream);
			g_error = 1;
			return (false);
		}
	}
	if (!isdigit(peek(stream)))
	{
		g_error = 1;
		return (false);
	}
	if (fscanf(stream, "%d", &result) != 1)
	{
		g_error = 1;
		return (false);
	}
	result *= sign;
	dst->type = INTEGER;
	dst->integer = result;
	return (true);
}

char	*get_str(FILE *stream)
{
	char	*buffer = malloc(1096);
	if (!buffer)
		return (NULL);

	int		i = 0;
	while (peek(stream) != '"' && peek(stream) != EOF)
	{
		accept(stream, '\\');
		buffer[i++] = getc(stream);
	}
	if (!accept(stream, '"'))
	{
		free(buffer);
		g_error = 1;
		return (NULL);
	}

	buffer[i] = '\0';
	return (buffer);
}

bool	parse_string(json *dst, FILE *stream)
{
	dst->type = STRING;
	dst->string = get_str(stream);
	if (!dst->string)
	{
		g_error = 1;
		return (false);
	}
	return (true);
}

/**
 * Another json primary type. I store the key value pair in the json struct
 * the key is always a string and the value can be any json type, an integer,
 * a string or a map again which is a json...
 */
bool	parse_map(json *dst, FILE *stream)
{
	int	i = 0;

	dst->type = MAP;
	dst->map.data = malloc(256 * sizeof(pair));
	if (!dst->map.data)
		return NULL;
	dst->map.size = 0;
	while (peek(stream) != '}' && peek(stream) != EOF)
	{
		dst->map.size++;
		// the key is always a string
		if (accept(stream, '"'))
		{
			dst->map.data[i].key = get_str(stream);
			if (!dst->map.data[i].key)
			{
				g_error_no_key = 1;
				return (false);
			}
		}
		else
		{
			g_error_no_key = 1;
			return (false);
		}
		// the key is always followed by a colon and a value
		if (!accept(stream, ':') || !parse_json(&dst->map.data[i].value, stream))
		{
			g_error = 1;
			return (false);
		}
		if (accept(stream, ','))
		{
			i++;
			continue ;
		}
	}
	if (!accept(stream, '}'))
	{
		g_error = 1;
		return (false);
	}
	return (true);
}

bool	parse_json(json *dst, FILE *stream)
{
	if (isdigit(peek(stream)) || (peek(stream) == ('-')))
		return (parse_integer(dst, stream));
	else if (accept(stream, '"'))
		return (parse_string(dst, stream));
	else if (accept(stream, '{'))
		return (parse_map(dst, stream));
	else
		return (false);
}

int	argo(json *dst, FILE *stream)
{
	if (g_error_no_key)
		return (-1);
	if (g_error)
	{
		unexpected(stream);
		return (-1);
	}
	if (!parse_json(dst, stream))
	{
		unexpected(stream);
		return (-1);		
	}
	return (1);
}

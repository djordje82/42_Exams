#include "argo.h"

json	parse_json(FILE *stream);

int		g_error = 0;
int		g_error_no_key = 0;


int	peek(FILE *stream)
{
	int	c;

	c = getc(stream);
	ungetc(c, stream);
	return (c);
}

void	unexpected(FILE *stream)
{
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
	unexpected(stream);
	return (0);
}


char	*get_str(FILE *stream)
{
	if (!accept(stream, '"'))
	{
		g_error = 1;
		return NULL;
	}
	char	*buffer = malloc(1096);
	if (!buffer)
		return (NULL);

	int		i = 0;
	while (peek(stream) != '"' && peek(stream) != EOF)
	{
		char current = getc(stream);
		if (current == '\\')
		{
			current = getc(stream);
			if (current == '\\')
			{
				g_error = 1;
				free(buffer);
				return NULL;	
			}
		}
		buffer[i++] = current;
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

json	parse_string(FILE *stream)
{
	json	json_string;

	json_string.type = STRING;
	json_string.string = get_str(stream);
	return (json_string);
}

json	parse_integer(FILE *stream)
{
	json	num_json;
	int		result;

	num_json.type = INTEGER;
	if (fscanf(stream, "%d", &result) != 1)
	{
		g_error = 1;
		return num_json;
	}
	num_json.integer = result;
	return (num_json);
}

json parse_map(FILE *stream)
{
	json json_map = { .type = MAP, .map.data = NULL, .map.size = 0 };

	if (!accept(stream, '{'))
	{
		g_error = 1;
		return json_map;
	}

	while (peek(stream) != '}' && peek(stream) != EOF)
	{
		pair new_pair;
		if (!accept(stream, '"')) // Ensure key starts with "
		{
			g_error_no_key = 1;
			return json_map;
		}

		new_pair.key = get_str(stream);
		if (g_error || !accept(stream, ':')) // Ensure colon follows key
		{
			free(new_pair.key);
			g_error = 1;
			return json_map;
		}

		new_pair.value = parse_json(stream);
		if (g_error) // Handle parsing failure of value
		{
			free(new_pair.key);
			free_json(new_pair.value);
			return json_map;
		}

		// Resize map and store new key-value pair
		json_map.map.data = realloc(json_map.map.data, ++json_map.map.size * sizeof(pair));
		json_map.map.data[json_map.map.size - 1] = new_pair;

		if (!accept(stream, ',')) // Expect ',' or end of object
			break;
	}

	if (!accept(stream, '}')) // Ensure object ends with '}'
		g_error = 1;

	return json_map;
}

json	parse_json(FILE *stream)
{
	json	nothing;

	if (peek(stream) == '"')
		return (parse_string(stream));
	if (peek(stream) == '{')
		return (parse_map(stream));
	if (peek(stream) == '-' || isdigit(peek(stream)))
		return (parse_integer(stream));
	nothing.type = STRING;
	nothing.string = NULL;
	g_error = 1;
	return (nothing);
}

int	argo(json *dst, FILE *stream)
{
	*dst = parse_json(stream);
	if (g_error_no_key)
	{
		return (-1);
	}
	if (g_error)
	{
		unexpected(stream);
		return (-1);
	}
	return (1);
}
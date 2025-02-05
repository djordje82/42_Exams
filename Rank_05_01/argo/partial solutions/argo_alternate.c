#include "argo.h"

int g_no_key = 0;
int g_error = 0;

json    parse_json(FILE *stream);


int peek(FILE *stream) {
    int c = getc(stream);
    ungetc(c, stream);
    return c;
}

void unexpected(FILE *stream) {
    if (peek(stream) != EOF)
        printf("Unexpected token '%c'\n", peek(stream));
    else
        printf("Unexpected end of input\n");
}

int accept(FILE *stream, char c) {
    if (peek(stream) == c) {
        (void) getc(stream);
        return 1;
    }
    return 0;
}

int expect(FILE *stream, char c) {
    if (accept(stream, c)) {
        return 1;
    }
    unexpected(stream);
    return 0;
}
 //solution below
json    parse_int(FILE *stream) {
    int result;
    int fscan_res;
    json    j_int;
    j_int.type = INTEGER;
    j_int.integer = -42;
    fscan_res = fscanf(stream, "%d", &result);
    if (fscan_res <= 0 || fscan_res == EOF) {
        g_error = 1;
        return j_int;
    }
    j_int.integer = result;
    return j_int;

}

char    *get_str(FILE *stream) {
    char    *buff = malloc(1);
    buff[0] = '\0';
    int     length = 0;
    char    current;
    if(!accept(stream, '"')) {
        g_error = 1;
        return buff;
    }
    while (peek(stream) != EOF && peek(stream) != '"' && !g_error) {
        current = getc(stream);
        if (current == '\\') {
            current = getc(stream);
            if (current != '\\' && current != '"') {
                g_error = 1;
                return buff;
            }
        }
        length++;
        buff = realloc(buff, length + 1);
        buff[length] = '\0';
        buff[length - 1] = current;
    }
    if (!accept(stream, '"'))
        g_error = 1;
    return buff;
}

json    parse_str(FILE *stream) {
    json    j_str;
    j_str.type = STRING;
    j_str.string = get_str(stream);
    return j_str;
}

json    parse_map(FILE *stream) {
    json    j_map;
    j_map.type = MAP;
    j_map.map.data = NULL;
    j_map.map.size = 0;

    if(!accept(stream, '{')) {
        g_error = 1;
        return j_map;
    }
    while(peek(stream) != EOF && peek(stream) != '}') {
        pair    new_pair;
        if(peek(stream) != '"') {
            g_no_key = 1;
            return j_map;
        }
        new_pair.key = get_str(stream);
        if (g_error || !accept(stream, ':')) {
            g_error = 1;
            free(new_pair.key);
            return j_map;
        }
        new_pair.value = parse_json(stream);
        if (g_error) {
            free(new_pair.key);
            free_json(new_pair.value);
            return j_map;
        }
        j_map.map.size++;
        j_map.map.data = realloc(j_map.map.data, j_map.map.size * sizeof(pair));
        j_map.map.data[j_map.map.size - 1] = new_pair;
        if(!accept(stream, ','))
            break ;
    }
    if (g_error || !accept(stream, '}')) {
        g_error =  1;
        return j_map;
    }
    return j_map;
}

json    parse_json(FILE *stream) {
    json    j_empty;
    j_empty.type = INTEGER;
    j_empty.integer = -42;
    if (peek(stream) == '"')
        return parse_str(stream);
    else if (isdigit(peek(stream)) || peek(stream) == '-')
        return parse_int(stream);
    else if (peek(stream) == '{')
        return parse_map(stream);
    g_error = 1;
    return j_empty;
}

int argo(json *dst, FILE *stream) {
    *dst = parse_json(stream);
    if(g_no_key)
        return -1;
    if (g_error) {
        unexpected(stream);
        return -1;
    }
    return 1;
}
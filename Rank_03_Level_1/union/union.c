#include <unistd.h>

int    is_there(char *s, char c)
{
    int i;

    i = 0;
    while (s[i])
    {
        if (s[i] == c)
            return (0);
        i++;
    }
    return (1);
}

int is_in_it(char *s, char c, int pos)
{
    int i;

    i = 0;
    while (i < pos)
    {
        if (s[i] == c)
            return (0);
        i++;
    }
    return (1);
}


int main(int ac, char **av)
{
    int i;

    if (ac == 3)
    {
        i = 0;
        while (av[1][i])
        {
            if (is_in_it(av[1], av[1][i], i))
                write (1, &av[1][i], 1);
            i++;
        }
        i = 0;
        while (av[2][i])
        {
            if (is_there(av[1], av[2][i]))
            {
                if (is_in_it(av[2], av[2][i], i))
                    write (1, &av[2][i], 1);
            }
            i++;
        }
    }
    write (1, "\n", 1);
    return (0);
}
#include <unistd.h>

char    doubles_in(char *s, char c)
{
    while (*s)
    {
        if (*s == c)
            return (1);
        s++;
    }
    return (0);
}

void inter(char *s1, char *s2)
{
    char    printed[128] = {0};
 
    while (*s1)
    {
        if (doubles_in(s2, *s1) && !printed[(unsigned char)*s1])
        {
            write (1, s1, 1);
            printed[(unsigned char)*s1] = 1;
        }
        s1++;
    }
}

int main(int ac, char **av)
{
    if (ac == 3)
        inter(av[1], av[2]);
    write (1, "\n", 1);
    return (0);
}
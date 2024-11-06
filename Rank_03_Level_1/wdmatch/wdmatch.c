#include <unistd.h>

int    wdmatch(char *s1, char *s2)
{
    while (*s1 && *s2)
    {
        if (*s1 == *s2)
        {
            s1++;
        }
        s2++;
    }
    return (*s1 == '\0');
    
}

void    ft_putstr(char *str)
{
    while (*str)
    {
        write (1, str, 1);
        str++;
    }
}

int main(int ac, char **av)
{
    if (ac == 3)
    {
        if (wdmatch(av[1], av[2]))
        {
            ft_putstr(av[1]);
        }
    }
    write (1, "\n", 1);
    return (0);
}
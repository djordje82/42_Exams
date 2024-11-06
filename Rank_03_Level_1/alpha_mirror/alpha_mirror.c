#include <unistd.h>

void    alpha_mirror(char *str)
{
    while (*str)
    {
        if (*str >= 'a' && *str <= 'z')
            *str = 'z' - *str + 'a';
        else if (*str >= 'A' && *str <= 'Z')
            *str = 'Z' - *str + 'A';
        else
            *str = *str;
        str++;
    }
}

int main(int ac, char **av)
{
    if (ac == 2)
    {
        alpha_mirror (av[1]);
        while (*av[1])
        {
            write (1, av[1], 1);
            av[1]++;
        }
    }
    write (1, "\n", 1);
    return (0);
}
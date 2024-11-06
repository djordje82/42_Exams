#include <stdio.h>
#include <stdlib.h>

int main(int ac, char **av)
{
    int res;

    if (ac == 4)
    {
        if (*av[2] == '+')
            res = atoi(av[1]) + atoi(av[3]);
        if (*av[2] == '-')
            res = atoi(av[1]) - atoi(av[3]);
        if (*av[2] == '*')
            res = atoi(av[1]) * atoi(av[3]);
        if (*av[2] == '%')
            res = atoi(av[1]) % atoi(av[3]);
        if (*av[2] == '/')
            res = atoi(av[1]) / atoi(av[3]);
        printf("%d", res);
    }
    printf("\n");
    return (0);
}
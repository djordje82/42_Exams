#include <unistd.h>

void    last_word(char *s)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (s[i])
    {
        if (s[i] == ' ' && s[i + 1] >= 33 && s[i + 1] <= 126)
            j = i + 1;
        i++;
    }
    while (s[j] >= 33 && s[j] <= 126)
    {
        write (1, &s[j], 1);
        j++;
    }
}

int main(int ac, char **av)
{
    if (ac == 2)
    {
        last_word(av[1]);
    }
    write (1, "\n", 1);
    return (0);
}
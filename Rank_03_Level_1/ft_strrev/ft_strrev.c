#include <stdio.h>

char    *ft_strrev(char *str)
{
    int     i;
    int     j;
    char    temp;
    
    i = 0;
    while (str[i])
        i++;
    j = 0;
    i -= 1;
    while (i > j)
    {
        temp = str[j];
        str[j] =  str[i];
        str[i] = temp;
        i--;
        j++;
    }
    return (str);
}
/*
int main(void)
{
    char    str[] = "hello world";
    printf("%s\n", ft_strrev(str));
    return (0);
}
*/

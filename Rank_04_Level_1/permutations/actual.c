#include <stdio.h>
#include <stdlib.h>

int ft_strlen(char *str)
{
    int len = 0;
    while (str[len] != '\0') len++;
    return len;
}

void swap(char *a, char *b)
{
    char tmp = *a;
    *a = *b;
    *b = tmp;
}
void ft_sort(char *str)
{
    int len = ft_strlen(str);
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len - i - 1; j++)
        {
            if (str[j] > str[j + 1])
                swap(&str[j], &str[j + 1]);
        }
    }
}
void permute(char *str, int left, int right)
{
    if (left == right)
    {
        puts(str);
        return;
    }

    for (int i = left; i <= right; i++)
    {
        swap(&str[left], &str[i]);
        ft_sort(str + left + 1); 
        permute(str, left + 1, right);
        swap(&str[left], &str[i]);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        puts("Usage: ./permutations <string>");
        return 1;
    }

    char *str = argv[1];
    int len = 0;
    while (str[len] != '\0') len++;

    permute(str, 0, len - 1);

    return 0;
}

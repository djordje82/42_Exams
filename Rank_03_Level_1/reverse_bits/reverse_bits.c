#include <unistd.h>

unsigned char	reverse_bits(unsigned char octet)
{
    unsigned char   reversed;
    int             i;

    i = 8;
	reversed = 0;
    while (i--)
    {
        reversed = (reversed << 1) | (octet & 1);
        octet >>= 1;
    }
    return (reversed);
}
/*
void	print_bits(unsigned char octet)
{
	unsigned char	bit;
	int				i = 8;

	while (i--)
	{
		bit = ((octet >> i) & 1) + '0';
		write (1, &bit, 1);
	}
}

int main(void)
{
    unsigned char   rev;

    rev = 5;
	print_bits (rev);
	write (1, "\n", 1);
	rev = reverse_bits (rev);
	print_bits (rev);
	return (0);
}
*/
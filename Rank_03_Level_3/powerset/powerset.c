#include <stdio.h>
#include <stdlib.h>

void	print_subset(int *subset, int size)
{
	int	i;

	i = 0;
	printf ("{");
	while (i < size)
	{
		printf ("%d", subset[i]);
		if (i < size - 1)
			printf (", ");
		i++;
	}
	printf ("}\n");
}

void	find_subset(int *arr, int arr_size, int target_sum)
{
	int	found;
	int	total_subsets;
	int	subset_mask;
	int	sum;
	int	subset_size;
	int	*subset;
	int	i;

	found = 0;
	total_subsets = 1 << arr_size;
	subset_mask = 0;
	while (subset_mask < total_subsets)
	{
		sum = 0;
		subset_size = 0;
		subset = (int *)malloc(arr_size * sizeof(int));
		if (subset == NULL)
		{
			perror ("malloc failed");
			exit (1);
		}
		i = 0;
		while (i < arr_size)
		{
			if (subset_mask & (1 << i))
			{
				sum += arr[i];
				subset[subset_size++] = arr[i];
			}
			i++;
		}
		if (sum == target_sum)
		{
			print_subset (subset, subset_size);
			found = 1;
		}
		subset_mask++;
		free (subset);
	}
	if (!found)
		printf ("\n");
}

int	main(int ac, char **av)
{
	if (ac < 3)
	{
		fprintf (stderr, "usage: tagert_sum num1 num2 ... numN\n");
		return (1);
	}
	int target_sum = atoi (av[1]);
	int	arr_size = ac - 2;
	int *arr = (int *)malloc(arr_size * sizeof(int));
	if (arr == NULL)
	{
		perror ("malloc failed");
		return (1);
	}
	int	i = 0;
	while (i < arr_size)
	{
		arr[i] = atoi (av[i + 2]);
		i++;
	}
	find_subset (arr, arr_size, target_sum);
	free (arr);
	return (0);
}

#include "minishell.h"
/**
 * Frees null terminated array of strings
 */
void	free_split(char **arr)
{
	size_t	i;

	i = 0;
	while (arr[i])
	{
		free (arr[i]);
		i++;
	}
	free(arr);
}
/**
 * Frees non-NULL terminated array of strings in case of error.
 * @param arr Array of strings.
 * @param n index number where we failed.
 */
void	free_partial(char **arr, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
		free(arr[i++]);
	free(arr);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 18:21:31 by wheino            #+#    #+#             */
/*   Updated: 2025/10/08 18:24:45 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/**
 * Frees null terminated array of strings
 */
void	free_split(char ***arr_ptr)
{
	size_t	i;
	char	**arr;

	if (arr_ptr == NULL || *arr_ptr == NULL)
		return ;
	arr = *arr_ptr;
	i = 0;
	while (arr[i])
	{
		free (arr[i]);
		i++;
	}
	free(arr);
	*arr_ptr = NULL;
}

/**
 * Frees non-NULL terminated array of strings in case of error.
 * @param arr Array of strings.
 * @param n index number where we failed.
 */
void	free_partial(char **arr, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
		free(arr[i++]);
	free(arr);
}

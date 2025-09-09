/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:55:42 by wheino            #+#    #+#             */
/*   Updated: 2025/05/02 15:34:43 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int		count;

	if (lst == NULL)
		return (0);
	count = 0;
	while (lst->next != NULL)
	{
		count++;
		lst = lst->next;
	}
	count++;
	return (count);
}

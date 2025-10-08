/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_working_dir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 18:21:36 by wheino            #+#    #+#             */
/*   Updated: 2025/10/08 18:24:54 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	getworkindir(char *buf, size_t size)
{
	if (NULL == getcwd(buf, size))
		perror("getwcd FAILED");
}

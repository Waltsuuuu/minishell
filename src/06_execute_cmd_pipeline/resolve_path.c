#include "minishell.h"
/**
 * @brief Extracts PATH directories from environment.
 *
 * @param envp environment variables array
 * @returns Array of PATH directories split by ':' or NULL on failure.
 */
char	**find_from_path(char *envp[])
{
	char	*path;
	size_t	counter;

	path = NULL;
	counter = 0;
	while (envp[counter])
	{
		if (ft_strncmp(envp[counter], "PATH=", 5) == 0)
		{
			path = &envp[counter][5];
			break ;
		}
		counter++;
	}
	if (!path || !*path)
		return (NULL);
	return (ft_split(path, ':'));
}

/**
 * @brief Joins a directory path with a command name.
 *
 * @param path Directory under PATH variable.
 * @param cmd Command to append.
 * @return Newly allocated absolute path string or NULL on failure.
 */
char	*join_cmd_to_path(const char *path, const char *cmd)
{
	char	*temp;
	char	*absolute_path;

	if (!path || !cmd)
		return (NULL);
	temp = ft_strjoin(path, "/");
	if (!temp)
		return (NULL);
	absolute_path = ft_strjoin(temp, cmd);
	free (temp);
	return (absolute_path);
}

/**
 * @brief Builds an array of absolute paths for a command.
 *
 * @param paths Array of PATH directories.
 * @param cmd Command to resolve.
 * @return Array of absolute paths or NULL on failure.
 */
char	**build_absolute_paths(char **paths, const char *cmd)
{
	size_t	counter;
	size_t	len;
	char	**absolute_paths;

	if (!paths || !cmd || !*cmd)
		return (NULL);
	len = 0;
	while (paths[len])
		len++;
	absolute_paths = malloc(sizeof(char *) *(len + 1));
	if (!absolute_paths)
		return (NULL);
	counter = 0;
	while (paths[counter])
	{
		absolute_paths[counter] = join_cmd_to_path(paths[counter], cmd);
		if (!absolute_paths[counter])
			return (free_partial(absolute_paths, counter), NULL);
		counter++;
	}
	absolute_paths[counter] = NULL;
	return (absolute_paths);
}

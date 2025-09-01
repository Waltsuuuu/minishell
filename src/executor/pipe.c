#include "minishell.h"

static int	wait_one(pid_t pid, int *out_status)
{
	int status;
	int result;

	if (pid <= 0)
		return (0);
	while (1)
	{
		result = waitpid(pid, &status, 0);
		if (result == pid)
		{
			*out_status = status;
			return (0);
		}
		if (result == -1 && errno == EINTR)
			continue;
		if (result == -1 && errno == ECHILD)
			return (0); /* already reaped elsewhere */
		return (-1);    /* unexpected error */
	}
}

static int	wait_two_and_status(pid_t left, pid_t right)
{
	int s1;
	int s2;

	if (wait_one(left, &s1) < 0)
		return (1);
	if (wait_one(right, &s2) < 0)
		return (1);
	if (WIFEXITED(s2))
		return (WEXITSTATUS(s2));
	if (WIFSIGNALED(s2))
		return (128 + WTERMSIG(s2));
	return (1);
}

/* Fork one stage of the pipeline.
   in_fd  = -1 means leave stdin as-is; out_fd = -1 means leave stdout as-is. 
static void print_not_found(const char *cmd)
{
	write(2, "minishell: ", 11);
	write(2, cmd, strlen(cmd));
	write(2, ": command not found\n", 20);
}*/

static pid_t spawn_stage(char **argv, char **envp, int pipefd[2],
                         int use_stdin, int use_stdout)            // pipefd is the same fd[] created by pipe();
{                                                                  // use_stdin/use_stdout tell which end to dup
    pid_t pid = fork();                                            // create a child process
    if (pid == 0)                                                  // child path
    {
        char **dirs;                                               // will hold PATH directories (if any)
        int    i;

        setup_signal_handlers_for_child();                         // child should use default-like signals (SIGINT/SIGQUIT)

        if (use_stdin && dup2(pipefd[0], STDIN_FILENO) < 0)        // if this stage should read from pipe: dup read end -> stdin
            _exit(1);                                              // on dup2 failure, exit child with error

        if (use_stdout && dup2(pipefd[1], STDOUT_FILENO) < 0)      // if this stage should write to pipe: dup write end -> stdout
            _exit(1);                                              // on dup2 failure, exit child with error

        close(pipefd[0]);                                          // IMPORTANT: child closes both ends of the pipe
        close(pipefd[1]);                                          // (avoids keeping the pipe alive and hanging the reader)

        if (argv && argv[0] && has_slash(argv[0]))                 // if command name contains '/', treat as direct/absolute path
        {
            execve(argv[0], argv, envp);                           // try to exec directly (only returns on failure)
            _exit((errno == ENOENT || errno == ENOTDIR) ? 127 : 126); // map failure: 127=not found, 126=not runnable
        }

        dirs = find_from_path(envp);                               // otherwise, get PATH directories (may be NULL if no PATH)
        if (argv && argv[0] && dirs)                               // if we have a command and PATH dirs
        {
            for (i = 0; dirs[i]; i++)                              // iterate each PATH directory
            {
                char *full = join_cmd_to_path(dirs[i], argv[0]);   // build "dir/command"
                if (!full) { free_split(dirs); _exit(1); }         // allocation failure → exit
                execve(full, argv, envp);                          // try exec; if it succeeds, this never returns
                free(full);                                        // if exec failed, free and try next dir
            }
            free_split(dirs);                                      // done with PATH dirs
        }

        write(2, "minishell: ", 11);                               // if we reached here, nothing exec’d: print "not found"
        if (argv && argv[0]) write(2, argv[0], strlen(argv[0]));   // print command name if available
        write(2, ": command not found\n", 20);
        _exit(127);                                                // exit as "command not found"
    }
    return pid;                                                    // parent receives child's PID (or -1 on fork error)
}

int exec_pipe2(char **left_argv, char **right_argv, char *envp[])   // entry: argv for left/right commands + env
{
    int   fd[2];                                                    // pipe descriptors: fd[0]=read end, fd[1]=write end
    pid_t lpid;                                                     // will hold PID of left child
    pid_t rpid;                                                     // will hold PID of right child

    if (pipe(fd) < 0)                                              // create the pipe; returns 0 on success, -1 on error
        return (-1);                                                // on error, return non-zero (simple failure code)

    // Left stage: write side of the pipe → stdout
    lpid = spawn_stage(left_argv,  envp, fd, 0, 1);                // spawn left child; use_stdin=0 (leave stdin),
                                                                   // use_stdout=1 (dup pipe write end to stdout)

    // Right stage: read side of the pipe → stdin
    rpid = spawn_stage(right_argv, envp, fd, 1, 0);                // spawn right child; use_stdin=1 (dup pipe read end to stdin),
                                                                   // use_stdout=0 (leave stdout)

    close(fd[0]);                                                  // parent: close read end (parent must not keep ends open)
    close(fd[1]);                                                  // parent: close write end

    return (wait_two_and_status(lpid, rpid));                      // reap both; return RIGHT child's status for $?
}


static char **make_argv_simple(char *s)
{
    char    *trim;
    char    **argv;

    trim = ft_strtrim(s, " \t");     // 1) Allocates a new copy of `s` with
                                     //    LEADING and TRAILING spaces/tabs removed.
                                     //    (Inner spaces/tabs are left as-is.)

    if (!trim)                       // 2) If allocation failed, bail out.
        return (NULL);

    argv = ft_split(trim, ' ');      // 3) Splits the TRIMMED string into words
                                     //    using a SINGLE delimiter: the space ' '.
                                     //    Consecutive spaces are skipped; tabs
                                     //    inside the string are NOT considered delimiters,
                                     //    so "a\tb" stays as one token.

    free(trim);                      // 4) Free the temporary trimmed string.

    return (argv);                   // 5) Return a NULL-terminated array of words
                                     //    (caller must free with your free_split()).
}


int run_line_minipipe(char *line, t_shell *shell, char *envp[])     // Run either a simple cmd or A|B; returns exit status
{
    char    **parts;                                            // holds the line split around the first '|'
    char    **left_argv;                                        // argv for the left side (A)
    char    **right_argv;                                       // argv for the right side (B)
    int     status;                                             // result status to return

    parts = ft_split(line, '|');                                // naive split by '|' (no quote awareness)
    if (!parts)                                                 // allocation failure?
        return (1);                                             // return non-zero on error

    if (!parts[1])                                              // if there's no right side => no pipe present
    {
        char **paths;                                           // PATH= directories as array
        char **abs_paths;                                       // candidate absolute paths "dir/cmd"

        left_argv = make_argv_simple(parts[0]);                 // build argv by splitting on spaces (no quotes handled)
        if (!left_argv || !left_argv[0]) {                      // empty/blank line after trimming?
            free_split(left_argv);                              // free if allocated
            free_split(parts);                                  // free the split around '|'
            return (0);                                         // treat as success/no-op
        }
        shell->input.words = left_argv;                            // point shell->input.words to this argv (used by exec_ext_func)
        paths = find_from_path(envp);                           // extract PATH= value and split by ':'
        abs_paths = build_absolute_paths(paths, left_argv[0]);  // join each dir with argv[0] -> "dir/argv0"
        exec_ext_func_and_direct(abs_paths, shell);                     // your single-cmd exec path; forks, waits, sets sh->last_status

        free_split(abs_paths);                                  // free "dir/argv0" candidates
        free_split(paths);                                      // free PATH directories array
        status = shell->last_status;                               // capture status to return from this function

        free_split(left_argv);                                  // free argv built above
        free_split(parts);                                      // free the '|' split array
        return (status);                                        // return the single-command exit status
    }

    /* Pipe case: handle exactly "A | B" */
    left_argv  = make_argv_simple(parts[0]);                    // argv for left command A
    right_argv = make_argv_simple(parts[1]);                    // argv for right command B
    status = exec_pipe2(left_argv, right_argv, envp);           // run the 2-stage pipeline, returns RIGHT's status ($?)
    free_split(left_argv);                                      // free left argv
    free_split(right_argv);                                     // free right argv
    free_split(parts);                                          // free the '|' split array
    return (status);                                            // return pipeline status (rightmost command)
}


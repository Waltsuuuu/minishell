# Minishell

Minishell is a minimal [Bash](https://www.gnu.org/software/bash/manual/bash.html#What-is-Bash_003f)-inspired shell program designed to study shell internals through the re-implementation of essential features.

Th shell offers an interactive command-line interface for executing programs, working with the file system, handling pipes and redirections, and managing environment variables.

------------------------------------------------------------------------

## Capabilities

### Execution

-   Run binaries found in `$PATH`
-   Execute programs via relative or absolute paths
-   Correct handling of exit codes (`$?`)

### Built-in Commands

-   `echo` (supports `-n`)
-   `cd`
-   `pwd`
-   `export`
-   `unset`
-   `env`
-   `exit`

### Redirections & Pipes

-   Input redirection (`<`)
-   Output redirection (`>`)
-   Append redirection (`>>`)
-   Here-documents (`<<`)
-   Single and multiple pipes (`|`)

### Parsing & Expansion

-   Single quotes `'` (literal behavior)
-   Double quotes `"` (variable expansion allowed)
-   Environment variable expansion (`$VAR`)
-   Exit status expansion (`$?`)

### User Interaction

-   Line editing and command history via `readline`
-   Signal handling for `Ctrl-C`, `Ctrl-D`, `Ctrl-\`

------------------------------------------------------------------------

## Building & Running

### Requirements

- **cc** (system C compiler)  
- **make**  
- **GNU Readline** (development headers)


### Compile

``` bash
git clone https://github.com/M-W-Minishell/minishell.git
cd minishell
make
```

### Run

``` bash
./minishell
```

------------------------------------------------------------------------

## Makefile Targets

- `make`  
  Builds the `minishell` executable.

- `make clean`  
  Removes all compiled object files.

- `make fclean`  
  Removes all compiled object files and the `minishell` executable.

- `make re`  
  Rebuilds the project from scratch by running `fclean` followed by `make`.

------------------------------------------------------------------------

## How to Use

### Basic commands

``` bash
ls -l
echo Hello
grep main src/main.c
```

### Pipes

``` bash
ls | wc -l
cat file.txt | grep error | sort
```

### Redirections

``` bash
echo test > file.txt
cat < file.txt
echo more >> file.txt
cat << EOF
hello
world
EOF
```

### Environment variables

``` bash
export NAME=minishell
echo $NAME
unset NAME
```


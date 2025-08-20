# Name of the static library
NAME = libft.a

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -I. -D BUFFER_SIZE=42

# Source folders
SRC_DIR = src
GNL_DIR = get_next_line
PRINTF_DIR = printf

# Libft source files
SRC_FILES = \
ft_atoi ft_bzero ft_calloc ft_isalnum ft_isalpha ft_isascii ft_isdigit ft_isprint \
ft_itoa ft_memchr ft_memcmp ft_memcpy ft_memmove ft_memset ft_putchar_fd ft_putendl_fd \
ft_putnbr_fd ft_putstr_fd ft_split ft_strchr ft_strdup ft_striteri ft_strjoin \
ft_strlcat ft_strlcpy ft_strlen ft_strmapi ft_strncmp ft_strnstr ft_strrchr \
ft_strtrim ft_substr ft_tolower ft_toupper

# Get Next Line files
GNL_FILES = get_next_line get_next_line_utils

# Printf files
PRINTF_FILES = ft_printf ft_printf_utils

# Combine all source files with paths
SRCS = $(addprefix $(SRC_DIR)/, $(addsuffix .c, $(SRC_FILES))) \
       $(addprefix $(GNL_DIR)/, $(addsuffix .c, $(GNL_FILES))) \
       $(addprefix $(PRINTF_DIR)/, $(addsuffix .c, $(PRINTF_FILES)))

# Convert .c files to .o files
OBJS = $(SRCS:.c=.o)

# Default rule
all: $(NAME)

# Build static library from object files
$(NAME): $(OBJS)
	@ar rcs $@ $^

# Compilation rule for .c to .o
%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

# Clean object files
clean:
	@rm -f $(OBJS)

# Clean object files and lib
fclean: clean
	@rm -f $(NAME)

# Rebuild everything
re: fclean all


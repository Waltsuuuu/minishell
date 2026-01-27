NAME        := minishell
CC          := cc
CFLAGS      := -Wall -Wextra -Werror
INCS        := -I include -I libft

UNAME_S     := $(shell uname -s)

# Try Homebrew readline prefix (works on macOS with brew; empty on Linux)
READLINE_DIR := $(shell brew --prefix readline 2>/dev/null)

ifeq ($(UNAME_S),Darwin)
  # macOS: prefer GNU readline from Homebrew (keg-only)
  ifeq ($(READLINE_DIR),)
    $(error "GNU readline not found. Run: brew install readline")
  endif
  CFLAGS  += -I$(READLINE_DIR)/include
  LDFLAGS += -L$(READLINE_DIR)/lib
endif

# Linux: assumes libreadline-dev / readline-devel installed in system paths
LDLIBS      += -lreadline -lhistory -lncurses
LIBFT_DIR   := libft
LIBFT       := $(LIBFT_DIR)/libft.a
LIBS        := -L$(LIBFT_DIR) -lft

SRC			:=  src/00_check_quote_balance/quote_check.c \
				src/01_tokenize/parse_input.c \
				src/01_tokenize/tokenize.c \
				src/01_tokenize/tok_init_free.c \
				src/01_tokenize/tok_utils.c \
				src/02_exapand_vars_and_status/01_expansion.c \
				src/02_exapand_vars_and_status/02_expand_status.c \
				src/02_exapand_vars_and_status/03_expand_variable.c \
				src/02_exapand_vars_and_status/04_expand_utils.c \
				src/02_exapand_vars_and_status/05_expand_utils_2.c \
				src/03_remove_quotes/quote_removal.c \
				src/04_build_cmd_pipeline/build_pipeline.c \
				src/04_build_cmd_pipeline/pipeline_args.c \
				src/04_build_cmd_pipeline/pipeline_exit_free.c \
				src/04_build_cmd_pipeline/pipeline_redirs.c \
				src/04_build_cmd_pipeline/pipeline_utils.c \
				src/05_collect_heredocs/collect_heredocs.c \
				src/05_collect_heredocs/hd_expand_status.c \
				src/05_collect_heredocs/hd_expand_var.c \
				src/05_collect_heredocs/hd_input_read_write.c \
				src/05_collect_heredocs/hd_signal_utils.c \
				src/05_collect_heredocs/hd_utils.c \
				src/06_execute_cmd_pipeline/apply_redirs.c \
				src/06_execute_cmd_pipeline/exec_direct_and_path.c \
				src/06_execute_cmd_pipeline/exec_dispatch.c \
				src/06_execute_cmd_pipeline/exec_pipeline.c \
				src/06_execute_cmd_pipeline/exec_pipeline_utils.c \
				src/06_execute_cmd_pipeline/exec_utils.c \
				src/06_execute_cmd_pipeline/handle_redirs.c \
				src/06_execute_cmd_pipeline/pipe_utils.c \
				src/06_execute_cmd_pipeline/pipe_utils2.c \
				src/06_execute_cmd_pipeline/resolve_path.c \
				src/06_execute_cmd_pipeline/spawn_cmd.c \
				src/06_execute_cmd_pipeline/wait_utils.c \
				src/07_builtins/apply_redirs_parent.c \
				src/07_builtins/builtin_utils.c \
				src/07_builtins/cd.c \
				src/07_builtins/echo.c \
				src/07_builtins/env.c \
				src/07_builtins/env_array_build.c \
				src/07_builtins/env_init.c \
				src/07_builtins/exit.c \
				src/07_builtins/export_builtin.c \
				src/07_builtins/export_exec_parent.c \
				src/07_builtins/export_helpers.c \
				src/07_builtins/pwd.c \
				src/07_builtins/unset.c \
				src/main/main.c \
				src/main/main_parse.c \
				src/utils/count_heredocs.c \
				src/utils/env_utils.c \
				src/utils/error_utils.c \
				src/utils/free_allocs.c \
				src/utils/free_split.c \
				src/utils/ft_strtoll.c \
				src/utils/get_working_dir.c \
				src/utils/mode_check.c \
				src/utils/print_msh_banner.c \
				src/utils/signal_handling/signal.c \
				src/utils/tty.c
OBJ         := $(SRC:.c=.o)

.PHONY: all clean fclean re

all: $(LIBFT) $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) $(LDFLAGS) $(LDLIBS) -o $@

%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

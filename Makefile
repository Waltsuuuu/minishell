# =========================
# Project
# =========================
NAME        := minishell
CC          := cc
CFLAGS      := -Wall -Wextra -Werror
INCS        := -I include -I libft
SRC         :=  src/main.c \
                src/00_check_quote_balance/quote_check.c \
                src/01_tokenize/parse_input.c src/01_tokenize/tokenize.c src/01_tokenize/tok_init_free.c src/01_tokenize/tok_utils.c src/01_tokenize/tok_debug.c \
                src/02_exapand_vars_and_status/01_expansion.c src/02_exapand_vars_and_status/02_expand_status.c src/02_exapand_vars_and_status/03_expand_variable.c src/02_exapand_vars_and_status/04_expand_utils.c src/02_exapand_vars_and_status/05_expand_utils_2.c \
                src/03_remove_quotes/quote_removal.c \
                src/04_build_cmd_pipeline/build_pipeline.c src/04_build_cmd_pipeline/pipeline_args.c src/04_build_cmd_pipeline/pipeline_debugger.c src/04_build_cmd_pipeline/pipeline_exit_free.c src/04_build_cmd_pipeline/pipeline_redirs.c src/04_build_cmd_pipeline/pipeline_utils.c \
                src/05_collect_heredocs/collect_heredocs.c src/05_collect_heredocs/hd_signal_utils.c src/05_collect_heredocs/hd_tty_utils.c src/05_collect_heredocs/hd_utils.c src/05_collect_heredocs/hd_input_read_write.c \
				src/06_execute_cmd_pipeline/exec_pipeline.c src/06_execute_cmd_pipeline/pipe_utils.c src/06_execute_cmd_pipeline/resolve_path.c src/06_execute_cmd_pipeline/spawn_cmd.c src/06_execute_cmd_pipeline/wait_utils.c src/06_execute_cmd_pipeline/exec_dispatch.c  \
				src/07_builtins/env_init.c src/07_builtins/env.c src/07_builtins/export_exec_parent.c src/07_builtins/export_builtin.c src/07_builtins/export_helpers.c \
                src/utils/free_allocs.c src/utils/free_split.c src/utils/get_working_dir.c src/utils/print_msh_banner.c src/utils/signal_handling/signal.c	src/07_builtins/unset.c src/07_builtins/builtin_utils.c src/07_builtins/cd.c src/07_builtins/pwd.c \
				src/07_builtins/echo.c src/utils/ft_strtoll.c src/07_builtins/exit.c
OBJ         := $(SRC:.c=.o)

# =========================
# Libft
# =========================
LIBFT_DIR   := libft
LIBFT       := $(LIBFT_DIR)/libft.a

# =========================
# Readline (Linux / macOS)
# =========================
UNAME_S     := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
  RL_INC    ?= -I/usr/include -I/usr/include/readline
  RL_LIB    ?= -lreadline -lncurses
else
  # macOS: try Homebrew prefixes (Apple Silicon first, then Intel)
  BREW_PREFIX ?= $(shell if [ -d /opt/homebrew ]; then echo /opt/homebrew; \
                           elif [ -d /usr/local ]; then echo /usr/local; fi)
  RL_INC    ?= -I$(BREW_PREFIX)/opt/readline/include
  RL_LIB    ?= -L$(BREW_PREFIX)/opt/readline/lib -lreadline
  # Silence readline deprecation warnings on macOS (optional)
  CFLAGS    += -Wno-deprecated-declarations
endif

# =========================
# Pretty printing
# =========================
OK   := \033[1;32m
ERR  := \033[1;31m
MUT  := \033[2m
ACC  := \033[1;36m
RST  := \033[0m

ICON_OK   := ✔
ICON_FAIL := ✖
ICON_BLD  := 🔧
ICON_LNK  := 🔗
ICON_CLS  := 🧹
ICON_TRS  := 🗑️
ICON_RE   := ♻️

MAKEFLAGS += --no-print-directory

.PHONY: all clean fclean re libft

# =========================
# Default: build everything
# =========================
all: $(LIBFT) $(NAME)
	@printf "$(OK)$(ICON_OK) Done.$(RST)\n"

$(NAME): $(OBJ) $(LIBFT)
	@printf "$(ACC)$(ICON_LNK) Linking $(NAME)...$(RST)\n"
	@$(CC) $(CFLAGS) $(OBJ) $(INCS) $(RL_INC) -L$(LIBFT_DIR) -lft $(RL_LIB) -o $@
	@printf "$(OK)$(ICON_OK) Built $(NAME)$(RST)\n"

# =========================
# Compilation
# =========================
%.o: %.c
	@mkdir -p $(dir $@)
	@printf "$(ACC)$(ICON_BLD) Compiling %-40s$(MUT)[cc]$(RST)\n" "$<"
	@$(CC) $(CFLAGS) $(INCS) $(RL_INC) -c $< -o $@

# =========================
# Libft
# =========================
$(LIBFT):
	@printf "$(ACC)$(ICON_BLD) Building libft$(RST)\n"
	@$(MAKE) -C $(LIBFT_DIR) >/dev/null

# =========================
# Cleaning
# =========================
clean:
	@printf "$(ACC)$(ICON_CLS) Cleaning objects$(RST)\n"
	@rm -f $(OBJ)
	@find . -type f -name '*.o' -not -path './$(LIBFT_DIR)/*' -delete 2>/dev/null || true
	@printf "$(MUT)$(ICON_CLS) Cleaning libft objects$(RST)\n"
	@$(MAKE) -C $(LIBFT_DIR) clean >/dev/null

fclean: clean
	@if [ -f "$(NAME)" ]; then \
		printf "$(ACC)$(ICON_TRS) Removing $(NAME)$(RST)\n"; \
		rm -f $(NAME); \
	else \
		printf "$(MUT)$(ICON_TRS) Already clean (no binary)$(RST)\n"; \
	fi
	@printf "$(MUT)$(ICON_TRS) Removing libft.a$(RST)\n"
	@$(MAKE) -C $(LIBFT_DIR) fclean >/dev/null

re:
	@printf "$(ACC)$(ICON_RE) Rebuilding$(RST)\n"
	@$(MAKE) fclean
	@$(MAKE) all

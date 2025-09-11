# =========================
# Project
# =========================
NAME        := minishell
CC          := cc
CFLAGS      := -Wall -Wextra -Werror
INCS        := -I include -I libft
SRC         :=	src/main.c	src/executor/resolve_path.c	src/utils/free_split.c src/parser/simple_parse.c \
				src/signal_handling/signal.c src/parser/tokenize.c src/parser/tok_init_free.c src/parser/tok_utils.c src/utils/quote_check.c \
				src/expander/01_expansion.c src/expander/02_expand_status.c src/expander/03_expand_variable.c src/expander/04_expand_utils.c  \
				src/expander/05_expand_utils_2.c src/utils/quote_removal.c src/build_commands/build_pipeline.c src/build_commands/pipeline_debugger.c \
				src/executor/exec_pipeline.c src/executor/pipe_utils.c src/executor/spawn_cmd.c src/executor/wait_utils.c src/build_commands/pipeline_exit_free.c
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
  RL_PREFIX ?= /opt/homebrew   # /usr/local for Intel mac if needed
  RL_INC    ?= -I$(RL_PREFIX)/opt/readline/include
  RL_LIB    ?= -L$(RL_PREFIX)/opt/readline/lib -lreadline
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

# Silence sub-make directory spam
MAKEFLAGS += --no-print-directory

.PHONY: all clean fclean re libft

# =========================
# Default: build everything
# =========================
all: $(LIBFT) $(NAME)
	@printf "$(OK)$(ICON_OK) Done.$(RST)\n"

# Link step with a nice line
$(NAME): $(OBJ) $(LIBFT)
	@printf "$(ACC)$(ICON_LNK) Linking $(NAME)...$(RST)\n"
	@$(CC) $(CFLAGS) $(OBJ) $(INCS) $(RL_INC) -L$(LIBFT_DIR) -lft $(RL_LIB) -o $@
	@printf "$(OK)$(ICON_OK) Built $(NAME)$(RST)\n"

# Objects with muted stdout but visible errors
%.o: %.c
	@mkdir -p $(dir $@)
	@printf "$(ACC)$(ICON_BLD) Compiling %-30s$(MUT) [cc]$(RST)\n" "$<"
	@$(CC) $(CFLAGS) $(INCS) $(RL_INC) -c $< -o $@

# Build libft (muted)
$(LIBFT):
	@printf "$(ACC)$(ICON_BLD) Building libft$(RST)\n"
	@$(MAKE) -C $(LIBFT_DIR) >/dev/null

# =========================
# Cleaning
# =========================
clean:
	@printf "$(ACC)$(ICON_CLS) Cleaning objects$(RST)\n"
	@rm -f $(OBJ)
	@find . -type f -name '*.o' -not -path './$(LIBFT_DIR)/*' -delete || true
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
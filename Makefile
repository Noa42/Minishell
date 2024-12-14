# Librerías
LIBFT_DIR = include/libft
LIBFT = $(LIBFT_DIR)/libft.a
FT_PRINTF_DIR = include/ft_printf
FT_PRINTF = $(FT_PRINTF_DIR)/libftprintf.a

# Flags
INC = -I /home/achacon-/.brew/Cellar/readline/8.2.13/include
LFLAGS = -L /home/achacon-/.brew/Cellar/readline/8.2.13/lib -lreadline -lncurses
CFLAGS = -Wall -Werror -Wextra
COMPILE = -g

# Colors
GREEN = \033[0;32m
YELLOW = \033[0;93m
FYELLOW = \033[38;5;154m
BLUE = \033[0;96m
PURPLE = \033[0;95m
PINK = \033[0;91m
RED = \033[0;31m
END = \033[0m

# Minishell
NAME = minishell
SRC = src/builtins/builtins_utils.c\
src/builtins/builtins_execution.c\
src/builtins/ft_cd.c\
src/builtins/ft_echo.c\
src/builtins/ft_env.c\
src/builtins/ft_exit.c\
src/builtins/ft_export.c\
src/builtins/ft_pwd.c\
src/builtins/ft_unset.c\
src/data/data.c\
src/execution/execution_utils.c\
src/execution/execution.c\
src/execution/multiple_cmd.c\
src/execution/one_cmd.c\
src/execution/path.c\
src/lists/cmd_list_utils_1.c\
src/lists/cmd_list_utils_2.c\
src/lists/redir_list_utils.c\
src/lists/token_list_utils.c\
src/lists/print_lists.c\
src/main/main.c\
src/redirs/APPEND_redir.c\
src/redirs/HERE_DOC_redir.c\
src/redirs/HERE_DOC_redir_utils.c\
src/redirs/INPUT_redir.c\
src/redirs/OUTPUT_redir.c\
src/redirs/redirs_execution.c\
src/redirs/redirs_utils.c\
src/signals/signals.c\
src/utils/utils_1.c\
src/utils/utils_2.c\
src/parsing/parsing_1.c\
src/parsing/parsing_2.c\
src/parsing/parsing_3.c\
src/parsing/parsing_4.c\
src/parsing/parsing_5.c\

OBJ_DIR = obj
OBJT = $(SRC:src/%.c=$(OBJ_DIR)/%.o)

# Crear directorios necesarios
DIRS = $(sort $(dir $(OBJT)))

# Compilation Rules
$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	@gcc ${CFLAGS} ${INC} -c $< -o $@

all: $(NAME)

$(NAME): ${OBJT}
	@echo "$(YELLOW)Compiling Libft.$(END)"
	@make -s -C  $(LIBFT_DIR)
	@echo "$(GREEN)Libft Compiled.$(END)"
	@echo "$(YELLOW)Compiling Ft_printf.$(END)"
	@make -s -C $(FT_PRINTF_DIR)
	@echo "$(GREEN)Ft_printf Compiled.$(END)"
	@echo "$(YELLOW)Compiling Minishell.$(END)"
	@gcc $(COMPILE) $(OBJT) $(LIBFT) $(FT_PRINTF) $(LFLAGS) -o $(NAME)
	@echo "$(GREEN)Minishell compiled successfully!$(END)"

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)Object files removed!$(END)"
	@make -s -C $(LIBFT_DIR) clean
	@make -s -C $(FT_PRINTF_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)$(NAME) removed!$(END)"
	@make -s -C $(LIBFT_DIR) fclean
	@make -s -C $(FT_PRINTF_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/08 15:19:20 by jqueijo-          #+#    #+#              #
#    Updated: 2024/10/22 15:27:50 by fandre-b         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC_PATH = ./srcs
OBJ_DIR = ./obj

SRC = $(SRC_PATH)/main.c \
	$(SRC_PATH)/init/main_utils.c \
	$(SRC_PATH)/init/init.c \
	$(SRC_PATH)/init/cleanup.c \
	$(SRC_PATH)/init/export_init.c \
	$(SRC_PATH)/init/cleanup_struct.c \
	$(SRC_PATH)/init/env_init.c \
	$(SRC_PATH)/utils/token_utils.c \
	$(SRC_PATH)/utils/len_utils.c \
	$(SRC_PATH)/utils/to_utils.c \
	$(SRC_PATH)/utils/char_extract_utils.c \
	$(SRC_PATH)/utils/var_utils.c \
	$(SRC_PATH)/utils/print_utils.c \
	$(SRC_PATH)/utils/token_utils2.c \
	$(SRC_PATH)/utils/find_x_utils.c \
	$(SRC_PATH)/utils/fd_utils.c \
	$(SRC_PATH)/utils/str_utils.c \
	$(SRC_PATH)/utils/is_x.c \
	$(SRC_PATH)/parsing/first_pars.c \
	$(SRC_PATH)/parsing/error_msg.c \
	$(SRC_PATH)/exe_cmd/env.c \
	$(SRC_PATH)/processing/var_conc.c \
	$(SRC_PATH)/processing/quotes_hide_operators.c \
	$(SRC_PATH)/processing/var_remove_quotes.c \
	$(SRC_PATH)/processing/input_trim_and_tokenize.c \
	$(SRC_PATH)/processing/split_into_words.c \
	$(SRC_PATH)/processing/heredoc_expand.c \
	$(SRC_PATH)/processing/quotes_remover.c \
	$(SRC_PATH)/processing/token_separate_operator.c \
	$(SRC_PATH)/processing/input_process.c \
	$(SRC_PATH)/processing/input_tokenize.c \
	$(SRC_PATH)/processing/tilde_replace.c \
	$(SRC_PATH)/processing/token_check_before_pipex.c \
	$(SRC_PATH)/processing/var_replace.c \
	$(SRC_PATH)/processing/var_swap.c \
	$(SRC_PATH)/processing/token_split_words.c \
	$(SRC_PATH)/processing/tokenize_smarter.c \
	$(SRC_PATH)/exe_cmd/pipex_struct.c \
	$(SRC_PATH)/exe_cmd/pipex_path.c \
	$(SRC_PATH)/exe_cmd/built_in_cmds.c \
	$(SRC_PATH)/exe_cmd/pipex_main.c \
	$(SRC_PATH)/exe_cmd/here_doc.c \
	$(SRC_PATH)/exe_cmd/pipex_utils.c \
	$(SRC_PATH)/exe_cmd/pipex_path_utils.c \
	$(SRC_PATH)/exe_cmd/history.c \
	$(SRC_PATH)/exe_cmd/pipex_files_ctl.c \
	$(SRC_PATH)/built_ins/run_built_ins.c \
	$(SRC_PATH)/built_ins/ft_exit.c \
	$(SRC_PATH)/built_ins/ft_unset.c \
	$(SRC_PATH)/built_ins/ft_export.c \
	$(SRC_PATH)/signals/ft_signals_set.c \
	$(SRC_PATH)/signals/ft_signals_func.c \

OBJ = $(SRC:$(SRC_PATH)/%.c=$(OBJ_DIR)/%.o)

CC = cc
CFLAGS = -Wall -Werror -Wextra -g #-fsanitize=address

SANITIZE = -g -fsanitize=address

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -lreadline

# @mkdir: the @ tells Make not to echo the command before executing.
# -p: mkdir creates target and necessary parent directories and doesn't throw an error if folder already exists.
# $(@D): %@ represents target of current rule, + D extracts the directory part of target.
$(OBJ_DIR)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
# -c: compile without linking
# $<: represents first prerequisite of current target.
# -o $@: -o output $@ represents target of current rule.


clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

debug: CFLAGS += $(SANITIZE)
debug: re

.PHONY: all, clean, fclean, re, debug

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/08 15:19:20 by jqueijo-          #+#    #+#              #
#    Updated: 2024/07/11 11:52:37 by jqueijo-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC_PATH = ./srcs
OBJ_DIR = ./obj

SRC = $(SRC_PATH)/main.c \
	$(SRC_PATH)/init/init.c \
	$(SRC_PATH)/init/env.c \
	$(SRC_PATH)/init/cleanup.c \
	$(SRC_PATH)/utils/len_utils.c \
	$(SRC_PATH)/utils/str_utils.c \
	$(SRC_PATH)/utils/is_x.c \
	$(SRC_PATH)/utils/split_into_words.c \
	$(SRC_PATH)/processing/process_input.c \
	$(SRC_PATH)/processing/trim_input.c \
	$(SRC_PATH)/processing/tokenize_input.c \
	$(SRC_PATH)/processing/tokenize_refine_word.c \
	$(SRC_PATH)/processing/reorg_word.c \
	$(SRC_PATH)/processing/tokenize_smarter.c \
	$(SRC_PATH)/parsing/first_pars.c \
	$(SRC_PATH)/parsing/error_msg.c \

# OBJ = $(SRC:.c=.o)
OBJ = $(SRC:$(SRC_PATH)/%.c=$(OBJ_DIR)/%.o)

CC = cc
CFLAGS = -Wall -Werror -Wextra

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

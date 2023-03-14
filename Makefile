NAME	=	pbrain-gomoku-ai

CC	=	g++

WCC	=	x86_64-w64-mingw32-g++

GAME_PATH	=	game
	
GAME_NAME	=	piskvork.exe

GAME	=	$(GAME_PATH)/$(GAME_NAME)

GAME_AI	=	$(GAME_PATH)/pbrain-pela.exe

LOG_DIR	=	log

SRC_DIR	=	src

CFLAGS  = -Wall -Wextra -Werror -std=c++20 -O3 -pthread

CPPFLAGS  = -Wall -Wextra -Werror -std=c++20 -O3 -pthread

WCPPFLAGS  = -Wall -Wextra -Werror -std=c++20 -O3 -static -static-libgcc -static-libstdc++ -pthread

SRC_CONTENT = $(shell find $(SRC_DIR) -name "*.cpp")

OBJ_CONTENT = $(SRC_CONTENT:.cpp=.o)

all:	$(NAME)

$(NAME):	$(OBJ_CONTENT)
	$(CC) -o $(NAME) $(OBJ_CONTENT)

debug:	CFLAGS += -g3
debug:	CPPFLAGS += -g3
debug:	$(NAME)
	$(CC) -o $(NAME) $(OBJ_CONTENT) $(CFLAGS)

win:
	$(WCC) -o $(NAME).exe $(SRC_CONTENT) $(WCPPFLAGS)

exec:	win
	wine $(GAME) -p $(NAME).exe $(GAME_AI) -logpipe $(LOG_DIR)/ -timeturn 5
	wait
	./cleaning_wine_processes.sh

exec2:	win
	wine $(GAME) -p $(GAME_AI) $(NAME).exe -logpipe $(LOG_DIR)/ -timeturn 5
	wait
	./cleaning_wine_processes.sh

vs:	win
	wine $(GAME) -p $(NAME).exe zach/$(NAME).exe -timeturn 5
	wait
	./cleaning_wine_processes.sh

logs:
	@clear
	@echo "Logs:" && echo ""
	@cat $(LOG_DIR)/$(NAME).exe.log

clean:
	@rm -rf build
	@rm -rf dist
	@rm -rf $(NAME).spec
	@rm -rf $(LOG_DIR)/*
	@rm -rf $(SRC_DIR)/*.o
	@rm -rf vgcore.*

fclean:	clean
	@rm -rf $(NAME)
	@rm -rf $(NAME).exe

re:	fclean all

.PHONY: all clean fclean re exec win logs
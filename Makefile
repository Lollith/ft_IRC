NAME = ircserv
CXX = c++

SRC_PATHS = ./sources/

SRC = $(addprefix $(SRC_PATHS),\
	server.cpp\
	main.cpp)

OBJ		=	$(SRC:./sources/%.cpp=./build/%.o)
DEP		=	$(SRC:./sources/%.cpp=./build/%.d)
INC		=	-I ./includes/

CXXFLAGS = -MMD -Wall -Wextra -Werror -std=c++98 -fsanitize=address

build/%.o:	./sources/%.cpp
			mkdir -p build
			$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(NAME): $(OBJ)
		$(CXX) $(CXXFLAG) $(OBJ) -o $(NAME)


all	: $(NAME)

clean:
		rm -rf build

fclean: clean
		rm -f $(NAME)

re: fclean
	make -C .

.PHONY: all re clean fclean

-include $(DEP)

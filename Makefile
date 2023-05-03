NAME = ircserv
# CXX = c++
CXX = g++ #pour pouvoir utiliser lldb à enlever !! 

SRC_PATHS = ./sources/

SRC = $(addprefix $(SRC_PATHS),\
	server.cpp\
	s_chan_operation.cpp\
	s_messages.cpp\
	client.cpp\
	c_operator.cpp\
	message.cpp\
	channel.cpp\
	numerics_replies.cpp\
	main.cpp)

OBJ		=	$(SRC:./sources/%.cpp=./build/%.o)
DEP		=	$(SRC:./sources/%.cpp=./build/%.d)
INC		=	-I ./includes/

CXXFLAGS = -MMD -g3 -Wall -Wextra -Werror -std=c++98 
# CXXFLAGS += -fsanitize=address

build/%.o:	./sources/%.cpp
			mkdir -p build
			$(CXX) $(CXXFLAGS) -c $< -o $@ $(INC)

$(NAME): $(OBJ)
		$(CXX) $(CXXFLAG) $(OBJ) -o $(NAME)


all	: $(NAME)

clean:
		rm -rf build

fclean: clean
		rm -f $(NAME)

re: fclean
	make -C .

grind: $(NAME)
	valgrind --track-origins=yes --track-fds=yes --leak-check=full --show-leak-kinds=all ./ircserv 6667 coucou

.PHONY: all re clean fclean grind

-include $(DEP)

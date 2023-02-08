NAME		= ircserv

SRCS		= \
			  main.cpp \
			  srcs/baseserver.cpp \
			  srcs/ircserver.cpp \

HEADERS		= \
			  includes

BUILDDIR	= .build
OBJS		= $(SRCS:%.cpp=$(BUILDDIR)/%.o)
DEPS		= $(SRCS:%.cpp=$(BUILDDIR)/%.d)
CXX			= c++ -std=c++98
CXXWFLGS	= -Wall -Wextra -Werror -g3 #-fsanitize=address
DEPSFLAGS	= -MMD -MP
RM			= rm -Rf
MAKE		= make -C
MKDIR		= mkdir

all : $(NAME)

$(NAME) : $(OBJS)
		$(CXX) $(CXXWFLGS) -o $(NAME) $(OBJS)

bonus : $(NAME)

clean :
		-$(RM) $(BUILDDIR)

fclean : clean
		-$(RM) $(NAME)

re : fclean
		$(MAKE) ./ all

-include $(DEPS)

$(BUILDDIR)/%.o : %.cpp Makefile
		@mkdir -p $(@D)
		$(CXX) $(CXXWFLGS) $(DEPSFLAGS) -I$(HEADERS) -c $< -o $@

.PHONY: all clean fclean re bonus

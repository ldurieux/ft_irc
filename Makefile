NAME		= ircserv
BOT			= bot

SRCS		= \
			  main.cpp \
			  srcs/baseserver.cpp \
			  srcs/ircserver.cpp \

BOTSRCS		= \
			  bot.cpp \
			  srcs/baseclient.cpp \
			  srcs/ircclient.cpp \

HEADERS		= \
			  includes

BUILDDIR	= .build
OBJS		= $(SRCS:%.cpp=$(BUILDDIR)/%.o)
DEPS		= $(SRCS:%.cpp=$(BUILDDIR)/%.d)
BOTOBJS		= $(BOTSRCS:%.cpp=$(BUILDDIR)/%.o)
BOTDEPS		= $(BOTSRCS:%.cpp=$(BUILDDIR)/%.d)
CXX			= c++ -std=c++98
CXXWFLGS	= -Wall -Wextra -Werror -g3 #-fsanitize=address
DEPSFLAGS	= -MMD -MP
RM			= rm -Rf
MAKE		= make -C
MKDIR		= mkdir

all : $(NAME) $(BOT)

$(NAME) : $(OBJS)
		$(CXX) $(CXXWFLGS) -o $(NAME) $(OBJS)

$(BOT) : $(BOTOBJS)
		$(CXX) $(CXXWFLGS) -o $(BOT) $(BOTOBJS)

bonus : $(NAME)

clean :
		-$(RM) $(BUILDDIR)

fclean : clean
		-$(RM) $(NAME) $(BOT)

re : fclean
		$(MAKE) ./ all

-include $(DEPS)

$(BUILDDIR)/%.o : %.cpp Makefile
		@mkdir -p $(@D)
		$(CXX) $(CXXWFLGS) $(DEPSFLAGS) -I$(HEADERS) -c $< -o $@

.PHONY: all clean fclean re bonus

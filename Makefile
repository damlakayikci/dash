# Define the compiler
CC = gcc

# Define any compile-time flags
CFLAGS = -Wall -g

# Define any directories containing header files
INCLUDES = 

# Define any libraries to link into executable
LIBS =

# Define the source files
SRCS = dash.c find_command.c trim.c

# Define the object files
OBJS = $(SRCS:.c=.o)

# Define the executable file
MAIN = myshell

.PHONY: depend clean

all:    $(MAIN)
	@echo  Compiled $(MAIN) successfully.

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LIBS)

# This is a suffix replacement rule for building .o's from .c's
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

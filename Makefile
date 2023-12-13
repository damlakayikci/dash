# Define the compiler
CC = gcc-13

# Define any compile-time flags
CFLAGS = -Wall -g
# -Wall flag tells the compiler to enable all standard warning messages.
# It's a way to tell the compiler to be verbose about potential issues in your code. 
# Warnings can include things like uninitialized variables, usage of deprecated functions, and other potential problems 
# that aren't strictly errors but could lead to bugs.

#  -g flag adds debugging information to the generated output. 
# This information is used by debugging tools, like gdb, to help you debug your program. 
# With -g, the debugger can provide more detailed information about the state of your program, 
# such as variable values, function call stacks, and more.

# Define any directories containing header files
INCLUDES = -Iheaders

# Define any libraries to link into executable
LIBS =

# Define the source files
SRCS = dash.c helper.c execute.c alias.c 

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

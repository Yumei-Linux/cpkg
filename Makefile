# Variables
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lyaml

# Source code
SRCDIR = src
SRCS = $(wildcard $(SRCDIR)/*.c)

# Object files
OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/,$(notdir $(SRCS:.c=.o)))

# Header files
INCDIR = include
INCFLAGS = -I$(INCDIR)

# Executable name
EXECUTABLE = cpkg

# Rules
all: $(EXECUTABLE)

$(OBJDIR):
	mkdir -pv $(OBJDIR)

$(EXECUTABLE): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

# Clean files
clean:
	rm -f $(EXECUTABLE) $(OBJS)

# Utilities
run: all
	@clear
	@./$(EXECUTABLE)

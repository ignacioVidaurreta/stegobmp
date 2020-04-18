CC = gcc
CFLAGS = -pthread --std=c99 -pedantic -Wall
GREEN = \e[92m
NORMAL = \e[0m
FILES = ./*.c
EXEC_NAME = stegobmp
DEBUG_NAME = stegobmp_debug
.PHONY = clean
all:
	@echo "$(GREEN)Compiling ... $(NORMAL)"
	$(CC) $(CFLAGS) $(FILES) -o $(EXEC_NAME)
	@echo "$(GREEN)Done!$(NORMAL)"

sanitized:
	@echo "$(GREEN)Compiling with address sanitizer...$(NORMAL)"
	$(CC) $(CFLAGS) -fsanitize=address $(FILES) -o $(EXEC_NAME)
	@echo "$(GREEN)Done!$(NORMAL)"

debug:
	@echo "$(GREEN)Compiling in DEBUG mode ...$(NORMAL)"
	$(CC) -g $(CFLAGS) $(FILES) -o $(DEBUG_NAME)
	@echo "$(GREEN)Done!$(NORMAL)"


test: all
	@echo "WIP"

clean:
	@echo "$(GREEN)Cleaning up ...$(NORMAL)"
	rm -rf $(EXEC_NAME) $(DEBUG_NAME)
	@echo "$(GREEN)Done!$(NORMAL)"

## TODO: Missing rules: run and gdb. We need to figure out the whole 
## arguments first

CC = gcc
CFLAGS = -pthread --std=c99 -pedantic -Wall
FINAL_FLAGS = -lcrypto
GREEN = \e[92m
NORMAL = \e[0m
FILES = ./*.c ./steganography/*.c ./utils/*.c ./symmetric_cryptography/*.c
EXEC_NAME = stegobmp
DEBUG_NAME = stegobmp_debug
.PHONY = clean
all:
	@echo "$(GREEN)Compiling ... $(NORMAL)"
	@mkdir -p log
	$(CC) $(CFLAGS) $(FILES) -lm -o $(EXEC_NAME) $(FINAL_FLAGS)
	@echo "$(GREEN)Done!$(NORMAL)"

sanitized:
	@echo "$(GREEN)Compiling with address sanitizer...$(NORMAL)"
	$(CC) $(CFLAGS) -fsanitize=address $(FILES) -lm -o $(EXEC_NAME) $(FINAL_FLAGS)
	@echo "$(GREEN)Done!$(NORMAL)"


valgrind: debug
	valgrind --leak-check=full --show-leak-kinds=all --log-file="log/valgrind_output.txt" ./$(DEBUG_NAME) && cat log/valgrind_output.txt

debug:
	@echo "$(GREEN)Compiling in DEBUG mode ...$(NORMAL)"
	$(CC) -g $(CFLAGS) $(FILES) -lm -o $(DEBUG_NAME) $(FINAL_FLAGS)
	@echo "$(GREEN)Done!$(NORMAL)"


test:
	@echo "$(GREEN)Running Tests...$(NORMAL)"
	$(MAKE) -C tests/
	@echo "$(GREEN)Done$(NORMAL)"


clean:
	@echo "$(GREEN)Cleaning up ...$(NORMAL)"
	rm -rf $(EXEC_NAME) $(DEBUG_NAME)
	@echo "$(GREEN)Done!$(NORMAL)"

purge-logs:
	@echo "$(GREEN)Cleaning logs ...$(NORMAL)"
	rm -rf log
	@echo "$(GREEN)Done!$(NORMAL)"


## TODO: Missing rules: run and gdb. We need to figure out the whole 
## arguments first

CC = gcc
CFLAGS = -pthread -g
FINAL_FLAGS = -lcrypto -lm
BLUE = \e[34m
NORMAL = \e[0m
GREEN = \e[92m

all: 
	$(CC) $(CFLAGS) ./*.c ../bmp.c ../parser.c ../logging.c ../rc4.c ../utils/*.c ../steganography/* ../symmetric_cryptography/*.c $(FINAL_FLAGS)
	@echo "$(BLUE)"
	@./a.out
	@echo "$(NORMAL)" 
	@$(MAKE) clean

clean:
	@rm -rf a.out

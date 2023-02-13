CC = gcc
CFLAGS = 

all: decrypt

decrypt: decrypt.c
	$(CC) $(CFLAGS) -o decrypt decrypt.c aes.c libaesni/libaes_lin64.so -DAESNI

clean:
	rm -f decrypt
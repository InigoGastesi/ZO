#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <memory.h>
#include "aes.h"
#include "sha256.h"


// Enable CTR and CBC mode. Note this can be done before including aes.h or at compile-time.
#define CBC 1

#define IPAD 0x36
#define OPAD 0x5C
#define SHA256_INPUT_SIZE 64

#define MAXLENGTH 200



//Declare all functions
int test_encrypt_cbc(uint8_t* key, uint8_t* buf, int size_buf, uint8_t* iv);
int test_decrypt_cbc(uint8_t* key, uint8_t* buf, int size_buf, uint8_t* iv);
void phex(uint8_t* str, int len);
void write_hex_file(FILE* file, uint8_t* in, int size);
int read_hex_file(FILE* file, uint8_t* out);
int hex_to_int(char c);
int hex_to_ascii(char c, char d);
uint32_t hexdigit_value(uint8_t c);
void parse(uint32_t length, uint8_t *in, uint8_t *out);
void HMAC_SHA256(uint8_t* key, int size_k, uint8_t* m, int size_m, uint8_t* HMAC);


int main(int argc, char *argv[])
{
    	//Initialize variables
    	uint8_t* totalfile = malloc(MAXLENGTH * sizeof(uint8_t));
    	uint8_t key[AES_KEYLEN];
    	uint8_t* iv=malloc(AES_BLOCKLEN * sizeof(uint8_t));
	uint8_t* c = malloc(MAXLENGTH * sizeof(uint8_t));
	uint8_t* HMAC = malloc(SHA256_BLOCK_SIZE * sizeof(uint8_t));
	int total_size_m;
	int size_c;
	uint8_t* m = malloc(MAXLENGTH * sizeof(uint8_t));
	int size_m;
	uint8_t* HMAC2 = malloc(SHA256_BLOCK_SIZE * sizeof(uint8_t));
    	FILE *fd_in;
	    	
	if(argc != 3)
	{
		fprintf(stderr, "Usage: %s source_file key(hex)\n", argv[0]);
		return(0);
	}

	//Read input file .txt file and add padding
	if ((fd_in = fopen(argv[1],"r"))== 0)
	{
		fprintf(stderr, "Error opening output file");
		return(0);
	}
	total_size_m = read_hex_file(fd_in, totalfile);
	fclose(fd_in);
	//Read hex key input
	parse(AES_KEYLEN, argv[2], key);


    
	//Interpret source file iv + c + tag
	size_c = total_size_m - AES_BLOCKLEN - SHA256_BLOCK_SIZE;
	memcpy(iv,totalfile, AES_BLOCKLEN);
	memcpy(c, totalfile + AES_BLOCKLEN, size_c);
	memcpy(HMAC, totalfile + AES_BLOCKLEN+size_c, SHA256_BLOCK_SIZE);
	printf("Received iv: ");  phex(iv, AES_BLOCKLEN);
	printf("Received Cipher: "); phex(c, size_c);

	//Decrypt
	memcpy(m, c, size_c);
	size_m = test_decrypt_cbc(key, m, size_c,iv);

	//Calculate MAC of decrypted message
	HMAC_SHA256(key, AES_KEYLEN, m, size_m, HMAC2);
	printf("Received HMAC: "); phex(HMAC, SHA256_BLOCK_SIZE);
	printf("Calculated HMAC: "); phex(HMAC2, SHA256_BLOCK_SIZE);
	if (memcmp(HMAC, HMAC2, SHA256_BLOCK_SIZE) != 0)
		printf("Careful! Message has been modified.\n");
	else
	{
		printf("Message is correct!\nDecryption key is:\n");
		phex(m, size_m);
	}
}

void HMAC_SHA256(uint8_t* key, int size_k, uint8_t* m, int size_m, uint8_t* HMAC)
{
    //Primero debemos calcular los padded key para calcular el hmac

    uint8_t padded_key[SHA256_INPUT_SIZE]; //512 bits de clave
    size_m = 32;
    if(size_k == SHA256_INPUT_SIZE){
        //Usamos la clave como tal
        memcpy(padded_key,key,SHA256_INPUT_SIZE * sizeof(uint8_t));

    }
    else if(size_k < SHA256_INPUT_SIZE){
        //Rellenamos con ceros

        memset(padded_key,0, SHA256_INPUT_SIZE * sizeof(uint8_t));
        memcpy(padded_key, key, size_k * sizeof(uint8_t));
    }
    else{
        //Hasheamos la clave y llenamos con cero

        SHA256_CTX  ctx_key_hash;

        BYTE key_hash[SHA256_BLOCK_SIZE];
        sha256_init(&ctx_key_hash);
        sha256_update(&ctx_key_hash,key, strlen(key));
        sha256_final(&ctx_key_hash,key_hash);

        memset(padded_key,0, SHA256_INPUT_SIZE * sizeof(uint8_t));
        memcpy(padded_key, key_hash, SHA256_BLOCK_SIZE * sizeof(uint8_t));
    }


    //Una vez creada el padded key, comenzamos a calcular los xor con los pad correspondientes


    // ###### padded_key xor ipad #############

    uint8_t padded_xor_ipad[SHA256_INPUT_SIZE];
    int i;

    memcpy(padded_xor_ipad,padded_key, SHA256_INPUT_SIZE * sizeof (uint8_t));

    for (int i = 0; i < SHA256_INPUT_SIZE; i++)
    {
        padded_xor_ipad[i] = padded_xor_ipad[i] ^ IPAD;
    }

    //#####################

    // ###### padded_key xor opad #############

    uint8_t padded_xor_opad[SHA256_INPUT_SIZE];

    memcpy(padded_xor_opad,padded_key, SHA256_INPUT_SIZE * sizeof (uint8_t));

    for (int i = 0; i < SHA256_INPUT_SIZE; i++)
    {
        padded_xor_opad[i] = padded_xor_opad[i] ^ OPAD;
    }

    //#####################


    //Una vez calculados los valores, empezamos por calcular el hash del padded_xor_ipad concatenado con el mensaje


    uint8_t * key_xor_mensaje = malloc((SHA256_INPUT_SIZE + size_m) * sizeof(uint8_t));

    memcpy(key_xor_mensaje,padded_xor_ipad,SHA256_INPUT_SIZE );
    memcpy(key_xor_mensaje + SHA256_INPUT_SIZE,m,size_m);


    SHA256_CTX ctx_hash1;
    BYTE hash1[SHA256_BLOCK_SIZE];

    sha256_init(&ctx_hash1);
    sha256_update(&ctx_hash1, key_xor_mensaje, SHA256_INPUT_SIZE + size_m);
    sha256_final(&ctx_hash1, hash1);

    free(key_xor_mensaje);
    
    

    //Ahora vamos a calcular hash2

    uint8_t text_hash2[SHA256_INPUT_SIZE + SHA256_BLOCK_SIZE];

    memcpy(text_hash2,padded_xor_opad, SHA256_INPUT_SIZE);

    memcpy(text_hash2 + SHA256_INPUT_SIZE,hash1, SHA256_BLOCK_SIZE);


    SHA256_CTX ctx_hash2;
    BYTE hash2[SHA256_BLOCK_SIZE];

    sha256_init(&ctx_hash2);
    sha256_update(&ctx_hash2,text_hash2, SHA256_INPUT_SIZE + SHA256_BLOCK_SIZE);
    sha256_final(&ctx_hash2,hash2);

    //Copiamos el resultado a la variable
    memcpy(HMAC,hash2, SHA256_BLOCK_SIZE * sizeof(uint8_t));



}


int test_decrypt_cbc(uint8_t* key, uint8_t* buf, int size_buf, uint8_t* iv)
{
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CBC_decrypt_buffer(&ctx, buf, size_buf);
    size_buf -= buf[size_buf-1];
    return size_buf;
}

int test_encrypt_cbc(uint8_t* key, uint8_t* buf, int size_buf, uint8_t* iv)
{
    uint8_t l_lastblock = size_buf % AES_BLOCKLEN;
    struct AES_ctx ctx;
    if (l_lastblock < AES_BLOCKLEN) //add padding
    {
        memset(&buf[size_buf + l_lastblock], AES_BLOCKLEN - l_lastblock, AES_BLOCKLEN - l_lastblock);
    }
    size_buf += AES_BLOCKLEN;
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CBC_encrypt_buffer(&ctx, buf, size_buf);
    return size_buf;
}

void phex(uint8_t* str, int len)
{
    unsigned char i;
    for (i = 0; i < len; ++i)
        printf("%.2x", str[i]);
    printf("\n");
}


void write_hex_file(FILE* file, uint8_t* in, int size)
{
    if (!feof(file))
    {
        for (int i = 0; i < size; i++)
        {
            fprintf(file, "%.2x", in[i]);
        }
    }
}

int read_hex_file(FILE* file, uint8_t* out)
{
    char ch0, ch1;
    int size = 0;
    ch0 = fgetc(file);
    ch1 = fgetc(file);
    while (ch0 != EOF)
    {
        out[size] = hex_to_ascii(ch0, ch1);
        size++;
        ch0 = fgetc(file);
        ch1 = fgetc(file);
    }
    return size;
}

int hex_to_int(char c)
{
    if (c >= 48 && c <= 57)
        return c - 48;
    else if (c >= 97 && c <= 102)
        return c - 87;
    else
        return c - 55;
}

int hex_to_ascii(char c, char d)
{
    int high = hex_to_int(c) * 16;
    int low = hex_to_int(d);
    return high + low;
}

uint32_t hexdigit_value(uint8_t c)
{
    int nibble = -1;
    if(('0' <= c) && (c <= '9')) 
        nibble = c-'0';
    if(('a' <= c) && (c <= 'f'))
        nibble = c-'a' + 10;
    if(('A' <= c) && (c <= 'F'))
        nibble = c-'A' + 10;
    return nibble;
}

void parse(uint32_t length, uint8_t *in, uint8_t *out)
{
    uint32_t i, shift, idx;
    uint8_t nibble, c;
    uint32_t len = strlen(in);
    if(length >(len/2))
        length = (len/2);
    memset(out, 0, length);
    for(i = 0;i < length * 2;i++)
    {
        shift = 4 - 4 * (i & 1);
        idx = i;//len-1-i;
        c = in[idx];
        nibble = hexdigit_value(c);
        out[i/2] |= nibble << shift;
    }
}

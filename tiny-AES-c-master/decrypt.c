#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "aes.h"

#define BLOCK_SIZE 16
#define AES_KEY_LENGTH 32
#define KEY_LENGTH 32 
#define RANGE 256

uint8_t aux[BLOCK_SIZE];

uint8_t iv[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

void print_hex(uint8_t *buf, uint32_t c)
{
    uint32_t i;

    for(i = 0; i < c; i++)
    {
        printf("%02x", buf[i]);
    }
    printf("\n");
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

uint32_t parse_mask(uint8_t *in, int64_t **key_mask){
    
    uint64_t i = 0;                                                                                                                             
    uint32_t n_masks = 0;                                                                                                                      
    char *end_ptr;                                                                                                                         
    uint8_t *in_aux = strdup(in);                                                                                                     
    uint8_t *pt = strtok (in_aux,"_");                                                                                                    
    while (pt != NULL) 
    {                                                                                                                   
        if(strtol(pt, &end_ptr, 10) == -1)
        {
            n_masks = 0;
            return(n_masks);
        }
        n_masks++;                                                                                                                        
        pt = strtok (NULL, "_");                                                                                                           
    }                                                                                                                                      
    *key_mask = malloc(n_masks * sizeof(int64_t));                                                                                               
                                                                                                                                           
    pt = strtok (in, "_");                                                                                                              
    while (pt != NULL) 
    {                                                                                                                   
        (*key_mask)[i++] = strtol(pt, &end_ptr, 10);                                                                                            
        pt = strtok (NULL, "_");                                                                                                           
    }                                                                                                                                      
    return(n_masks);                                                                                                                      
}

int search(int64_t n_key_mask, int64_t *key_mask, int64_t n_plaintext_mask, int64_t *plaintext_mask, uint8_t *key, uint8_t *plain_text, uint8_t *cypher_text)
{
    struct AES_ctx ctx;
    uint8_t *in = malloc(sizeof(uint8_t)*BLOCK_SIZE);
    uint8_t *out = malloc(sizeof(uint8_t)*BLOCK_SIZE);
    for(int i=0; i<256; i++){
        key[key_mask[0]]=i;
        for(int j=0; j<256; j++){
            key[key_mask[1]]=j;
            for(int k=0; k<256; k++){
                key[key_mask[2]]=k;
                for(int w=0; w<256; w++){
                    key[key_mask[3]]=w;
                    printf("0x%02x 0x%02x 0x%02x 0x%02x\n", key[key_mask[0]], key[key_mask[1]], key[key_mask[2]], key[key_mask[3]]);
                    memcpy(in, cypher_text, BLOCK_SIZE);
                    memcpy(out, plain_text, BLOCK_SIZE);
                    AES_init_ctx_iv(&ctx, key, iv);
                    AES_CBC_decrypt_buffer(&ctx, in, BLOCK_SIZE);
                    if (0 == memcmp((char*) out, (char*) in, BLOCK_SIZE)) {
                        printf("SUCCESS! KEY:");
                        print_hex(key, KEY_LENGTH);
                        return(0);
                    }
                }
            }
        }
    }
    return(0);
}

int main(int argc, char *argv[])
{

	int64_t n_key_mask;
	int64_t n_plaintext_mask;
	uint8_t key[AES_KEY_LENGTH];
	uint8_t plain_text[BLOCK_SIZE];
	uint8_t cypher_text[BLOCK_SIZE];
	int64_t *key_mask;
	int64_t *plaintext_mask;
	uint64_t n_threads, n_threads_sys;
	char *end_ptr;
	
	if(argc != 6 && argc != 7)
	{
		fprintf(stderr, "Usage: %s key key_mask plaintext plaintext_mask cyphertext\n", argv[0]);
		return(0);
	}
	if(argc == 6)
		n_threads = 1;
	else
	{
		n_threads_sys = sysconf(_SC_NPROCESSORS_ONLN);
		n_threads = strtol(argv[6], &end_ptr, 10);
		if(n_threads > n_threads_sys)
	    		n_threads = n_threads_sys;
	}

    parse(AES_KEY_LENGTH, argv[1], key);
    n_key_mask = parse_mask(argv[2], &key_mask);
    parse(BLOCK_SIZE, argv[3], plain_text);
    n_plaintext_mask = parse_mask(argv[4], &plaintext_mask);
    parse(BLOCK_SIZE, argv[5], cypher_text);
    printf("Key: ");
    print_hex(key, AES_KEY_LENGTH);
    printf("Plain text: ");
    print_hex(plain_text, BLOCK_SIZE);
    printf("Cypher text: ");
    print_hex(cypher_text, BLOCK_SIZE);
    printf("Key mask length: %ld\n", n_key_mask);
    printf("Plaintext mask length: %ld\n", n_plaintext_mask);

    search(n_key_mask, key_mask, n_plaintext_mask, plaintext_mask, key, plain_text, cypher_text);
	
}








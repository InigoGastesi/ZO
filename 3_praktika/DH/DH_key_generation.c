#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <memory.h>
#include "dhexchange.h"

static void _print_key(const char* name, const DH_KEY key) 
{
	int i;

	printf("%s=\t", name);
	for (i = 0; i< DH_KEY_LENGTH; i++)
	{
		printf("%02x", key[i]);
	}
	printf("\n");
}

int main(void)
{
	DH_KEY alice_public={0xcc,0xb1,0xcf,0x31,0x6e,0xf3,0xea,0x88,0x68,0x48,0x14,0x72,0xe8,0x38,0x5a,0x7e};
	DH_KEY alice_private;
	DH_KEY bob_private={0x45,0x45,0x1f,0xae,0x9b,0x3a,0x9d,0x5f,0x46,0x3c,0xcb,0x75,0x63,0x03,0x55,0x7c};
	DH_KEY bob_public={0x87,0x0d,0x72,0x53,0xbe,0xf3,0xe1,0x7b,0xe1,0x2d,0x97,0x38,0x93,0x75,0x31,0xdc};
	DH_KEY alice_secret; 
	DH_KEY bob_secret;

	time_t seed;
	time(&seed);
	srand((unsigned int)seed);

	// /*Alice generate her private key and public key */
	// DH_generate_key_pair(alice_public, alice_private);

	// /*Bob generate his private key and public key */
	// DH_generate_key_pair(bob_public, bob_private);

	// /*Bob send his public key to Alice, Alice generate the secret key */
	DH_generate_key_secret(alice_secret, alice_private, bob_public);

	/*Alice send her public key to Bob, Bob generate the secret key */
	// DH_generate_key_secret(bob_secret, bob_private, alice_public);

	_print_key("alice_private", alice_private);
	_print_key("alice_public", alice_public);
	_print_key("bob_private", bob_private);
	_print_key("bob_public", bob_public);
	// _print_key("alice_secret", alice_secret);
	// _print_key("bob_secret", bob_secret);

	if (memcmp(alice_secret, bob_secret, DH_KEY_LENGTH) != 0) 
	{
		printf("ERROR!\n");
		return 1;
	}
	return 0;
}

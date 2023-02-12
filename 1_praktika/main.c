#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void hex2int(char *message, int *decMessage);
void count_spaces(int *c1, int *c2, int tarteak[14][1000], int c1index, int size);
void decryptMessage(unsigned char *decKey, char *chyper, int len);

int main(){

    /*--------- Messages to decrypt ---------*/
    char c0[1000] = "1a1617451a411517490b061b0f08535404044e17450c1c45326222420a00340006544816170b54030b55020d530046";
    char c1[1000] = "184f184f0a081a000016071a00010017090b00100416010054530e060c52301b0c000a131304430e0a0640";
    char c2[1000] = "09001a5248041b04490a4f060b07550601115953150c010007000604134f2b4f01481a0417115348";
    char c3[1000] = "7926114506151f1159461b1d0b025454010850120617014542104b08104c35061a4e48201b00520f0c1400170e";
    char c4[1000] = "0c3c5547071713174e0b0a1b1a445018090b5353110c5216044505015904685a5542010d1a0c4f084f1a0044430d0a005200000007171d54124b";
    char c5[1000] = "1a001c4e480c1f0b490808551e1645070d0b5400450252071d474b020b4f2e1b1d00010f560659040a070d1649190d4b";
    char c6[1000] = "0d0710003d32560c5346211a4e550000091747161143140a06001f040b473c1b1044480002114105040640";
    char c7[1000] = "1c02054c0718130000080a12020d47110606455306021c45174f06150b4f34060645480f131157091d1e4e1745171d1749071c4d";
    char c8[1000] = "103b55530d02031749121655070a541104094914000d1100544918450c4e3d0a07551c081a0c5a030b5b";
    char c9[1000] = "120a1050010f1145410a03551d0b46001f0452164516020115540e0159492a4f14441e0805004448";
    char c10[1000] = "793b024f48151f0845461f140a444907480b4f074510170601520e4959552a0a5541480f1312000d0a0c4e0141170045541a08065c";
    char c11[1000] = "163b55530d020317491216551e1645070d0b54004502520b11574b161c54790013000b0917094c0301120b170e";
    char c12[1000] = "100021000c04000c43031c550d054e54010b43010002010054530e060c52301b0c001e141a0b45140e17070849000100535d";
    char c13[1000] = "793b1d451a04560c53460e550d1d42111a4553160616000c00594b161249350306001b0919175407081040";

    char *cypherMesages[14] = {c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13};
    int decMessages[14][1000], tarteak[14][1000], decKey[1000], size, space;
    unsigned char key_raw[1000];

    // Convert char chypher messages from hexadeciman to integer
    for (int i = 0; i < 14; i++){
        hex2int(cypherMesages[i], decMessages[i]);
    }

    // Compute spaces
    for (int i = 0; i < 14; i++) {
        for (int j = 0; j < 13; j++) {
            // Take the smaller size
            size = (strlen(cypherMesages[i]) < strlen(cypherMesages[j])) ? strlen(cypherMesages[i]) : strlen(cypherMesages[j]);
            count_spaces(decMessages[i], decMessages[j], tarteak, i, size);
        }
    }

    // Print computed spaces 
    printf("Tarte kopurua:\n");
    for (int i = 0; i < 14; i++){
        printf(" %d --> ", i);

        for (int j = 0; j < 100; j++){
            printf("%02d ", tarteak[i][j]);
        }

        printf("\n");
    }
    printf("\n");
    
    // Apply the algorithm to find the key
    space = 32;
    for (int i = 0; i < 14; i++){
        for (int j = 0; j < 100; j++){

            if (tarteak[i][j] > 5) {
                key_raw[j] = (unsigned char)space ^ decMessages[i][j];
            }
        }
    } 

    // Print RAW key
    printf("Aurkitutako gakoa: ");
    for (int i = 0; i < 100; i++){
        printf("%c", key_raw[i]);
    } 
    printf("\n\n");

    // Print key
    char key[1000] = "You have found the secret key You have found the secret key You have found the secret key You have found the secret key ";
    printf("Gakoa: %s\n\n", key);

    // Decrypt all messages
    printf("Mezu deszifratuak:\n");
    for(int i = 0; i < 14; i++){
        printf(" %d --> ", i);
        decryptMessage(key, cypherMesages[i], strlen(cypherMesages[i])/2);
    }
}

/* Convert hexadecimal coded char array to integers */
void hex2int(char *hex_message, int *int_message){
    char *hex = malloc(2*sizeof(char));

    for(int i = 0; i < strlen(hex_message); i += 2){
        hex[0] = hex_message[i];
        hex[1] = hex_message[i + 1];
        int_message[i/2] = (int)strtol(hex, NULL, 16);
    }
}

/* Compute number of spaces */
void count_spaces(int *char_1, int *char_2, int spaces[14][1000], int index, int size){
    int mayus;

    for (int i = 0; i < size; i++){
        mayus = char_1[i] ^ char_2[i];

        if (mayus > 64 && mayus < 91) {
            spaces[index][i] += 1;
        }
    }
}

/* Decrypt message */
void decryptMessage(unsigned char *key, char *chyper_hex, int len){
    int chyper_int[1000];
    char decrypted_message[1000];

    hex2int(chyper_hex, chyper_int);

    for (int i = 0; i < len; i++){
        decrypted_message[i] = (unsigned char)((int)key[i] ^ chyper_int[i]);
        printf("%c", decrypted_message[i]);
    }
    
    printf("\n");
}
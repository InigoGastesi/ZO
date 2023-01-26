#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void print_string (unsigned char a[], int len);

int main() {
    unsigned char cipher[1000] = "Pda Ywaown yeldan ywj xa awoehu xnkgaj araj ej w yeldanpatp-kjhu oyajwnek. Psk oepqwpekjo ywj xa ykjoezanaz: 1) wj wppwygan gjkso (kn cqaooao) pdwp okia oknp kb oeilha oqxopepqpekj yeldan dwo xaaj qoaz, xqp jkp olayebeywhhu pdwp ep eo w Ywaown oydaia; wjz 2) wj wppwygan gjkso pdwp w Ywaown yeldan eo ej qoa, xqp zkao jkp gjks pda odebp rwhqa. Ej pda benop ywoa, pda yeldan ywj xa xnkgaj qoejc pda owia paydjemqao wo bkn w cajanwh oeilha oqxopepqpekj yeldan, oqyd wo bnamqajyu wjwhuoeo kn lwppanj sknzo. Sdeha okhrejc, ep eo hegahu pdwp wj wppwygan sehh mqeyghu jkpeya pda nacqhwnepu ej pda okhqpekj wjz zazqya pdwp w Ywaown yeldan eo pda olayebey whcknepdi ailhkuaz.Ej pda oaykjz ejopwjya, xnawgejc pda oydaia eo araj ikna opnwecdpbknswnz. Oejya pdana wna kjhu w heiepaz jqixan kb lkooexha odebpo (26 ej Ajcheod), pdau ywj awyd xa paopaz ej pqnj ej w xnqpa bknya wppwyg. ";
    unsigned char *line = NULL;
    float scores[25] = {0.0};
    float max = 0.0;
    int max_index = 0;
    int len = strlen(cipher);
    int word_index = 0;
    int size = 10;

    unsigned int c, message_score;
    unsigned char decrypted_message[25][1000], dictionary[22][10], c_shifted;
    ssize_t read;
    FILE *f;

    /* Dictionary of words to verify message*/
    f = fopen("dictionary.txt", "r");
    while((fgets(dictionary[word_index], 10, f))){
        dictionary[word_index][strlen(dictionary[word_index]) - 1] = ' ';
        dictionary[word_index][strlen(dictionary[word_index])] = '\0';
        word_index++;
    }

    /* Decrypt messages */
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < len; j++) {
            c = (unsigned int)tolower(cipher[j]);
            
            if ((unsigned int)c > 96 && (unsigned int)c < 123) {
                c_shifted = (unsigned char)c + i;
            
                if (c_shifted > 122) {
                    c_shifted = (unsigned char)(96 + (c_shifted - 122));
                    decrypted_message[i][j] = c_shifted;
                }

                decrypted_message[i][j] = c_shifted;
            }

            else {
                decrypted_message[i][j] = c;
            }
        }

        /* Compute accuracy (correctness) of decrypted message */
        /* Algorithm takes into account lenght of words in dictionary */
        message_score = 0;
        for (word_index = 0; word_index < 22; word_index++) {

            if (strstr(decrypted_message[i], dictionary[word_index]) != NULL) {
                message_score += strlen(dictionary[word_index]);
            }
        }
        scores[i] = (float)message_score / 22;
    }

    /* Find message with max accuracy score */
    for (int i = 1; i < 26; i++) {
        if (scores[i] > max) {
            max = scores[i];
            max_index = i;
        }
    }

    print_string(decrypted_message[max_index], len);
    return 0;
}

void print_string (unsigned char a[], int len) {
    for (int i=0; i<len; i++) {
        printf("%c",a[i]);
    }
    printf("\n");
}
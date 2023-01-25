#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
void print_string (unsigned char a[], int len);
int main() {
    unsigned char cipher[1000]="Pda Ywaown yeldan ywj xa awoehu xnkgaj araj ej w yeldanpatp-kjhu oyajwnek. Psk oepqwpekjo ywj xa ykjoezanaz: 1) wj wppwygan gjkso (kn cqaooao) pdwp okia oknp kb oeilha oqxopepqpekj yeldan dwo xaaj qoaz, xqp jkp olayebeywhhu pdwp ep eo w Ywaown oydaia; wjz 2) wj wppwygan gjkso pdwp w Ywaown yeldan eo ej qoa, xqp zkao jkp gjks pda odebp rwhqa. Ej pda benop ywoa, pda yeldan ywj xa xnkgaj qoejc pda owia paydjemqao wo bkn w cajanwh oeilha oqxopepqpekj yeldan, oqyd wo bnamqajyu wjwhuoeo kn lwppanj sknzo. Sdeha okhrejc, ep eo hegahu pdwp wj wppwygan sehh mqeyghu jkpeya pda nacqhwnepu ej pda okhqpekj wjz zazqya pdwp w Ywaown yeldan eo pda olayebey whcknepdi ailhkuaz.Ej pda oaykjz ejopwjya, xnawgejc pda oydaia eo araj ikna opnwecdpbknswnz. Oejya pdana wna kjhu w heiepaz jqixan kb lkooexha odebpo (26 ej Ajcheod), pdau ywj awyd xa paopaz ej pqnj ej w xnqpa bknya wppwyg. ";
    int len=strlen(cipher);
    print_string(cipher,len);
    //Rellenar el código aquí
    unsigned int frecuency[26];
    unsigned char message[1000];
    unsigned int c, offset;
    unsigned char shifted;
    char dictionary[25][10];
    unsigned char *line = NULL;
    int size = 10;
    int index=0;
    ssize_t read;
    FILE *f;
    char *token;
    f = fopen("dictionary.txt", "r");
    while((fgets(dictionary[index], 10, f))){
        dictionary[index][strlen(dictionary[index])-1]='\0';
        index++;
    }
    for(int i = 0; i < 26; i++){
        for(int j=0; j < len; j++){
            c = (unsigned int)tolower(cipher[j]);
            //printf("%d\n", c);
            
            if((unsigned int)c > 96 && (unsigned int)c < 123){
                shifted = (unsigned char)c+i;
                message[j] = shifted;
                //frecuency[(unsigned int)c+i-97]+=1;
            
                if(shifted > 122){
                    offset = shifted - 122;
                    shifted = (unsigned char)(96+offset);
                    message[j] = shifted;
                }
            }
            else{
                message[j] = c;
            }
        }
        print_string(message, len);
    }
    return 0;
}
void print_string (unsigned char a[], int len)
{
    for (int i=0; i<len; i++)
    {
    printf("%c",a[i]);
    }
    printf("\n");
}
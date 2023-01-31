#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void char2dec(char *message, int *decMessage);
void countTarteak(int *c1, int *c2, int tarteak[14][1000], int c1index, int size);

int main(){
    char c0[1000]="1a1617451a411517490b061b0f08535404044e17450c1c45326222420a00340006544816170b54030b55020d530046";
    char c1[1000]="184f184f0a081a000016071a00010017090b00100416010054530e060c52301b0c000a131304430e0a0640";
    char c2[1000]="09001a5248041b04490a4f060b07550601115953150c010007000604134f2b4f01481a0417115348";
    char c3[1000]="7926114506151f1159461b1d0b025454010850120617014542104b08104c35061a4e48201b00520f0c1400170e";
    char c4[1000]="0c3c5547071713174e0b0a1b1a445018090b5353110c5216044505015904685a5542010d1a0c4f084f1a0044430d0a005200000007171d54124b";
    char c5[1000]="1a001c4e480c1f0b490808551e1645070d0b5400450252071d474b020b4f2e1b1d00010f560659040a070d1649190d4b";
    char c6[1000]="0d0710003d32560c5346211a4e550000091747161143140a06001f040b473c1b1044480002114105040640";
    char c7[1000]="1c02054c0718130000080a12020d47110606455306021c45174f06150b4f34060645480f131157091d1e4e1745171d1749071c4d";
    char c8[1000]="103b55530d02031749121655070a541104094914000d1100544918450c4e3d0a07551c081a0c5a030b5b";
    char c9[1000]="120a1050010f1145410a03551d0b46001f0452164516020115540e0159492a4f14441e0805004448";
    char c10[1000]="793b024f48151f0845461f140a444907480b4f074510170601520e4959552a0a5541480f1312000d0a0c4e0141170045541a08065c";
    char c11[1000]="163b55530d020317491216551e1645070d0b54004502520b11574b161c54790013000b0917094c0301120b170e";
    char c12[1000]="100021000c04000c43031c550d054e54010b43010002010054530e060c52301b0c001e141a0b45140e17070849000100535d";
    char c13[1000]="793b1d451a04560c53460e550d1d42111a4553160616000c00594b161249350306001b0919175407081040";
    unsigned char k[1000];
    char *cypherMesages[14]={c0,c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13};
    int decMessages[14][1000];
    int tarteak[14][1000];

    for(int i = 0; i < 14; i++){
        char2dec(cypherMesages[i], decMessages[i]);
    }

    for(int i = 0; i < 14; i++){
        for(int j = 0; j < 13; j++){
            int size;
            if (strlen(cypherMesages[i]) < strlen(cypherMesages[j])){
                size = strlen(cypherMesages[i]);
            }
            else{
                size = strlen(cypherMesages[j]);
            }
            countTarteak(decMessages[i], decMessages[j], tarteak, i, size);
        }
    }
    int space = 32;
    for(int i = 0; i < 14; i++){
        for(int j = 0; j < 100; j++){
            if(tarteak[i][j]>7){
                k[j]=(unsigned char)tarteak[i][j]^decMessages[i][j];
            }
            else{
                k[j]=' ';
            }
            // printf("%d ", tarteak[i][j]);
        }
        // printf("\n");
    } 
    for(int i = 0; i < 100; i++){
        printf("%c ",k[i]);
    } 
    printf("\n");
}

void char2dec(char *message, int *decMessage){
    char *lag=malloc(2*sizeof(char));
    for(int i = 0; i < strlen(message); i+=2){
        lag[0]=message[i];
        lag[1]=message[i+1];
        decMessage[i/2]=(int)strtol(lag, NULL, 16);
    }
}

void countTarteak(int *c1, int *c2, int tarteak[14][1000], int c1index, int size){
    int mayus;

    for (int i = 0; i < size; i++){
        mayus = c1[i] ^ c2[i];
        if(mayus > 64 && mayus < 91){
            tarteak[c1index][i]+=1;
        }
    }
}
#include <stdio.h>
#include <stdbool.h> 
#include <stdlib.h> 

bool isIsomorphic(char * s, char * t){
    char dictionary[256] = {0};
    for(int i = 0;s[i];i++){
        printf("\n");
        for(int j = 0;j<256;j++){
            printf("%c: %c\n",j,dictionary[j]);
        }
        if(dictionary[s[i]]){
            if(dictionary[s[i]] != t[i]){
                return false;
            }
        }
        else{
            if(dictionary[128+t[i]]){
                return false;
            }
            else{
                dictionary[s[i]] = t[i];
                dictionary[128+t[i]] = 1;
            }
        }
    }
    return true;
}

int main(int argc, char **argv){
    return 0;
}
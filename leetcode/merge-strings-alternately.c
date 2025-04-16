#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * mergeAlternately(char * word1, char * word2){
	char * result = malloc(sizeof(char)* (strlen(word1)+strlen(word2)));
	int counter = 0;
	while((*word1 != '\0') && (*word2 != '\0')){
		if(counter%2){
			result[counter] = *word2;
			word2++;
		}
		else{
			result[counter] = *word1;
			word1++;
		}
		counter++;
	}
	
	while(*word1 != '\0'){
		result[counter] = *word1;
		word1++;
		counter++;
	}
	while(*word2 != '\0'){
		result[counter] = *word2;
		word2++;
		counter++;
	}
	return result;  
}

int main(int argc, char **argv){
	printf("%s\n", mergeAlternately("abc","pdf"));
	printf("%s\n", mergeAlternately("abc","pdxzclkf"));
	printf("%s\n", mergeAlternately("abawdawdc","pdf"));
	return 0;
}


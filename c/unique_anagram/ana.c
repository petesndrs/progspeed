#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int factorial(int n){
    int fact = 1;
	int i;
    for (i=1; i < n+1; ++i){
        fact = fact * i;
	}
    return fact;
}

void anagram(const char* string, char** output, int output_index){
	int i;
	int length = strlen(string);
    for (i=0; i<length; ++i){

        char pick = string[i];
        char* remainder = (char*)malloc(length * sizeof(char)); 
		strncpy(remainder, string, i);
		remainder[i] = '\0';
		strncat(remainder, &string[i+1], length - i - 1);
		
        int j;
        for (j=0; j<factorial(length - 1); ++j){
            strncat(output[ output_index + j], &pick, 1);
		}
		
        anagram((const char*)remainder, output, output_index);

        output_index += factorial(length - 1);
		
		free(remainder);
	}
}

int deduplicate(char** output, int number){
    int count = 0;
	int i;
    for (i=0; i<number; ++i){
        if (*output[i] != '\0'){
            count++;
			int j;
            for (j=i+1; j<number; ++j){
                if (strcmp(output[i], output[j]) == 0){
                    *output[j] = '\0';       
				}
            }
        }
    }		
    return count;
}

int main() {
    const char* a = "pineapple";

    int number = strlen(a);
    printf("Number of letters: %d\n",number);

    int anagrams = factorial(number);
    printf("Number of anagrams: %d\n",anagrams);

    char** output = (char**)malloc(sizeof(char*) * anagrams);
	for (int i=0; i<anagrams; ++i){
		output[i] = (char*)malloc(sizeof(char) * (number+1));
		*output[i] = '\0';
	}
	
    anagram(a, output, 0);

    printf("First anagram: %s\n",output[0]);
    printf("Last anagram: %s\n",output[anagrams-1]);
    int count = deduplicate(output, anagrams);
    printf("Number of unique anagrams: %d\n",count);
}
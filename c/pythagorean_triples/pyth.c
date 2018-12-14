

#include <stdio.h>
#include <stdbool.h>

struct triple {
    int a;
    int b;
    int c;
    bool valid;
};


int search(int a, int b, int start, int end, int inc){

    int c;
    for (c = start; c <= end; c += inc){
        if (c*c == a*a + b*b) return c;
    }

    return 0;
}

void remove_multiples(struct triple* found, int num){
    int i, j, k;
    for (i = 0; i < num; ++i){
        if (found[i].valid) {
            k = 2;
            for (j = i+1; j < num; ++j){
                if (found[j].valid) {
                    if ( found[i].a * k == found[j].a &&
                         found[i].b * k == found[j].b &&
                         found[i].c * k == found[j].c ){
                      found[j].valid = false;
                      ++k;
                    }
                }
            }
        }
    }
}

int main(int argc, char* argv[]){

    const int MIN = 1;
    const int MAX = 2000;
    const int MAX_TRIPLES = 4000;
    struct triple found[MAX_TRIPLES];

    // c**2 = a**2 + b**2

    int a, b, c;
    int found_triples = 0;
    for (a = MIN; a < MAX; ++a){
        for (b = a; b < MAX; ++b){ 
            c = search (a, b, b+1, MAX, 1);
            
            if (c) {
                //printf("%d, %d, %d, %d\n", found_triples, a, b, c);
                found[found_triples].a = a;
                found[found_triples].b = b;
                found[found_triples].c = c;
                found[found_triples].valid = true;
                ++found_triples;
            }
        }
    }

    printf("Found %d\n", found_triples);
    remove_multiples(found, found_triples);

    int i;
	int final_triples = 0;
    for (i = 0; i < found_triples; ++i){
        if (found[i].valid) {
            printf("%d, %d, %d, %d\n", i, found[i].a, found[i].b, found[i].c);
            ++final_triples;
        }
    }
    printf("Final %d\n", final_triples);

    return 0;
}



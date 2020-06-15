#include <stdio.h>

#define MAX_VALUE 200000

int main() {

	int primes[MAX_VALUE] = {0};

    int num_primes = 0;
    int last_prime = 2;
	int max_gap = 0;

	for (int i=2; i<MAX_VALUE/2; ++i){
		if (primes[i] == 0) {
			if ( i - last_prime > max_gap) {
				max_gap = i - last_prime;
				printf("Gap %d, Primes(%d,%d)\n",max_gap, last_prime, i);
			}
			last_prime = i;
			num_primes++;
			int current = i + i;
			while (current < MAX_VALUE){
				primes[current] = 1;
				current += i;
			}
		}
	}
	
	for (int i=MAX_VALUE/2; i<MAX_VALUE; ++i){
		if (primes[i] == 0) {
			if ( i - last_prime > max_gap) {
				max_gap = i - last_prime;
				printf("Gap %d, Primes(%d,%d)\n",max_gap,last_prime, i);
			}
			last_prime = i;
			num_primes++;
		}
	}
	printf("Num primes = %d\n", num_primes);
	printf("Last prime = %d\n", last_prime);
}
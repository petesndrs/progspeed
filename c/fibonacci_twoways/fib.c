#include <stdio.h>

int rec_fib(int n){

	if (n==1 || n==2) return 1;
	
	return rec_fib(n-1) + rec_fib(n-2);
}

int it_fib(int n){
	int prev_sum = 1;
	int sum = 1;
	
	if (n==1 || n==2) return 1;
	
	for (int i=3; i<=n; ++i){
		int tmp = sum;
		sum += prev_sum;
		prev_sum = tmp;
	}
	return sum;
}

int main() {
	const int N = 44;

	printf("Iterative Fibonacci(%d) = %d\n", N, it_fib(N));
	printf("Recursive Fibonacci(%d) = %d\n", N, rec_fib(N));

}
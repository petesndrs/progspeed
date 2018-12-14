package main

import "fmt"

func rec_fib(n int) int {

	if n==1 || n==2 { return 1 }
	
	return rec_fib(n-1) + rec_fib(n-2)
}

func it_fib(n int) int {
	var prev_sum int = 1
	var sum int = 1
	var tmp int
	
	if n==1 || n==2 { return 1 }
	
	for i:=3; i<=n; i+=1 {
		tmp = sum
		sum += prev_sum
		prev_sum = tmp
	}
	return sum
}

func main() {
	const N int = 44

	fmt.Printf("Iterative Fibonacci(%d) = %d\n", N, it_fib(N))
	fmt.Printf("Recursive Fibonacci(%d) = %d\n", N, rec_fib(N))

}
package main

import "fmt"

const MAX_VALUE int = 200000

func main() {

	var primes[MAX_VALUE] int

    var num_primes int = 0
    var last_prime int = 2
	var max_gap int = 0

	for i:=2; i<MAX_VALUE/2; i++ {
		if primes[i] == 0 {
			if i - last_prime > max_gap {
				max_gap = i - last_prime
				fmt.Printf("Gap %d, Primes(%d,%d)\n",max_gap, last_prime, i)
			}
			last_prime = i
			num_primes++
			var current int = i + i
			for current < MAX_VALUE {
				primes[current] = 1
				current += i
			}
		}
	}
	
	for i:=MAX_VALUE/2; i<MAX_VALUE; i++ {
		if primes[i] == 0 {
			if i - last_prime > max_gap {
				max_gap = i - last_prime
				fmt.Printf("Gap %d, Primes(%d,%d)\n",max_gap,last_prime, i)
			}
			last_prime = i
			num_primes++
		}
	}
	fmt.Printf("Num primes = %d\n", num_primes)
	fmt.Printf("Last prime = %d\n", last_prime)
}
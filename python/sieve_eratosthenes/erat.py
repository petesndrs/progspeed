
MAX_VALUE=200000

def main():

    primes = [0] * MAX_VALUE

    num_primes = 0
    last_prime = 2
    max_gap = 0
    for i in range(2, int(MAX_VALUE/2)):
        if (primes[i] == 0):
            if ( i - last_prime > max_gap):
                max_gap = i - last_prime
                print("Gap {}, Primes({},{})".format(max_gap, last_prime, i))

            last_prime = i
            num_primes += 1
            current = i + i
            while (current < MAX_VALUE):
                primes[current] = 1
                current += i
	
	
    for i in range(int(MAX_VALUE/2), MAX_VALUE):
        if (primes[i] == 0):
            if ( i - last_prime > max_gap):
                max_gap = i - last_prime
                print("Gap {}, Primes({},{})".format(max_gap,last_prime, i))
		
            last_prime = i
            num_primes += 1

    print("Num primes = {}".format(num_primes))
    print("Last prime = {}".format(last_prime))

if __name__ == "__main__":
    main()
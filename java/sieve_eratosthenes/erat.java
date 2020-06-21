class erat
{
	final static int MAX_VALUE = 200000;

	public static void main(String args[])
	{

		int[] primes  = new int[MAX_VALUE];;

		int num_primes = 0;
		int last_prime = 2;
		int max_gap = 0;

		for (int i=2; i<MAX_VALUE/2; ++i){
			if (primes[i] == 0) {
				if ( i - last_prime > max_gap) {
					max_gap = i - last_prime;
					System.out.printf("Gap %d, Primes(%d,%d)\n",max_gap, last_prime, i);
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
					System.out.printf("Gap %d, Primes(%d,%d)\n",max_gap,last_prime, i);
				}
				last_prime = i;
				num_primes++;
			}
		}
		System.out.printf("Num primes = %d\n", num_primes);
		System.out.printf("Last prime = %d\n", last_prime);
	}
}
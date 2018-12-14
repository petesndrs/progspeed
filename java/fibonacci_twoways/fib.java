class fib
{
	
	private static int rec_fib(int n){
		
		if (n==1 || n==2) return 1;
		
		return rec_fib(n-1) + rec_fib(n-2);
	}

	private static int it_fib(int n){
		int prev_sum = 1;
		int sum = 1;
		int tmp;
		
		if (n==1 || n==2) return 1;
		
		for (int i=3; i<=n; ++i){
			tmp = sum;
			sum += prev_sum;
			prev_sum = tmp;
		}
		return sum;
	}

	public static void main(String args[])
	{
		final int N = 44;

		System.out.printf("Iterative Fibonacci(%d) = %d\n", N, it_fib(N));
		System.out.printf("Recursive Fibonacci(%d) = %d\n", N, rec_fib(N));
	}
}
class pirn
{

	private int next_xor(int last)
	{
		last ^= last << 13;
		last ^= last >>> 17;
		last ^= last << 5;
		return last;	
	}

	private int next_lincong(int last) 
	{
		return ((last * 1103515245) + 12345) & 0x7fffffff;
	}

	public void run()
	{
		final int SEED = 1;
		int val1 = SEED;
		int val2 = SEED;
		int number = 0;
		int inside = 0;
	
		do {
			val1 = next_xor(val1);
			val2 = next_lincong(val2);
			long val1long = 0xffffffffl & (long)val1;
			float f1 = (float)val1long/0xffffffffl;
			float f2 = (float)val2/0x7fffffff;
			if ( (number&0xfffff) == 0 ){
				System.out.printf("number %d, val1 %x, val2 %x, f1 %f, f2 %f\n", number, val1, val2, f1, f2);
			}
			if ((f1*f1 + f2*f2) < 1){
				inside++;
			}
			number++;
		} while (number < 10000000);

		System.out.printf("%d %d, %f\n", number, inside, 4.0 * inside/number);
	}
	
	public static void main(String args[])
	{
		pirn myClass = new pirn();
		myClass.run();
	}
}
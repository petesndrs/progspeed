class ana
{
	private int factorial(int n)
	{
		int fact = 1;
		for (int i=1; i < n+1; ++i)
		{
			fact = fact * i;
		}
		return fact;
	}

	private void anagram(final String string, String[] output, int output_index)
	{
		int length = string.length();
		for (int i=0; i<length; ++i)
		{

			char pick = string.charAt(i);
			String remainder = string.substring(0,i)+string.substring(i+1,length); 

			for (int j=0; j<factorial(length - 1); ++j)
			{
				output[ output_index + j] += pick;
			}

			anagram(remainder, output, output_index);

			output_index += factorial(length - 1);
		}
	}

	private int deduplicate(String[] output, int number)
	{
		int count = 0;
		for (int i=0; i<number; ++i)
		{
			if (!output[i].equals(""))
			{
				count++;
				for (int j=i+1; j<number; ++j){
					if (output[i].equals(output[j]))
					{
						output[j] = "";
					}
				}
			}
		}
		return count;
	}

	public void run()
	{
		final String a = "pineapple";

		int number = a.length();
		System.out.printf("Number of letters: %d\n",number);

		int anagrams = factorial(number);
		System.out.printf("Number of anagrams: %d\n",anagrams);

		String[] output = new String[anagrams];
		for (int i=0; i<anagrams; ++i)
		{
			output[i] = "";
		}
	
		anagram(a, output, 0);

		System.out.printf("First anagram: %s\n",output[0]);
		System.out.printf("Last anagram: %s\n",output[anagrams-1]);
		int count = deduplicate(output, anagrams);
		System.out.printf("Number of unique anagrams: %d\n",count);
	}
	
	public static void main(String args[])
	{
		ana myClass = new ana();
		myClass.run();
	}
}
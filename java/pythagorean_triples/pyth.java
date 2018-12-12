class pyth
{

	public class triple
	{
		public int a;
		public int b;
		public int c;
		public boolean valid;
	}

	private int search(int a, int b, int start, int end, int inc)
	{
		for (int c = start; c <= end; c+=inc)
		{
			if (c*c == a*a + b*b)
			{
				return c;
			}
		}
		return 0;
	}

	private void remove_multiples(triple[] found, int num)
	{
		for (int i = 0; i < num; ++i)
		{
			if (found[i].valid)
			{
				int k = 2;
				for (int j = i+1; j < num; ++j)
				{
					if (found[j].valid)
					{
						if ( found[i].a * k == found[j].a &&
							 found[i].b * k == found[j].b &&
							 found[i].c * k == found[j].c )
						{
						  found[j].valid = false;
						  k += 1;
						}
					}
				}
			}
		}
	}

	final int MIN = 1;
	final int MAX = 2000;
	final int MAX_TRIPLES = 4000;
	public triple[] found = new triple[MAX_TRIPLES];

	public void run()
	{
		int found_triples = 0;
		for (int a = MIN; a <= MAX; ++a)
		{
			for (int b = a; b <= MAX; ++b)
			{
				int c = search (a, b, b+1, MAX, 1);
				if (c > 0)
				{
					System.out.printf("%d, %d, %d, %d\n", found_triples, a, b, c);
					found[found_triples] = new triple();
					found[found_triples].a = a;
					found[found_triples].b = b;
					found[found_triples].c = c;
					found[found_triples].valid = true;
					found_triples += 1;
				}
			}
		}
		remove_multiples(found, found_triples);

		for (int i = 0; i < found_triples; ++i)
		{
			if (found[i].valid)
			{
				System.out.printf("%d, %d, %d, %d\n", i, found[i].a, found[i].b, found[i].c);
			}
		}
	}

	public static void main(String args[])
	{
		pyth myClass = new pyth();
		myClass.run();
	}

}

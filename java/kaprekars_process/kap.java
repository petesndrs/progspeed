class kap
{

	private static int kaprekar(int n){
		int digit[] = new int[4];
		int large = 0, small = 0;
		
		int count = 0;
		
		if ( n == 0000 || n == 1111 || n == 2222 || n == 3333 || n == 4444 || n == 5555 ||
			 n == 6666 || n == 7777 || n == 8888 || n == 9999) {
			return 0;
		}

		while (n != 6174) {
			
			//printf("%d\n", n);
			
			digit[0] = n/1000;
			digit[1] = n/100 - digit[0]*10;
			digit[2] = n/10 - digit[0]*100 - digit[1]*10;
			digit[3] = n - digit[0]*1000 - digit[1]*100 - digit[2]*10;
			
			int a,b,c,d,i;
			a = b = c = d = i = 0;
			for (i=0; i<24; ++i) {
				switch (i) {
				case 0:
					a=1; b=2; c=3; d=0; break;
				case 1:
					a=1; b=2; c=0; d=3; break;
				case 2:
					a=1; b=3; c=0; d=2; break;
				case 3:
					a=1; b=3; c=2; d=0; break;
				case 4:
					a=1; b=0; c=3; d=2; break;
				case 5:
					a=1; b=0; c=2; d=3; break;
				case 6:
					a=2; b=1; c=3; d=0; break;
				case 7:
					a=2; b=1; c=0; d=3; break;
				case 8:
					a=2; b=3; c=1; d=0; break;
				case 9:
					a=2; b=3; c=0; d=1; break;
				case 10:
					a=2; b=0; c=3; d=1; break;
				case 11:
					a=2; b=0; c=1; d=3; break;
				case 12:
					a=3; b=1; c=2; d=0; break;
				case 13:
					a=3; b=1; c=0; d=2; break;
				case 14:
					a=3; b=2; c=1; d=0; break;
				case 15:
					a=3; b=2; c=0; d=1; break;
				case 16:
					a=3; b=0; c=1; d=2; break;
				case 17:
					a=3; b=0; c=2; d=1; break;
				case 18:
					a=0; b=1; c=2; d=3; break;
				case 19:
					a=0; b=1; c=3; d=2; break;
				case 20:
					a=0; b=2; c=1; d=3; break;
				case 21:
					a=0; b=2; c=3; d=1; break;
				case 22:
					a=0; b=3; c=1; d=2; break;
				case 23:
					a=0; b=3; c=2; d=1; break;
				}
				
				if (digit[a] >= digit[b] && digit[b] >= digit[c] && digit[c] >= digit[d]) {
					large = digit[a]*1000 + digit[b]*100 + digit[c]*10 + digit[d];
					small = digit[d]*1000 + digit[c]*100 + digit[b]*10 + digit[a];
					break;
				}
			}
			n = large - small;
			count++;
		}
		
		return count;
	}
	
	public static void main(String args[])
	{
		final int MAX_STEPS = 10;

		int i, count;
		int[] counts = new int[MAX_STEPS];

		for (i=0; i<= 9999; ++i) {
			count = kaprekar(i);
			counts[count]++;
		}
		
		for (i=0; i<MAX_STEPS; ++i) {
				System.out.printf("Steps %d, Steps %d\n", i, counts[i]);
		}
	}
}
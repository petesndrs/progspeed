class xor 
{
    public static void main(String args[])
    {
        final int SEED = 1;
        int val = SEED;
        long period = 0;

         do {
            val ^= val << 13;
            val ^= val >>> 17;
            val ^= val << 5;
            if ( (val & 0xffffff00) == 0 )
            {
                System.out.printf("period %d, val %x\n", period, val);
            }
            period++;
        } while (val != SEED);

    System.out.printf("%d\n", period);
    }
}
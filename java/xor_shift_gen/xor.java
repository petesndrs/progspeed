class xor 
{
    public static void main(String args[])
    {
        final int SEED = 1;
        int state = SEED;
        int val = 0;
        long period = 0;

        while (val != SEED)
        {
            val = state;
            val ^= val << 13;
            val ^= val >>> 17;
            val ^= val << 5;
            if ( (val & 0xffffff00) == 0 )
            {
                System.out.printf("period %d state %x, val %x\n", period, state, val);
            }
            period++;
            state = val;
        }
    System.out.printf("%d\n", period);
    }
}
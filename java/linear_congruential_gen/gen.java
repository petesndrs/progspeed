class gen 
{
    public static void main(String args[])
    {
        final int SEED = 0;
        int state = SEED;
        int val = 1;
        long period = 0;

        while (val != SEED){
            val = ((state * 1103515245) + 12345) & 0x7fffffff;
            if (val < 0x100){
                System.out.printf("Count %d, state %x, val %x\n", period, state, val);
            }
            period++;
            state = val;
        }

        System.out.printf("Period %d\n", period);
    }
}
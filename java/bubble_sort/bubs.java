class bubs
{

  final static int NUMBER_VALUES=20000;

  private int next_xor(int last) {
      last ^= last << 13;
      last ^= last >>> 17;
      last ^= last << 5;
      return last;
  }

  private void run() {
      long[] values = new long[NUMBER_VALUES];
      final int SEED = 1;
      int val = SEED;

      for (int i=0; i < NUMBER_VALUES; ++i ){
          val = next_xor(val);
          values[i] = val & 0xffffffffl;
      }

      int num_switches = 0;
      boolean switched = true;

      while (switched){
          switched = false;
          for (int i = 0; i < NUMBER_VALUES - 1; ++i){
              if (values[i] > values[i+1]){
                  switched = true;
                  num_switches++;
                  long temp = values[i];
                  values[i] = values[i+1];
                  values[i+1] = temp;
              }
          }
      }

      System.out.printf("Switches: %d\n", num_switches);
      for (int i=0; i < NUMBER_VALUES; i+=NUMBER_VALUES/10 ){
          System.out.printf("%d: %d\n", i, values[i]);
      }
      System.out.printf("%d: %d\n", NUMBER_VALUES-1, values[NUMBER_VALUES-1]);
  }

  public static void main(String args[])
  {
    bubs myClass = new bubs();
    myClass.run();
  }

}

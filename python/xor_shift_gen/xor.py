
def main():

    SEED = 1;
    state = SEED;
    val = 0;
    period = 0;

    while (val != SEED):
        val = state;
        val ^= (val << 13) & 0xffffffff;
        val ^= val >> 17;
        val ^= (val << 5) & 0xffffffff;

        if ( (val & 0xffffff00) == 0):
            print('period {} state {:x}, val {:x}'.format(period, state, val));

        period += 1;
        state = val;

    print('{}'.format(period));

if __name__ == "__main__":
    main()
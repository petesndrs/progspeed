

def main():

    SEED = 0;
    state = SEED;
    val = 1;
    period = 0;

    while (val != SEED):
        val = ((state * 1103515245) + 12345) & 0x7fffffff;

        if (val < 0x100):
            print('Count {}, state {:x}, val {:x}'.format(period, state, val));

        period += 1;
        state = val;

    print('Period {}'.format(period));

if __name__ == "__main__":
    main()

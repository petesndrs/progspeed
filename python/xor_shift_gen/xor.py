
def main():
    SEED = 1;
    val = SEED;
    period = 0;
    keepgoing = True

    while (keepgoing):
        val ^= (val << 13) & 0xffffffff;
        val ^= val >> 17;
        val ^= (val << 5) & 0xffffffff;

        if ( (val & 0xffffff00) == 0):
            print('period {}, val {:x}'.format(period, val));

        period += 1;
        if (val == SEED):
            keepgoing = False

    print('{}'.format(period));

if __name__ == "__main__":
    main()

NUMBER_VALUES=20000

def next_xor(last):
    last ^= last << 13 & 0xffffffff
    last ^= last >> 17
    last ^= last << 5 & 0xffffffff
    return last

def main():
    values = [0] * NUMBER_VALUES
    SEED = 1
    val = SEED

    for i in range(0, NUMBER_VALUES):
        val = next_xor(val)
        values[i] = val

    num_switches = 0
    switched = True

    while (switched):
        switched = False
        for i in range(0, NUMBER_VALUES-1):
            if (values[i] > values[i+1]):
                switched = True
                num_switches += 1
                temp = values[i]
                values[i] = values[i+1]
                values[i+1] = temp

    print('Switches: {}'.format(num_switches))
    for i in range(0, NUMBER_VALUES, NUMBER_VALUES//10):
        print('{}: {}'.format(i, values[i]))

    print('{}: {}'.format(NUMBER_VALUES-1, values[NUMBER_VALUES-1]))

if __name__ == "__main__":
    main()

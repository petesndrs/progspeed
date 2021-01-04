
def next_xor(last):
    last ^= (last << 13) & 0xffffffff
    last ^= last >> 17
    last ^= (last << 5) & 0xffffffff
    return last

def next_lincong(last):
    return ((last * 1103515245) + 12345) & 0x7fffffff

def main():
    SEED = 1
    val1 = SEED
    val2 = SEED
    number = 0
    inside = 0
    keepgoing = True

    while (keepgoing):
        val1 = next_xor(val1)
        val2 = next_lincong(val2)
        f1 = val1/0xffffffff
        f2 = val2/0x7fffffff
        if number&0xfffff == 0:
            print('number {}, val1 {:x}, val2 {:x}, f1 {:f}, f2 {:f}'.format(number, val1, val2, f1, f2))

        if (f1*f1 + f2*f2) < 1:
            inside+=1

        number+=1
        if number == 10000000:
            keepgoing = False

    print('{} {}, {}'.format(number, inside, 4.0 * inside/number))
    
if __name__ == "__main__":
    main()

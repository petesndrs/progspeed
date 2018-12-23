
class Triple:

    def __init__(self):
        self.a = None
        self.b = None
        self.c = None
        self.valid = True

def search(a, b, start, end, inc):

    for c in range(start, end + 1, inc):
        if (c*c == a*a + b*b):
            return c

    return 0;

def remove_multiples(found, num):

    for i in range(0, num):
        if found[i].valid:
            k = 2
            for j in range(i+1, num):
                if found[j].valid:
                    if ( found[i].a * k == found[j].a and
                         found[i].b * k == found[j].b and
                         found[i].c * k == found[j].c ):
                      found[j].valid = False
                      k += 1

def main():

    MIN = 1
    MAX = 2000
    MAX_TRIPLES = 4000
    found = [Triple() for i in range(MAX_TRIPLES)]

    # c**2 = a**2 + b**2

    found_triples = 0
    for a in range(MIN, MAX):
        for b in range(a, MAX): 
            c = search (a, b, b+1, MAX, 1)
            if (c):
                #print('{}, {}, {}, {}'.format(found_triples, a, b, c))
                found[found_triples].a = a
                found[found_triples].b = b
                found[found_triples].c = c
                found_triples += 1

    print('Found {}'.format(found_triples))
    remove_multiples(found, found_triples)

    final_triples = 0
    for i in range(0, found_triples):
        if found[i].valid:
            print('{}, {}, {}, {}'.format(i, found[i].a, found[i].b, found[i].c))
            final_triples += 1

    print('Final {}'.format(final_triples))

if __name__ == "__main__":
    main()

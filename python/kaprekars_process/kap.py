

def kaprekar(n):
    digit = [0,0,0,0]

    count = 0

    if ( n == 0000 or n == 1111 or n == 2222 or n == 3333 or n == 4444 or n == 5555 or
        n == 6666 or n == 7777 or n == 8888 or n == 9999):
        return 0

    while (n != 6174):

        digit[0] = n//1000
        digit[1] = n//100 - digit[0]*10
        digit[2] = n//10 - digit[0]*100 - digit[1]*10
        digit[3] = n - digit[0]*1000 - digit[1]*100 - digit[2]*10

        for i in range(0, 24):
            if i==0:
                a=1; b=2; c=3; d=0
            elif i==1:
                a=1; b=2; c=0; d=3
            elif i==2:
                a=1; b=3; c=0; d=2
            elif i==3:
                a=1; b=3; c=2; d=0
            elif i==4:
                a=1; b=0; c=3; d=2
            elif i==5:
                a=1; b=0; c=2; d=3
            elif i==6:
                a=2; b=1; c=3; d=0
            elif i==7:
                a=2; b=1; c=0; d=3
            elif i==8:
                a=2; b=3; c=1; d=0
            elif i==9:
                a=2; b=3; c=0; d=1
            elif i==10:
                a=2; b=0; c=3; d=1
            elif i==11:
                a=2; b=0; c=1; d=3
            elif i==12:
                a=3; b=1; c=2; d=0
            elif i==13:
                a=3; b=1; c=0; d=2
            elif i==14:
                a=3; b=2; c=1; d=0
            elif i==15:
                a=3; b=2; c=0; d=1
            elif i==16:
                a=3; b=0; c=1; d=2
            elif i==17:
                a=3; b=0; c=2; d=1
            elif i==18:
                a=0; b=1; c=2; d=3
            elif i==19:
                a=0; b=1; c=3; d=2
            elif i==20:
                a=0; b=2; c=1; d=3
            elif i==21:
                a=0; b=2; c=3; d=1
            elif i==22:
                a=0; b=3; c=1; d=2
            elif i==23:
                a=0; b=3; c=2; d=1
            
            if (digit[a] >= digit[b] and digit[b] >= digit[c] and digit[c] >= digit[d]):
                large = digit[a]*1000 + digit[b]*100 + digit[c]*10 + digit[d];
                small = digit[d]*1000 + digit[c]*100 + digit[b]*10 + digit[a];
                break;
                
        n = large - small
        count += 1
    
    return count

def main():

    MAX_STEPS = 10
    counts = [0,0,0,0,0,0,0,0,0,0]

    for i in range(0,10000):
        count = kaprekar(i)
        counts[count] += 1

    for i in range(0,MAX_STEPS):
            print("Steps {}, Steps {}".format(i, counts[i]))

if __name__ == "__main__":
    main()
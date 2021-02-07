import math

def numdigits(n):
      i = 10
      digits = 1
      while (True):
          if (n < i):
              return digits
          i *= 10
          digits += 1

def main():
    for n in range(10, 10000000):
        total = 0
        digits = numdigits(n)
        temp_n = n

        for i in range(0, digits):
            a = temp_n%10
            total = total + math.pow(a, digits)
            temp_n = temp_n//10

        if (total == n):
            print('Armstrong number: {}'.format(n))

if __name__ == "__main__":
    main()

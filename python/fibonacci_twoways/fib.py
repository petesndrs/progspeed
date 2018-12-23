

def rec_fib(n):

    if (n==1 or n==2):
        return 1

    return rec_fib(n-1) + rec_fib(n-2)


def it_fib(n):
    prev_sum = 1
    sum = 1

    if (n==1 or n==2):
        return 1

    for i in range(3,n+1):
        tmp = sum
        sum += prev_sum
        prev_sum = tmp

    return sum


def main():
	N = 44

	print('Iterative Fibonacci({}) = {}'.format(N, it_fib(N)))
	print('Recursive Fibonacci({}) = {}'.format(N, rec_fib(N)))


if __name__ == "__main__":
    main()
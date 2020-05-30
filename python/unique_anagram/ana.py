
def factorial(n):
    fact = 1
    for i in range(1, n+1):
        fact = fact * i
    return fact

def anagram(string, output, output_index):

    length = len(string)
    for i in range(length):

        pick = string[i]
        remainder = string[0:i] + string[i+1:length]

        for j in range(0, factorial(length-1)):
            output[ output_index + j] += pick

        anagram(remainder, output, output_index)

        output_index += factorial(length-1);

def deduplicate(output, number):
    count = 0
    for i in range(number):
        if output[i] != "":
            count += 1
            for j in range(i+1, number):
                if output[i] == output[j]:
                    output[j] = ""
    return count

def main():
    a = "pineapple"

    number = len(a)
    print("Number of letters: {}".format(number))

    anagrams = factorial(number)
    print("Number of anagrams: {}".format(anagrams))

    output = [""] * anagrams

    anagram(a, output, 0)

    print("First anagram: {}".format(output[0]))
    print("Last anagram: {}".format(output[anagrams-1]))
    count = deduplicate(output, anagrams)
    print("Number of unique anagrams: {}".format(count))

if __name__ == "__main__":
    main()
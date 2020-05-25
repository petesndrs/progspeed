
def factorial(n):
    fact = 1
    for i in range(1, n+1): 
        fact = fact * i
    return fact

def anagram(string, output_index):

    for i in range(len(string)):

        pick = string[i]        
        remainder = string[0:i] + string[i+1:len(string)]

        for j in range(0, factorial(len(remainder))):
            output[ output_index + j] += pick

        anagram(remainder, output_index)

        output_index += factorial(len(remainder));

def deduplicate(output, number):
    count = 0
    for i in range(number):
        if output[i] != "":
            count += 1
            for j in range(i+1, number):
                if output[i] == output[j]:
                    output[j] = ""        
    return count

a = "pineapple"
    
number = len(a)
print("Number of letters: {}".format(number))

anagrams = factorial(number)
print("Number of anagrams: {}".format(anagrams))

output = [""] * anagrams

anagram(a, 0)

count = deduplicate(output, anagrams)    
print("Number of unique anagrams: {}".format(count))
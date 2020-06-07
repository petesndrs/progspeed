package main

import "fmt"

func factorial(n int) int {
    var fact int = 1
    for i:=1; i < n+1; i+=1 {
        fact = fact * i
	}
    return fact
}

func anagram(astring string, output[] string, output_index int){
	var length int = len(astring)
    for i:=0; i<length; i+=1 {

        a := []byte(astring)
        var pick byte = a[i]
        var remainder[] byte = append(a[0:i], a[i+1:length]...)
		
        for j:=0; j<factorial(length - 1); j+=1 {
            output[ output_index + j] += string(pick)
		}
		
        anagram(string(remainder), output, output_index)

        output_index += factorial(length - 1)
	}
}

func deduplicate(output[] string, number int) int {
    var count int = 0
	for i, ana1 := range output {
        if ana1 != "" {
            count++
            for j := i+1; j<number; j+=1 {
                if ana1 == output[j] {
                    output[j] = ""
				}
            }
        }
    }
    return count
}

func main() {
    const a string = "pineapple"

    const number int = len(a)
    fmt.Printf("Number of letters: %d\n",number)

    var anagrams int = factorial(number)
    fmt.Printf("Number of anagrams: %d\n",anagrams)

	output := make([]string, anagrams)
	
    anagram(a, output, 0);

    fmt.Printf("First anagram: %s\n",output[0]);
    fmt.Printf("Last anagram: %s\n",output[anagrams-1]);
    var count int = deduplicate(output, anagrams);
    fmt.Printf("Number of unique anagrams: %d\n",count);
}
package main

import "fmt"

type triple struct {
    a int
    b int
    c int
    valid bool
}


func search(a int, b int, start int, end int, inc int) int {

    for c := start; c <= end; c += inc {
        if c*c == a*a + b*b {
            return c
        }
    }

    return 0
}

func remove_multiples(found []triple, num int) {
    var k int
    for i := 0; i < num; i++ {
        if found[i].valid {
            k = 2
            for j := i+1; j < num; j++ {
                if found[j].valid {
                    if ( found[i].a * k == found[j].a &&
                         found[i].b * k == found[j].b &&
                         found[i].c * k == found[j].c ){
                      found[j].valid = false
                      k++
                    }
                }
            }
        }
    }
}

func main() {

    const MIN int = 1
    const MAX int = 2000
    const MAX_TRIPLES int = 4000
    var found [MAX_TRIPLES]triple

    // c**2 = a**2 + b**2

    var c int
    var found_triples = 0
    for a := MIN; a < MAX; a++ {
        for b := a; b < MAX; b++ { 
            c = search (a, b, b+1, MAX, 1)
            
            if c != 0 {
                //fmt.Printf("%d, %d, %d, %d\n", found_triples, a, b, c)
                found[found_triples].a = a
                found[found_triples].b = b
                found[found_triples].c = c
                found[found_triples].valid = true
                found_triples++
            }
        }
    }

    fmt.Printf("Found %d\n", found_triples)
    remove_multiples(found[:], found_triples)

    var final_triples = 0
    for i := 0; i < found_triples; i++ {
        if found[i].valid {
            fmt.Printf("%d, %d, %d, %d\n", i, found[i].a, found[i].b, found[i].c)
            final_triples++
        }
    }
    fmt.Printf("Final %d\n", final_triples)
}



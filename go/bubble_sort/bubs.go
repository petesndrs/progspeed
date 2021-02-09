package main

import "fmt"

const NUMBER_VALUES = 20000

func next_xor(last uint32) uint32{
    last ^= last << 13;
    last ^= last >> 17;
    last ^= last << 5;
    return last;
}

func main() {
    var values[NUMBER_VALUES] uint32
    const SEED uint32 = 1
    var val uint32 = SEED

    for i:=0; i < NUMBER_VALUES; i++ {
        val = next_xor(val)
        values[i] = val
    }

    var num_switches int = 0
    var switched bool = true

    for switched {
        switched = false
        for i := 0; i < NUMBER_VALUES - 1; i++ {
            if (values[i] > values[i+1]){
                switched = true
                num_switches++
                var temp uint32 = values[i]
                values[i] = values[i+1]
                values[i+1] = temp
            }
        }
    }

    fmt.Printf("Switches: %d\n", num_switches)
    for i:=0; i < NUMBER_VALUES; i+=NUMBER_VALUES/10 {
        fmt.Printf("%d: %d\n", i, values[i])
    }
    fmt.Printf("%d: %d\n", NUMBER_VALUES-1, values[NUMBER_VALUES-1])
}

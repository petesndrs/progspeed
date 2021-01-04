package main

import "fmt"

func next_xor(last uint32) uint32 {
    last ^= last << 13
    last ^= last >> 17
    last ^= last << 5
    return last
}

func next_lincong(last uint32) uint32 {
    return ((last * 1103515245) + 12345) & 0x7fffffff
}

func main() {
    const SEED uint32 = 1
    var val1 uint32 = SEED
	var val2 uint32 = SEED
    var number int = 0
    var inside int = 0
	var keepgoing bool = true
	
    for keepgoing {
        val1 = next_xor(val1)
		val2 = next_lincong(val2)
		f1 := float32(val1)/0xffffffff
		f2 := float32(val2)/0x7fffffff
        if (number&0xfffff) == 0 {
            fmt.Printf("number %d, val1 %x, val2 %x, f1 %f, f2 %f\n", number, val1, val2, f1, f2)
        }
		if (f1*f1 + f2*f2) < 1 {
		    inside++
		}
        number++
        if number == 10000000 {
		    keepgoing = false
		}
		
	}

    fmt.Printf("%d %d, %f\n", number, inside, 4.0 * float32(inside)/float32(number))

}

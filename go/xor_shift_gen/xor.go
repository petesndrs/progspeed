package main

import "fmt"

func main() {
    const SEED uint32 = 1
    var val uint32 = SEED
    var period int64 = 0
    var keepgoing bool = true

    for keepgoing {
        val ^= val << 13
        val ^= val >> 17
        val ^= val << 5
        if (val & 0xffffff00) == 0 {
            fmt.Printf("period %d, val %x\n", period, val)
        }
        period++
        if val == SEED {
            keepgoing = false
        }
    }

    fmt.Printf("%d\n", period)

}

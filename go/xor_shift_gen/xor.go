package main

import "fmt"

func main() {
    const SEED uint32 = 1
    var state uint32 = SEED
    var val uint32 = 0
    var period int64 = 0

    for val != SEED {
        val = state
        val ^= val << 13
        val ^= val >> 17
        val ^= val << 5
        if (val & 0xffffff00) == 0 {
            fmt.Printf("period %d state %x, val %x\n", period, state, val)
        }
        period++
        state = val
    }

    fmt.Printf("%d\n", period)

}

package main

import "fmt"

func main() {
    
    const SEED int = 0
    var state int = SEED
    var val int = 1
    var period int = 0

    for val != SEED {
        val = ((state * 1103515245) + 12345) & 0x7fffffff
        if val < 0x100 {
            fmt.Printf("period %d state %x, val %x\n", period, state, val)
        }
        period++
        state = val
    }

    fmt.Printf("%d\n", period)

}

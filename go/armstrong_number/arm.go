package main

import "fmt"
import "math"

func numdigits(n int) int {
  var i int = 10
  var digits int = 1
  for true {
    if (n < i) { return digits }
    i *= 10
    digits++
  }
  return 0
}

func main() {
  for n:=10; n<10000000; n++ {
    var total int = 0
    var digits int = numdigits(n)
    var temp_n int = n

    for i:=0; i<digits; i++ {
      a := temp_n%10
      total = total + int(math.Pow(float64(a), float64(digits))) 
      temp_n = temp_n/10
    }

    if (total == n) {
      fmt.Printf("Armstrong number: %d\n", n)
    }
  }
}

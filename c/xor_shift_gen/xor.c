
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int main() {
    const int32_t SEED = 1;
    uint32_t val = SEED;
    int64_t period = 0;

    do {
        val ^= val << 13;
        val ^= val >> 17;
        val ^= val << 5;
        if ( (val & 0xffffff00) == 0 ){
            printf("period %"PRId64", val %x\n", period, val);
        }
        period++;
    } while (val != SEED);

    printf("%"PRId64"\n", period);

}

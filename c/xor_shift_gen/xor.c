
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int main() {
    const int32_t SEED = 1;
    uint32_t state = SEED;
    uint32_t val = 0;
    int64_t period = 0;

    while (val != SEED){
        val = state;
        val ^= val << 13;
        val ^= val >> 17;
        val ^= val << 5;
        if ( (val & 0xffffff00) == 0 ){
            printf("period %"PRId64" state %x, val %x\n", period, state, val);
        }
        period++;
        state = val;
    }

    printf("%"PRId64"\n", period);

}

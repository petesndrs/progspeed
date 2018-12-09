
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int main() {
    const int32_t SEED = 0;
    int32_t state = SEED;
    int32_t val = 1;
    int64_t period = 0;

    while (val != SEED){
        val = ((state * 1103515245) + 12345) & 0x7fffffff;
        if (val < 0x100){
            printf("period %"PRId64" state %x, val %x\n", period, state, val);
        }
        period++;
        state = val;
    }

    printf("%"PRId64"\n", period);

}

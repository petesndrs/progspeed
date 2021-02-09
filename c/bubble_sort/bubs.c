#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>

#define NUMBER_VALUES 20000

uint32_t next_xor(uint32_t last) {
    last ^= last << 13;
    last ^= last >> 17;
    last ^= last << 5;
    return last;
}

int main() {
    uint32_t values[NUMBER_VALUES];
    const int32_t SEED = 1;
    uint32_t val = SEED;

    for (int i=0; i < NUMBER_VALUES; ++i ){
        val = next_xor(val);
        values[i] = val;
    }

    int num_switches = 0;
    bool switched = true;

    while (switched){
        switched = false;
        for (int i = 0; i < NUMBER_VALUES - 1; ++i){
            if (values[i] > values[i+1]){
                switched = true;
                num_switches++;
                uint32_t temp = values[i];
                values[i] = values[i+1];
                values[i+1] = temp;
            }
        }
    }

    printf("Switches: %d\n", num_switches);
    for (int i=0; i < NUMBER_VALUES; i+=NUMBER_VALUES/10 ){
        printf("%d: %"PRIu32"\n", i, values[i]);
    }
    printf("%d: %"PRIu32"\n", NUMBER_VALUES-1, values[NUMBER_VALUES-1]);
}

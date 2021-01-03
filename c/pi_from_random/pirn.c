
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

uint32_t next_xor(uint32_t last) {
    last ^= last << 13;
    last ^= last >> 17;
    last ^= last << 5;
    return last;	
}

uint32_t next_lincong(uint32_t last) {
    return ((last * 1103515245) + 12345) & 0x7fffffff;
}

int main() {
    const int32_t SEED = 1;
    uint32_t val1 = SEED;
	uint32_t val2 = SEED;
    int number = 0;
    int inside = 0;
	
    do {
        val1 = next_xor(val1);
		val2 = next_lincong(val2);
		float f1 = (float)val1/0xffffffff;
		float f2 = (float)val2/0x7fffffff;
        if ( (number&0xfffff) == 0 ){
            printf("number %d, val1 %x, val2 %x, f1 %f, f2 %f\n", number, val1, val2, f1, f2);
        }
		if ((f1*f1 + f2*f2) <= 1){
		    inside++;
		}
        number++;
    } while (number < 10000000);

    printf("%d %d, %f\n", number, inside, 4.0 * inside/number);

}


#include <stdio.h>

#define NUMBER_PRIMES 200

struct node {
	int value;
	int smaller;
	int larger;
};

int largest_pow2(int number){
	int power = 0;
	while (number != 1){
		number >>= 1;
		++power;
	}
	return 1<<power;
}

void create_tree(int* primes, int current, int last, struct node* nodes){
	int distance = (current > last) ? current - last : last - current;
	int next_distance = distance>>1;
	nodes[current].value = primes[current];
	if (next_distance == 0) {
		nodes[current].smaller = -1;
		nodes[current].larger = -1;
		return;
	}

	// lower
	nodes[current].smaller = current - next_distance;
	create_tree(primes, current - next_distance, current, nodes);

	//higher
	// At end
	if (current == NUMBER_PRIMES -1) {
		nodes[current].larger = -1;
		return;
	}
	// Would jump over end
	if (current + next_distance > NUMBER_PRIMES - 1){
		next_distance = largest_pow2(NUMBER_PRIMES - 1 - current);
	}
	nodes[current].larger = current + next_distance;
	create_tree(primes, current + next_distance, current, nodes);
}

int find_value(int value, struct node* nodes, int start){
	int current = start;
	while (current != -1){
		if (value == nodes[current].value){
			return 1;
		}
		else if (value > nodes[current].value) {
			current = nodes[current].larger;
		}
		else {
			current = nodes[current].smaller;
		}
	}
	return 0;
}

void dump_tree(struct node* nodes){
	for (int i = 0; i<NUMBER_PRIMES; ++i){
		printf("node %d, value %d, smaller %d, larger %d\n", i, nodes[i].value, nodes[i].smaller, nodes[i].larger);
	}
}

int main() {

	int primes[NUMBER_PRIMES] =
		{  3,   5,   7,  11,  13,  17,  19,  23,  29,  31,
		  37,  41,  43,  47,  53,  59,  61,  67,  71,  73,
		  79,  83,  89,  97, 101, 103, 107, 109, 113, 127,
		 131, 137, 139, 149, 151, 157, 163, 167, 173, 179,
		 181, 191, 193, 197, 199, 211, 223, 227, 229, 233,
		 239, 241, 251, 257, 263, 269, 271, 277, 281, 283,
		 293, 307, 311, 313, 317, 331, 337, 347, 349, 353,
		 359, 367, 373, 379, 383, 389, 397, 401, 409, 419,
		 421, 431, 433, 439, 443, 449, 457, 461, 463, 467,
		 479, 487, 491, 499, 503, 509, 521, 523, 541, 547,
		 557, 563, 569, 571, 577, 587, 593, 599, 601, 607,
		 613, 617, 619, 631, 641, 643, 647, 653, 659, 661,
		 673, 677, 683, 691, 701, 709, 719, 727, 733, 739,
		 743, 751, 757, 761, 769, 773, 787, 797, 809, 811,
		 821, 823, 827, 829, 839, 853, 857, 859, 863, 877,
		 881, 883, 887, 907, 911, 919, 929, 937, 941, 947,
		 953, 967, 971, 977, 983, 991, 997,1009,1013,1019,
		1021,1031,1033,1039,1049,1051,1061,1063,1069,1087,
		1091,1093,1097,1103,1109,1117,1123,1129,1151,1153,
		1163,1171,1181,1187,1193,1201,1213,1217,1223,1229};

	struct node nodes[NUMBER_PRIMES];
	printf("Number of primes %d\n", NUMBER_PRIMES);

	int start = largest_pow2(NUMBER_PRIMES) - 1;
	printf("Starting place %d\n", start);

	create_tree(primes, start, -1, nodes);
	dump_tree(nodes);

	int max = primes[NUMBER_PRIMES - 1] + primes[0];

	int i = 4;
	int end = 0;
	while (end == 0){
		i+=2;
		end = 1;
		for (int p = 0; p < NUMBER_PRIMES; ++p){
			int diff = i - primes[p];
			if (diff > 0){
				if (find_value(diff, nodes, start)) {
					printf("%d = %d + %d\n",i ,primes[p], diff);
					end = 0;
					break;
				}
			}
		}
	}
	printf("No Solution for %d\n",i);
}
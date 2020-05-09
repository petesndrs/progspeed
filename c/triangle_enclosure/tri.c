#include <stdio.h>

#define CIRCLE_RADIUS 20

struct point {
    int x;
    int y;
};

struct vector {
    int vx;
    int vy;
};

struct vector create_vector(struct point p1, struct point p2){
	struct vector v;
	v.vx = p1.x - p2.x;
	v.vy = p1.y - p2.y;
	return v;
}

struct vector perp_vector(struct vector v){
	struct vector vPerp;
	vPerp.vx = -v.vy;
	vPerp.vy = v.vx;
	return vPerp;
}

int dot_product(struct vector v1, struct vector v2){
	return (v1.vx*v2.vx) + (v1.vy*v2.vy); 
}

int bounded_point(struct point* points, int num, struct point pt){
	int max_x = points[0].x;
	int min_x = points[0].x;
	int max_y = points[0].y;
	int min_y = points[0].y;
	int i;
	for (i = 1; i < num; ++i) {
		if (points[i].x > max_x) max_x = points[i].x;
		if (points[i].x < min_x) min_x = points[i].x;
		if (points[i].y > max_y) max_y = points[i].y;
		if (points[i].y < min_y) min_y = points[i].y;
	}
	if (pt.x > min_x && pt.x < max_x && pt.y > min_y && pt.y < max_y){
		return 1;
	}
	return 0;
}

int enclosed_point(struct point* points, int num, struct point pt){
	int neg = 0, pos = 0;
	int i;
	for (i = 0; i < num; ++i) {
		struct vector v1 = perp_vector(create_vector(points[i], points[(i+1)%num]));
		struct vector v2 = create_vector(points[i], pt);
		int dp = dot_product(v1, v2);
		//printf("i %d, dp %d\n", i, dp);
		if (dp > 0) pos++;
		if (dp < 0) neg++;
	}
	//printf("pos %d, neg %d\n", pos, neg);
	if (pos == num) return 1;
	if (neg == num) return 1;
	return 0;
}

int main() {
	struct point all_points[4 * CIRCLE_RADIUS * CIRCLE_RADIUS];
	int num_points = 0;
	int xx, yy;
	
	for (xx = -CIRCLE_RADIUS; xx <= CIRCLE_RADIUS; ++xx){
		for (yy = -CIRCLE_RADIUS; yy <= CIRCLE_RADIUS; ++yy){
			if ( (xx*xx + yy*yy) < CIRCLE_RADIUS*CIRCLE_RADIUS ) {
				all_points[num_points].x = xx;
				all_points[num_points].y = yy;
				num_points++;
			}
		}
	}
	
	int triangles = 0;
	int bounded = 0;
	int enclosed = 0;
	const struct point pt = {.x = 0, .y = 0};
	int a, b, c;
	for (a = 0; a < num_points; ++a){
		for (b = a + 1; b < num_points; ++b){
			for (c = b + 1; c < num_points; ++c){
				triangles++;
				struct point points[3];
				points[0] = all_points[a];
				points[1] = all_points[b];
				points[2] = all_points[c];
				if (bounded_point(points, 3, pt)){
					bounded++;
					if (enclosed_point(points, 3, pt)){
						enclosed++;
					}
				}
			}
		}
	}
	
	printf("Valid points       : %d\n", num_points);
	printf("Valid triangles    : %d\n", triangles);
	printf("Bounded by points  : %d\n", bounded);
	printf("Enclosed by points : %d\n", enclosed);
}
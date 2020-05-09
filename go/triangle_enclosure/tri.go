package main

import "fmt"

const CIRCLE_RADIUS int = 20

type point struct {
	x int
	y int
};

type vector struct {
	vx int
	vy int
};

func create_vector(p1 point, p2 point) vector {
	var v vector
	v.vx = p1.x - p2.x
	v.vy = p1.y - p2.y
	return v
}

func perp_vector(v vector) vector {
	var vPerp vector
	vPerp.vx = -v.vy
	vPerp.vy = v.vx
	return vPerp
}

func dot_product(v1 vector, v2 vector) int {
	return (v1.vx*v2.vx) + (v1.vy*v2.vy)
}

func bounded_point(points[] point, num int, pt point) bool {
	var max_x int = points[0].x
	var min_x int = points[0].x
	var max_y int = points[0].y
	var min_y int = points[0].y
	for i:=1; i<num; i+=1 {
		if points[i].x>max_x { max_x = points[i].x }
		if points[i].x<min_x { min_x = points[i].x }
		if points[i].y>max_y { max_y = points[i].y }
		if points[i].y<min_y { min_y = points[i].y }
	}
	if pt.x>min_x && pt.x<max_x && pt.y>min_y && pt.y<max_y {
		return true
	}
	return false
}

func enclosed_point(points[] point, num int, pt point) bool {
	var neg int = 0
	var pos int = 0
	for i:=0; i<num; i+=1 {
		var v1 vector = perp_vector(create_vector(points[i], points[(i+1)%num]))
		var v2 vector = create_vector(points[i], pt)
		var dp int = dot_product(v1, v2)
		//printf("i %d, dp %d\n", i, dp);
		if dp>0 {pos++}
		if dp<0 {neg++}
	}
	//printf("pos %d, neg %d\n", pos, neg);
	if pos==num {return true}
	if neg==num {return true}
	return false
}

func main() {
	var all_points[4 * CIRCLE_RADIUS * CIRCLE_RADIUS] point
	var num_points int = 0;

	for xx:=-CIRCLE_RADIUS; xx<=CIRCLE_RADIUS; xx+=1 {
		for yy:=-CIRCLE_RADIUS; yy<=CIRCLE_RADIUS; yy+=1 {
			if (xx*xx + yy*yy)<CIRCLE_RADIUS*CIRCLE_RADIUS {
				all_points[num_points].x = xx
				all_points[num_points].y = yy
				num_points+=1
			}
		}
	}

	var triangles int = 0;
	var bounded int = 0;
	var enclosed int = 0;
	pt := point {x:0, y:0}

	for a:=0; a<num_points; a+=1 {
		for b:=a+1; b<num_points; b+=1 {
			for c:=b+1; c<num_points; c+=1 {
				triangles+=1
				var points[3] point
				points[0] = all_points[a]
				points[1] = all_points[b]
				points[2] = all_points[c]
				if bounded_point(points[:], 3, pt) {
					bounded+=1
					if enclosed_point(points[:], 3, pt) {
						enclosed+=1
					}
				}
			}
		}
	}
	
	fmt.Printf("Valid points       : %d\n", num_points)
	fmt.Printf("Valid triangles    : %d\n", triangles)
	fmt.Printf("Bounded by points  : %d\n", bounded)
	fmt.Printf("Enclosed by points : %d\n", enclosed)
}
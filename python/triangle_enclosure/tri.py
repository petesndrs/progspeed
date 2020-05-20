
class point:
    def __init__(self):
        self.x = 0
        self.y = 0

class vector:
    def __init__(self):
        self.vx = 0
        self.vy = 0

def create_vector(p1, p2):
	v = vector()
	v.vx = p1.x - p2.x
	v.vy = p1.y - p2.y
	return v

def perp_vector(v):
    vPerp = vector()
    vPerp.vx = -v.vy
    vPerp.vy = v.vx
    return vPerp

def dot_product(v1, v2):
    return (v1.vx*v2.vx) + (v1.vy*v2.vy) 

def bounded_point(points, num, pt):
    max_x = points[0].x
    min_x = points[0].x
    max_y = points[0].y
    min_y = points[0].y

    for i in range(1, num):
        if (points[i].x > max_x):
            max_x = points[i].x
        if (points[i].x < min_x):
            min_x = points[i].x
        if (points[i].y > max_y):
            max_y = points[i].y
        if (points[i].y < min_y):
            min_y = points[i].y

    if (pt.x > min_x and pt.x < max_x and pt.y > min_y and pt.y < max_y):
        return True

    return False

def enclosed_point(points, num, pt):
    neg = 0
    pos = 0
    for i in range(0, num):
        v1 = perp_vector(create_vector(points[i], points[(i+1)%num]))
        v2 = create_vector(points[i], pt)
        dp = dot_product(v1, v2)
        if (dp > 0):
            pos += 1
        if (dp < 0):
            neg += 1

    if (pos == num):
        return True
    if (neg == num):
        return True

    return False

def main():
    CIRCLE_RADIUS = 20
    all_points =[point() for i in range(4 * CIRCLE_RADIUS * CIRCLE_RADIUS)]
    num_points = 0

    for xx in range(-CIRCLE_RADIUS, CIRCLE_RADIUS+1):
        for yy in range(-CIRCLE_RADIUS, CIRCLE_RADIUS+1):
             if ((xx*xx + yy*yy) < CIRCLE_RADIUS*CIRCLE_RADIUS):
                all_points[num_points].x = xx
                all_points[num_points].y = yy
                num_points += 1
	
    triangles = 0
    bounded = 0
    enclosed = 0
    pt = point()
    for a in range(0, num_points):
        for b in range(a + 1,num_points):
            for c in range(b + 1,num_points):
                triangles += 1
                points = [point(), point(), point()]
                points[0] = all_points[a]
                points[1] = all_points[b]
                points[2] = all_points[c]
                if (bounded_point(points, 3, pt)):
                    bounded += 1
                    if (enclosed_point(points, 3, pt)):
                        enclosed += 1
	
    print("Valid points       : {}".format(num_points))
    print("Valid triangles    : {}".format(triangles))
    print("Bounded by points  : {}".format(bounded))
    print("Enclosed by points : {}".format(enclosed))

if __name__ == "__main__":
    main()
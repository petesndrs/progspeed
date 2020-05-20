class tri
{

	final int CIRCLE_RADIUS = 20;

	public class point
	{
		public int x;
		public int y;
	};

	public class vector
	{
		public int vx;
		public int vy;
	};

	vector create_vector(point p1, point p2)
	{
		vector v = new vector();
		v.vx = p1.x - p2.x;
		v.vy = p1.y - p2.y;
		return v;
	}

	vector perp_vector(vector v)
	{
		vector vPerp = new vector();
		vPerp.vx = -v.vy;
		vPerp.vy = v.vx;
		return vPerp;
	}

	int dot_product(vector v1, vector v2)
	{
		return (v1.vx*v2.vx) + (v1.vy*v2.vy); 
	}

	boolean bounded_point(point[] points, int num, point pt)
	{
		int max_x = points[0].x;
		int min_x = points[0].x;
		int max_y = points[0].y;
		int min_y = points[0].y;
		int i;
		for (i = 1; i < num; ++i)
		{
			if (points[i].x > max_x) max_x = points[i].x;
			if (points[i].x < min_x) min_x = points[i].x;
			if (points[i].y > max_y) max_y = points[i].y;
			if (points[i].y < min_y) min_y = points[i].y;
		}
		if (pt.x > min_x && pt.x < max_x && pt.y > min_y && pt.y < max_y)
		{
			return true;
		}
		return false;
	}

	boolean enclosed_point(point[] points, int num, point pt)
	{
		int neg = 0, pos = 0;
		int i;
		for (i = 0; i < num; ++i)
		{
			vector v1 = perp_vector(create_vector(points[i], points[(i+1)%num]));
			vector v2 = create_vector(points[i], pt);
			int dp = dot_product(v1, v2);
			//printf("i %d, dp %d\n", i, dp);
			if (dp > 0) pos++;
			if (dp < 0) neg++;
		}
		//printf("pos %d, neg %d\n", pos, neg);
		if (pos == num) return true;
		if (neg == num) return true;
		return false;
	}

	public void run()
	{
		point[] all_points = new point[4 * CIRCLE_RADIUS * CIRCLE_RADIUS];
		int num_points = 0;
		int xx, yy;
		for (xx = -CIRCLE_RADIUS; xx <= CIRCLE_RADIUS; ++xx)
		{
			for (yy = -CIRCLE_RADIUS; yy <= CIRCLE_RADIUS; ++yy)
			{
				if ( (xx*xx + yy*yy) < CIRCLE_RADIUS*CIRCLE_RADIUS )
				{
					all_points[num_points] = new point();
					all_points[num_points].x = xx;
					all_points[num_points].y = yy;
					num_points++;
				}
			}
		}
		
		int triangles = 0;
		int bounded = 0;
		int enclosed = 0;
		final point pt = new point();
		int a, b, c;
		for (a = 0; a < num_points; ++a)
		{
			for (b = a + 1; b < num_points; ++b)
			{
				for (c = b + 1; c < num_points; ++c)
				{
					triangles++;
					point[] points = new point[3];
					points[0] = all_points[a];
					points[1] = all_points[b];
					points[2] = all_points[c];
					if (bounded_point(points, 3, pt))
					{
						bounded++;
						if (enclosed_point(points, 3, pt))
						{
							enclosed++;
						}
					}
				}
			}
		}
		
		System.out.printf("Valid points       : %d\n", num_points);
		System.out.printf("Valid triangles    : %d\n", triangles);
		System.out.printf("Bounded by points  : %d\n", bounded);
		System.out.printf("Enclosed by points : %d\n", enclosed);
	}

	public static void main(String args[])
	{
		tri myClass = new tri();
		myClass.run();
	}

}
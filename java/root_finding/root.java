import java.lang.Math.*;

class root
{

	final static double THRESHOLD = 0.000000001;

	private interface Callable {
		public double call(double param);
	}

	private class problem {
		public String name;
		public double lowX;
		public double highX;
		public Callable function;
		public Callable derivative;
		
		public problem(String a_name, double a_lowX, double a_highX, Callable a_function, Callable a_derivative)
		{
			name = a_name;
			lowX = a_lowX;
			highX = a_highX;
			function = a_function;
			derivative = a_derivative;
		}
	};

	private void bisection(int count, double lowX, double highX, Callable function){

		double newX = (highX + lowX) / 2.0;
		double newY = function.call(newX);
		if (Math.abs(newY) < THRESHOLD){
			System.out.printf("Iterations=%d, X=%f\n", count, newX);
			return;
		}

		double lowY = function.call(lowX);
		if ( (newY > 0 && lowY > 0) || (newY < 0 && lowY < 0) ){
			lowX = newX;
		}

		double highY = function.call(highX);
		if ( (newY > 0 && highY > 0) || (newY < 0 && highY < 0) ){
			highX = newX;
		}

	  if (count == 1){
			System.out.printf("BISEC: First X=%f, ", newX);
		}
		bisection(++count, lowX, highX, function);

		return;
	}

	private void falsePosition(int count, double lowX, double highX, Callable function){

		double lowY = function.call(lowX);
		double highY = function.call(highX);

		double newX = (lowX * highY - lowY * highX) /  (highY - lowY);

		double newY = function.call(newX);
		if (Math.abs(newY) < THRESHOLD){
			System.out.printf("Iterations=%d, X=%f\n", count, newX);
			return;
		}

		if ( (newY > 0 && lowY > 0) || (newY < 0 && lowY < 0) ){
			lowX = newX;
		}
		if ( (newY > 0 && highY > 0) || (newY < 0 && highY < 0) ){
			highX = newX;
		}

		if (count == 1){
			System.out.printf("FPLIN: First X=%f, ", newX);
		}
		falsePosition(++count, lowX, highX, function);

		return;
	}

	void newtonRaphson(int count, double X, Callable function, Callable derivative){

		double newX = X - (function.call(X)/derivative.call(X));

		double newY = function.call(newX);
		if (Math.abs(newY) < THRESHOLD){
			System.out.printf("Iterations=%d, X=%f\n", count, newX);
			return;
		}

		if (count == 1){
			System.out.printf("NRAPH: First X=%f, ", newX);
		}
		newtonRaphson(++count, newX, function, derivative);

		return;
	}

	void secant(int count, double x0, double x1, Callable function){
		double fx0 = function.call(x0);
		double fx1 = function.call(x1);
		double newX = (x0 * fx1 - x1 * fx0) / (fx1 - fx0);

		double newY = function.call(newX);
		if (Math.abs(newY) < THRESHOLD){
			System.out.printf("Iterations=%d, X=%f\n", count, newX);
			return;
		}

		if (count == 1){
			System.out.printf("SECNT: First X=%f, ", newX);
		}
		secant(++count, x1, newX, function);

		return;
	}

	double quadraticRoot(double a, double b, double c, double lowX, double highX){

		double x1 = ((-1.0 * b) + Math.sqrt(b*b - 4.0*a*c)) / (2.0*a);
		double x2 = ((-1.0 * b) - Math.sqrt(b*b - 4.0*a*c)) / (2.0*a);

		if (x1 >= lowX && x1 <= highX){
			return x1;
		}
		if (x2 >= lowX && x2 <= highX) {
			return x2;
		}

		System.out.printf("Divergent: Roots %g %g\n", x1, x2);
		return 0.0;
	}

	void falsePositionQuadraticClosest(int count, double lowX, double highX, Callable function, Callable derivative){

		double lowY = function.call(lowX);
		double highY = function.call(highX);

		// Use derivative from closest point to y=0
		double a, b, c, deriv;
		if (Math.abs(lowY) < Math.abs(highY)){
			deriv = derivative.call(lowX);
			a = (lowY - highY + (deriv * (highX - lowX))) / ((lowX - highX) * (highX - lowX));
			b = deriv - (2 * a * lowX);
			c = lowY - (deriv * lowX) + (a * lowX * lowX);
		} else {
			deriv = derivative.call(highX);
			a = (highY - lowY + (deriv * (lowX - highX))) / ((highX - lowX) * (lowX - highX));
			b = deriv - (2 * a * highX);
			c = highY - (deriv * highX) + (a * highX * highX);
		}

		double newX = quadraticRoot(a, b, c, lowX, highX);

		double newY = function.call(newX);
		if (Math.abs(newY) < THRESHOLD){
			System.out.printf("Iterations=%d, X=%f\n", count, newX);
			return;
		}

		if ( (newY > 0 && lowY > 0) || (newY < 0 && lowY < 0) ){
			lowX = newX;
		}
		if ( (newY > 0 && highY > 0) || (newY < 0 && highY < 0) ){
			highX = newX;
		}

		if (count == 1){
			System.out.printf("FPQDC: First X=%f, ", newX);
		}
		falsePositionQuadraticClosest(++count, lowX, highX, function, derivative);

		return;
	}

	void falsePositionQuadraticSteepest(int count, double lowX, double highX, Callable function, Callable derivative){

		double lowY = function.call(lowX);
		double highY = function.call(highX);

		// Use steepest derivative
		double a, b, c, deriv;
		if (Math.abs(derivative.call(lowX)) > Math.abs(derivative.call(highX))){
			deriv = derivative.call(lowX);
			a = (lowY - highY + (deriv * (highX - lowX))) / ((lowX - highX) * (highX - lowX));
			b = deriv - (2 * a * lowX);
			c = lowY - (deriv * lowX) + (a * lowX * lowX);
		} else {
			deriv = derivative.call(highX);
			a = (highY - lowY + (deriv * (lowX - highX))) / ((highX - lowX) * (lowX - highX));
			b = deriv - (2 * a * highX);
			c = highY - (deriv * highX) + (a * highX * highX);
		}

		double newX = quadraticRoot(a, b, c, lowX, highX);

		double newY = function.call(newX);
		if (Math.abs(newY) < THRESHOLD){
			System.out.printf("Iterations=%d, X=%f\n", count, newX);
			return;
		}

		if ( (newY > 0 && lowY > 0) || (newY < 0 && lowY < 0) ){
			lowX = newX;
		}
		if ( (newY > 0 && highY > 0) || (newY < 0 && highY < 0) ){
			highX = newX;
		}

		if (count == 1){
			System.out.printf("FPQDS: First X=%f, ", newX);
		}
		falsePositionQuadraticSteepest(++count, lowX, highX, function, derivative);

		return;
	}

	void falsePositionTangents(int count, double lowX, double highX, Callable function, Callable derivative){

		double lowY = function.call(lowX);
		double highY = function.call(highX);

		double lowM = derivative.call(lowX);
		double lowC = lowY - (lowM * lowX);
		double highM = derivative.call(highX);
		double highC = highY - (highM * highX);

		double crossY = ((lowC * highM) - (highC * lowM))/(highM - lowM);
		double crossX = (highC - lowC)/(lowM - highM);

		double newX = 0.0f;
		if (crossX > lowX && crossX < highX){
			// tangents cross inside limits, decide on which tangent
			if ( (crossY > 0 && lowY > 0) || (crossY < 0 && lowY < 0) ){
				newX = -1.0 * (highC/highM);
			}
			if ( (crossY > 0 && highY > 0) || (crossY < 0 && highY < 0) ){
				newX = -1.0 * (lowC/lowM);
			}

		} else {
			// tangents cross outside limits so use line
			newX = (lowX * highY - lowY * highX) /  (highY - lowY);

		}

		double newY = function.call(newX);
		if (Math.abs(newY) < THRESHOLD){
			System.out.printf("Iterations=%d, X=%f\n", count, newX);
			return;
		}

		if ( (newY > 0 && lowY > 0) || (newY < 0 && lowY < 0) ){
			lowX = newX;
		}
		if ( (newY > 0 && highY > 0) || (newY < 0 && highY < 0) ){
			highX = newX;
		}

		if (count == 1){
			System.out.printf("FPTAN: First X=%f, ", newX);
		}
		falsePositionTangents(++count, lowX, highX, function, derivative);

		return;
	}

    private class f6 implements Callable
	{	
		public double call(double x){
			return Math.cos(x) - x;
		}
	}

	private class d6 implements Callable
	{
		public double call(double x){
			return (-1.0 * Math.sin(x)) - 1.0;
		}
	}
	
	private class f5 implements Callable
	{
		public double call(double x){
			return Math.exp(x) + x;
		}
	}
	
	private class d5 implements Callable
	{
		public double call(double x){
			return Math.exp(x) + 1.0;
		}
	}
	
	private class f4 implements Callable
	{
		public double call(double x){
			return Math.exp(x) - x*x;
		}
	}
	
	private class d4 implements Callable
	{
		public double call(double x){
			return Math.exp(x) - 2.0*x;
		}
	}
	
	private class f3 implements Callable
	{
		public double call(double x){
			return Math.pow(x,3) - 3*x - 8.0;
		}
	}
	
	private class d3 implements Callable
	{
		public double call(double x){
			return 3*x*x -3;
		}
	}
	
	private class f2 implements Callable
	{
		public double call(double x){
			return Math.exp(-1.0 * x) - 0.5;
		}
	}

	private class d2 implements Callable
	{
		public double call(double x){
			return -1.0 * Math.exp(-1.0 * x);
		}
	}

	private class f1 implements Callable
	{
		public double call(double x){
			return Math.pow(x,4) + x - 1;
		}
	}

	private class d1 implements Callable
	{
		public double call(double x){
			return (4*Math.pow(x,3)) + 1;
		}
	}

	public void run() {
        final int NUMBER_OF_PROBLEMS = 6;
		final problem[] problems = new problem[NUMBER_OF_PROBLEMS];
		int i = 0;
		problems[i++] = new problem("x^4 + x - 1", 0.0f, 4.0f, new f1(), new d1());
		problems[i++] = new problem("exp(-x) - 0.5", 0.0f, 4.0f, new f2(), new d2());
		problems[i++] = new problem("x^3 - 3x - 8", 2.0f, 5.0f, new f3(), new d3());
		problems[i++] = new problem("exp(x) - x^2", -2.0f, 2.0f, new f4(), new d4());
		problems[i++] = new problem("exp(x) + x", -4.0f, 0.0f, new f5(), new d5());
		problems[i++] = new problem("cos(x) - x", 0.0f, 3.0f, new f6(), new d6());
		
		int numberOfProblems = i;

		for (i = 0; i < numberOfProblems; ++i){
			System.out.printf("%s in range (%.0f:%.0f)\n",problems[i].name, problems[i].lowX, problems[i].highX);
			bisection(1, problems[i].lowX, problems[i].highX, problems[i].function);
			falsePosition(1, problems[i].lowX, problems[i].highX, problems[i].function);
			newtonRaphson(1, problems[i].lowX, problems[i].function, problems[i].derivative);
			newtonRaphson(1, problems[i].highX, problems[i].function, problems[i].derivative);
			secant(1, problems[i].lowX, problems[i].highX, problems[i].function);
			secant(1, problems[i].highX, problems[i].lowX, problems[i].function);
			falsePositionQuadraticClosest(1, problems[i].lowX, problems[i].highX, problems[i].function, problems[i].derivative);
			falsePositionQuadraticSteepest(1, problems[i].lowX, problems[i].highX, problems[i].function, problems[i].derivative);
			falsePositionTangents(1, problems[i].lowX, problems[i].highX, problems[i].function, problems[i].derivative);
		}
	}

	public static void main(String args[])
	{
		root myClass = new root();
		myClass.run();
	}

}

#include <stdio.h>
#include <math.h>

#define THRESHOLD (double)(0.000000001)

struct problem {
	char* name;
    double lowX;
	double highX;
	double (*function)(double);
    double (*derivative)(double);
};

void bisection(int count, double lowX, double highX, double (*function)(double)){

    double newX = (highX + lowX) / 2.0;
	double newY = function(newX);
	if (fabs(newY) < THRESHOLD){
	    printf("BISECT: Iterations %d, X=%g Y=%g\n", count, newX, newY);
		return;
	}

	double lowY = function(lowX);
	if ( (newY > 0 && lowY > 0) || (newY < 0 && lowY < 0) ){
	    lowX = newX;
	}

	double highY = function(highX);
	if ( (newY > 0 && highY > 0) || (newY < 0 && highY < 0) ){
	    highX = newX;
	}

	bisection(++count, lowX, highX, function);

    return;
}

void falsePosition(int count, double lowX, double highX, double (*function)(double)){
	
	double lowY = function(lowX);
	double highY = function(highX);
	
	double newX = (lowX * highY - lowY * highX) /  (highY - lowY); 
	
	double newY = function(newX);
	if (fabs(newY) < THRESHOLD){
	    printf("FPLINR: Iterations %d, X=%g Y=%g\n", count, newX, newY);
		return;
	}

	if ( (newY > 0 && lowY > 0) || (newY < 0 && lowY < 0) ){
	    lowX = newX;
	}	
	if ( (newY > 0 && highY > 0) || (newY < 0 && highY < 0) ){
	    highX = newX;
	}

	falsePosition(++count, lowX, highX, function);

    return;
}

void newtonRaphson(int count, double X, double (*function)(double), double (*derivative)(double)){
	
	double newX = X - (function(X)/derivative(X));
	
	double newY = function(newX);
	if (fabs(newY) < THRESHOLD){
	    printf("NRAPH: Iterations %d, X=%g Y=%g\n", count, newX, newY);
		return;
	}

	newtonRaphson(++count, newX, function, derivative);

    return;
}

void secant(int count, double x0, double x1, double (*function)(double)){
	double fx0 = function(x0);
	double fx1 = function(x1);
    double newX = (x0 * fx1 - x1 * fx0) / (fx1 - fx0); 	

    double newY = function(newX);
	if (fabs(newY) < THRESHOLD){
	    printf("SCANT: Iterations %d, X=%g Y=%g\n", count, newX, newY);
		return;
	}

	secant(++count, x1, newX, function);

    return;
}

double quadraticRoot(double a, double b, double c, double lowX, double highX){
	
	double x1 = ((-1.0 * b) + sqrt(b*b - 4.0*a*c)) / (2.0*a);
	double x2 = ((-1.0 * b) - sqrt(b*b - 4.0*a*c)) / (2.0*a);
	
	if (x1 >= lowX && x1 <= highX){
		return x1;
	}
	if (x2 >= lowX && x2 <= highX) {
		return x2;
	}
	
	printf("Divergent: Roots %g %g\n", x1, x2);
	return 0.0;
}

void falsePositionQuadraticClosest(int count, double lowX, double highX, double (*function)(double), double (*derivative)(double)){
	
	double lowY = function(lowX);
	double highY = function(highX);

	// Use derivative from closest point to y=0
	double a, b, c, deriv;
	if (fabs(lowY) < fabs(highY)){
		deriv = derivative(lowX);
	    a = (lowY - highY + (deriv * (highX - lowX))) / ((lowX - highX) * (highX - lowX));
		b = deriv - (2 * a * lowX);
		c = lowY - (deriv * lowX) + (a * lowX * lowX); 
	} else {
		deriv = derivative(highX);
	    a = (highY - lowY + (deriv * (lowX - highX))) / ((highX - lowX) * (lowX - highX));
		b = deriv - (2 * a * highX);
		c = highY - (deriv * highX) + (a * highX * highX);
    }
	
	double newX = quadraticRoot(a, b, c, lowX, highX);
	
	double newY = function(newX);
	if (fabs(newY) < THRESHOLD){
	    printf("FPQDC: Iterations %d, X=%g Y=%g\n", count, newX, newY);
		return;
	}

	if ( (newY > 0 && lowY > 0) || (newY < 0 && lowY < 0) ){
	    lowX = newX;
	}	
	if ( (newY > 0 && highY > 0) || (newY < 0 && highY < 0) ){
	    highX = newX;
	}

	falsePositionQuadraticClosest(++count, lowX, highX, function, derivative);

    return;
}

void falsePositionQuadraticSteepest(int count, double lowX, double highX, double (*function)(double), double (*derivative)(double)){
	
	double lowY = function(lowX);
	double highY = function(highX);

	// Use steepest derivative
	double a, b, c, deriv;
	if (fabs(derivative(lowX)) > fabs(derivative(highX))){
		deriv = derivative(lowX);
	    a = (lowY - highY + (deriv * (highX - lowX))) / ((lowX - highX) * (highX - lowX));
		b = deriv - (2 * a * lowX);
		c = lowY - (deriv * lowX) + (a * lowX * lowX); 
	} else {
		deriv = derivative(highX);
	    a = (highY - lowY + (deriv * (lowX - highX))) / ((highX - lowX) * (lowX - highX));
		b = deriv - (2 * a * highX);
		c = highY - (deriv * highX) + (a * highX * highX);
    }
	
	double newX = quadraticRoot(a, b, c, lowX, highX);
	
	double newY = function(newX);
	if (fabs(newY) < THRESHOLD){
	    printf("FPQDD: Iterations %d, X=%g Y=%g\n", count, newX, newY);
		return;
	}

	if ( (newY > 0 && lowY > 0) || (newY < 0 && lowY < 0) ){
	    lowX = newX;
	}	
	if ( (newY > 0 && highY > 0) || (newY < 0 && highY < 0) ){
	    highX = newX;
	}

	falsePositionQuadraticSteepest(++count, lowX, highX, function, derivative);

    return;
}

void falsePositionTangents(int count, double lowX, double highX, double (*function)(double), double (*derivative)(double)){

	double lowY = function(lowX);
	double highY = function(highX);

	double lowM = derivative(lowX);
	double lowC = lowY - (lowM * lowX);
	double highM = derivative(highX);
	double highC = highY - (highM * highX);

	double crossY = ((lowC * highM) - (highC * lowM))/(highM - lowM);
	double crossX = (highC - lowC)/(lowM - highM);

	double newX;
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

	double newY = function(newX);
	if (fabs(newY) < THRESHOLD){
	    printf("FPTAN: Iterations %d, X=%g Y=%g\n", count, newX, newY);
		return;
	}

	if ( (newY > 0 && lowY > 0) || (newY < 0 && lowY < 0) ){
	    lowX = newX;
	}
	if ( (newY > 0 && highY > 0) || (newY < 0 && highY < 0) ){
	    highX = newX;
	}

	falsePositionTangents(++count, lowX, highX, function, derivative);

    return;
}

double f5(double x){
	return exp(x) + x;
}

double d5(double x){
	return exp(x) + 1.0;
}

double f4(double x){
	return exp(x) - x*x;
}

double d4(double x){
	return exp(x) - 2.0*x;
}

double f3(double x){
	return pow(x,3) - 3*x - 8.0;
}

double d3(double x){
	return 3*x*x -3;
}

double f2(double x){
    return exp(-1.0 * x) - 0.5;
}

double d2(double x){
    return -1.0 * exp(-1.0 * x);
}

double f1(double x){
    return pow(x,4) + x - 1; 
}

double d1(double x){
	return (4*pow(x,3)) + 1;
}

int main() {

    struct problem problems[] = {
		{"Polynomial: x^4+x-1", 0.0, 4.0, f1, d1},
		{"Neg exponential: exp(-x)-0.5", 0.0, 4.0, f2, d2},
		{"Polynomial: x^3-3x-8", 2.0, 5.0, f3, d3},
		{"Exponential: exp(x) - x*x", -2.0, 2.0, f4, d4},
		{"Exponential: exp(x) + x", -4.0, 0.0, f5, d5},
	};
	
	int numberOfProblems = sizeof(problems)/sizeof(struct problem);
	
	for (int i = 0; i < numberOfProblems; ++i){
        printf("%s\n",problems[i].name);
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
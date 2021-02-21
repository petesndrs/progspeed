package main

import "fmt"
import "math"

const THRESHOLD float64 = 0.000000001

type problem struct {
	name string
	lowX float64
	highX float64
	function func(float64) float64
	derivative func(float64) float64
}

func bisection(count int, lowX float64, highX float64, function func(float64) float64){

	newX := (highX + lowX) / 2.0
	newY := function(newX)
	if (math.Abs(newY) < THRESHOLD){
		fmt.Printf("Iterations=%d, X=%f\n", count, newX)
		return
	}

	lowY := function(lowX)
	if ( (newY > 0 && lowY > 0) || (newY < 0 && lowY < 0) ){
		lowX = newX
	}

	highY := function(highX)
	if ( (newY > 0 && highY > 0) || (newY < 0 && highY < 0) ){
		highX = newX
	}

	if (count == 1){
		fmt.Printf("BISEC: First X=%f, ", newX)
	}
	count++
	bisection(count, lowX, highX, function)

	return
}

func falsePosition(count int, lowX float64, highX float64, function func(float64) float64){

	lowY := function(lowX)
	highY := function(highX)

	newX := (lowX * highY - lowY * highX) / (highY - lowY)

	newY := function(newX)
	if (math.Abs(newY) < THRESHOLD){
		fmt.Printf("Iterations=%d, X=%f\n", count, newX)
		return
	}

	if ( (newY > 0 && lowY > 0) || (newY < 0 && lowY < 0) ){
		lowX = newX
	}
	if ( (newY > 0 && highY > 0) || (newY < 0 && highY < 0) ){
		highX = newX
	}

	if (count == 1){
		fmt.Printf("FPLIN: First X=%f, ", newX)
	}
	count++
	falsePosition(count, lowX, highX, function)

	return
}

func newtonRaphson(count int, X float64, function func(float64) float64, derivative func(float64) float64){

	newX := X - (function(X)/derivative(X))

	newY := function(newX)
	if (math.Abs(newY) < THRESHOLD){
		fmt.Printf("Iterations=%d, X=%f\n", count, newX)
		return
	}

	if (count == 1){
		fmt.Printf("NRAPH: First X=%f, ", newX)
	}
	count++
	newtonRaphson(count, newX, function, derivative)

	return
}

func secant(count int, x0 float64, x1 float64, function func(float64) float64){
	fx0 := function(x0)
	fx1 := function(x1)
	newX := (x0 * fx1 - x1 * fx0) / (fx1 - fx0)

	newY := function(newX)
	if (math.Abs(newY) < THRESHOLD){
		fmt.Printf("Iterations=%d, X=%f\n", count, newX)
		return
	}

	if (count == 1){
		fmt.Printf("SECNT: First X=%f, ", newX)
	}
	count++
	secant(count, x1, newX, function)

	return
}

func quadraticRoot(a float64, b float64, c float64, lowX float64, highX float64) float64 {

	x1 := ((-1.0 * b) + math.Sqrt(b*b - 4.0*a*c)) / (2.0*a)
	x2 := ((-1.0 * b) - math.Sqrt(b*b - 4.0*a*c)) / (2.0*a)

	if (x1 >= lowX && x1 <= highX){
		return x1
	}
	if (x2 >= lowX && x2 <= highX) {
		return x2
	}

	fmt.Printf("Divergent: Roots %g %g\n", x1, x2)
	return 0.0
}

func falsePositionQuadraticClosest(count int, lowX float64, highX float64, function func(float64) float64, derivative func(float64) float64){

	lowY := function(lowX)
	highY := function(highX)

	// Use derivative from closest point to y=0
	var a, b, c, deriv float64
	if (math.Abs(lowY) < math.Abs(highY)){
		deriv = derivative(lowX)
		a = (lowY - highY + (deriv * (highX - lowX))) / ((lowX - highX) * (highX - lowX))
		b = deriv - (2 * a * lowX)
		c = lowY - (deriv * lowX) + (a * lowX * lowX)
	} else {
		deriv = derivative(highX)
		a = (highY - lowY + (deriv * (lowX - highX))) / ((highX - lowX) * (lowX - highX))
		b = deriv - (2 * a * highX)
		c = highY - (deriv * highX) + (a * highX * highX)
	}

	newX := quadraticRoot(a, b, c, lowX, highX)

	newY := function(newX)
	if (math.Abs(newY) < THRESHOLD){
		fmt.Printf("Iterations=%d, X=%f\n", count, newX)
		return
	}

	if ( (newY > 0 && lowY > 0) || (newY < 0 && lowY < 0) ){
		lowX = newX
	}
	if ( (newY > 0 && highY > 0) || (newY < 0 && highY < 0) ){
		highX = newX
	}

	if (count == 1){
		fmt.Printf("FPQDC: First X=%f, ", newX)
	}
	count++
	falsePositionQuadraticClosest(count, lowX, highX, function, derivative)

	return
}

func falsePositionQuadraticSteepest(count int, lowX float64, highX float64, function func(float64) float64, derivative func(float64) float64){

	lowY := function(lowX)
	highY := function(highX)

	// Use steepest derivative
	var a, b, c, deriv float64
	if (math.Abs(derivative(lowX)) > math.Abs(derivative(highX))){
		deriv = derivative(lowX)
		a = (lowY - highY + (deriv * (highX - lowX))) / ((lowX - highX) * (highX - lowX))
		b = deriv - (2 * a * lowX)
		c = lowY - (deriv * lowX) + (a * lowX * lowX)
	} else {
		deriv = derivative(highX)
		a = (highY - lowY + (deriv * (lowX - highX))) / ((highX - lowX) * (lowX - highX))
		b = deriv - (2 * a * highX)
		c = highY - (deriv * highX) + (a * highX * highX)
	}

	newX := quadraticRoot(a, b, c, lowX, highX)

	newY := function(newX)
	if (math.Abs(newY) < THRESHOLD){
		fmt.Printf("Iterations=%d, X=%f\n", count, newX)
		return
	}

	if ( (newY > 0 && lowY > 0) || (newY < 0 && lowY < 0) ){
		lowX = newX
	}
	if ( (newY > 0 && highY > 0) || (newY < 0 && highY < 0) ){
		highX = newX
	}

	if (count == 1){
		fmt.Printf("FPQDS: First X=%f, ", newX)
	}
	count++
	falsePositionQuadraticSteepest(count, lowX, highX, function, derivative)

	return
}

func falsePositionTangents(count int, lowX float64, highX float64, function func(float64) float64, derivative func(float64) float64){

	lowY := function(lowX)
	highY := function(highX)

	lowM := derivative(lowX)
	lowC := lowY - (lowM * lowX)
	highM := derivative(highX)
	highC := highY - (highM * highX)

	crossY := ((lowC * highM) - (highC * lowM))/(highM - lowM)
	crossX := (highC - lowC)/(lowM - highM)

	var newX float64
	if (crossX > lowX && crossX < highX){
		// tangents cross inside limits, decide on which tangent
		if ( (crossY > 0 && lowY > 0) || (crossY < 0 && lowY < 0) ){
			newX = -1.0 * (highC/highM)
		}
		if ( (crossY > 0 && highY > 0) || (crossY < 0 && highY < 0) ){
			newX = -1.0 * (lowC/lowM)
		}
	} else {
		// tangents cross outside limits so use line
		newX = (lowX * highY - lowY * highX) / (highY - lowY)
	}

	newY := function(newX)
	if (math.Abs(newY) < THRESHOLD){
		fmt.Printf("Iterations=%d, X=%f\n", count, newX)
		return
	}

	if ( (newY > 0 && lowY > 0) || (newY < 0 && lowY < 0) ){
		lowX = newX
	}
	if ( (newY > 0 && highY > 0) || (newY < 0 && highY < 0) ){
		highX = newX
	}

	if (count == 1){
		fmt.Printf("FPTAN: First X=%f, ", newX)
	}
	count++
	falsePositionTangents(count, lowX, highX, function, derivative)

	return
}

func f6(x float64) float64 {
	return math.Cos(x) - x
}

func d6(x float64) float64 {
	return (-1.0 * math.Sin(x)) - 1.0
}

func f5(x float64) float64 {
	return math.Exp(x) + x
}

func d5(x float64) float64 {
	return math.Exp(x) + 1.0
}

func f4(x float64) float64 {
	return math.Exp(x) - x*x
}

func d4(x float64) float64 {
	return math.Exp(x) - 2.0*x
}

func f3(x float64) float64 {
	return math.Pow(x,3) - 3*x - 8.0
}

func d3(x float64) float64 {
	return 3*x*x -3
}

func f2(x float64) float64 {
	return math.Exp(-1.0 * x) - 0.5
}

func d2(x float64) float64 {
	return -1.0 * math.Exp(-1.0 * x)
}

func f1(x float64) float64 {
	return math.Pow(x,4) + x - 1
}

func d1(x float64) float64 {
	return (4*math.Pow(x,3)) + 1
}

func main() {

	var problems = []problem {
		{"x^4 + x - 1", 0.0, 4.0, f1, d1},
		{"exp(-x) - 0.5", 0.0, 4.0, f2, d2},
		{"x^3 - 3x - 8", 2.0, 5.0, f3, d3},
		{"exp(x) - x^2", -2.0, 2.0, f4, d4},
		{"exp(x) + x", -4.0, 0.0, f5, d5},
		{"cos(x) - x", 0.0, 3.0, f6, d6},
	}

	numberOfProblems := len(problems)

	for i := 0; i < numberOfProblems; i++{
		fmt.Printf("%s in range (%g:%g)\n",problems[i].name, problems[i].lowX, problems[i].highX)
		bisection(1, problems[i].lowX, problems[i].highX, problems[i].function)
		falsePosition(1, problems[i].lowX, problems[i].highX, problems[i].function)
		newtonRaphson(1, problems[i].lowX, problems[i].function, problems[i].derivative)
		newtonRaphson(1, problems[i].highX, problems[i].function, problems[i].derivative)
		secant(1, problems[i].lowX, problems[i].highX, problems[i].function)
		secant(1, problems[i].highX, problems[i].lowX, problems[i].function)
		falsePositionQuadraticClosest(1, problems[i].lowX, problems[i].highX, problems[i].function, problems[i].derivative)
		falsePositionQuadraticSteepest(1, problems[i].lowX, problems[i].highX, problems[i].function, problems[i].derivative)
		falsePositionTangents(1, problems[i].lowX, problems[i].highX, problems[i].function, problems[i].derivative)
	}
}

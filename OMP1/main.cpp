#include <omp.h>
#include <iostream>
using namespace std;

double integrate(double x) {
	result = 4.0 / (1.0 + x*x);
	return result;
}

int main() {
	int steps_count = 10000;
	double step = 1.0 / steps_count;
	
	double sum1 = 0.0, x1 = 0.0;
	//Integrate
    for (int i = 0; i < steps_count; i++) {
	x1 = step * i;
        sum1 += integrate(x1);
	}
	
	double result1 = step * sum1;
	cout << result1;
	

        double sum2 = 0.0, x2 = 0.0;
        //Parallel integrate
    #pragma omp parallel for private(x2)
    for (int i = 0; i < steps_count; i++) {
	x2 = step * i;
        sum2 += integrate(x2);
	}
	
	double result2 = step * sum2;
	cout << result2;


	double sum3 = 0.0, x3 = 0.0;
	//Integrate with Reduction
    #pragma omp parallel for private(x3) reduction(+ : sum3)
    for (int i = 0; i < steps_count; i++) {
	x3 = step * i;
        sum3 += integrate(x3);
	}
	
	double result3 = step * sum3;
	cout << result3;

	return 0;
}

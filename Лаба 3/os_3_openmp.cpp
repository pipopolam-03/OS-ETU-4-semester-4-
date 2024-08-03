#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <math.h>
#include <omp.h>


using namespace std;

#define N 100000000

double pi = 0.0;

int main() {
    int i;


#pragma omp parallel for schedule(dynamic, 15) reduction(+:pi)
    for (i = 0; i < N; i++) {
        pi += 4.0 / (1.0 + pow((i + 0.5) / N, 2));
    }

    cout << fixed << setprecision(15) << pi / N << endl;

    return 0;
}
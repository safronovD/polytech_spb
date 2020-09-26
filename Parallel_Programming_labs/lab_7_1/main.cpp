#include <iostream>
#include <cmath>
#include <omp.h>

const int N = 1000;

int main() {
    double a[N];
    a[0] = 1;
    double w, comp = 1;
    int i;

# pragma omp parallel for private(w, i) shared(a) reduction(*:comp)
    for (i = 1; i < N; i++) {
        w = sin(i / N * 3.14);
        comp *= w;
        a[i] = comp;
    }
}
#include <iostream>
#include <omp.h>
#include <ctime>
#include <algorithm>

#include "util.h"

#define N 500

class Vector {
public:
    double x, y, z;

    Vector() : x(0), y(0), z(0) {}
    Vector(double x, double y, double z) : x(x), y(y), z(z) {}

    bool operator==(const Vector &other) const {
        return (x == other.x) && (y == other.y) && (y == other.y);
    }
};

bool isCompl(Vector a, Vector b, Vector c) {
    return 0 == (a.x * (b.y * c.z - b.z * c.y) + a.y * (b.z * c.x - b.x * c.z) + a.z * (b.x * c.y - b.y * c.x));
}

std::vector <Vector> createVectorSet (unsigned long len) {
    int min_value = -100;
    int max_value = 100;
    srand ( time(nullptr) );

    std::vector<Vector> result(len);
    int i;

#pragma omp parallel for private(i) shared(result)
    for (int i = 0; i < len; i++) {
        while (true) {
            Vector new_elem = Vector(rand() % (min_value - max_value + 1) + min_value,
                                     rand() % (min_value - max_value + 1) + min_value,
                                     rand() % (min_value - max_value + 1) + min_value);
            if (result.end() == std::find(result.begin(), result.end(), new_elem)) { //bad
                result[i] = new_elem;
                break;
            }
        }
    }

    return result;
}

int main() {
    std::vector<Vector> vectors = createVectorSet(N);
    std::vector<std::vector<unsigned long>> combinations = getCombinations(N, 3);
    std::vector<short> result(combinations.size(), 0);

    int i;
    unsigned t0;

    t0 = clock();
    for (i = 0; i < combinations.size(); i++) {
        if (isCompl(vectors[combinations[i][0]], vectors[combinations[i][1]], vectors[combinations[i][2]])) {
            result[i] = 1;
        }
    }
    std::cout << "Execution time: " << clock() - t0 << "\n";

    t0 = clock();
#pragma omp parallel for private(i) shared(vectors, combinations, result)
    for (i = 0; i < combinations.size(); i++) {
        if (isCompl(vectors[combinations[i][0]], vectors[combinations[i][1]], vectors[combinations[i][2]])) {
            result[i] = 1;
        }
    }
    std::cout << "Execution time with omp: " << clock() - t0 << "\n";

    return 0;
}

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <numeric>
#include <iterator>
#include "mpi.h"

const long int N = 100000;
const double eps = 0.00001;

double getVecNorm(const std::vector<double> &v) {
    return std::accumulate(v.begin(), v.end(), 0.0, [](const double &lhs, const double &rhs) {
        return std::fabs(lhs) + std::fabs(rhs);
    });
}

double getMatNorm(const std::vector<std::vector<double>> &m) {
    double max = 0, vec_norm;
    std::vector<double> temp(m.size());
    for (int i = 0; i < m[0].size(); i++) {
        std::generate(temp.begin(), temp.end(), [m, i, j = 0]() mutable {
            return m[j++][i];
        });
        vec_norm = getVecNorm(temp);
        if (vec_norm > max) {
            max = vec_norm;
        }
    }
    return max;
}

double generateData (std::vector<std::vector<double>> &A, std::vector<double> &b, std::vector<double> &x_) {
    std::srand(std::time(nullptr));

    std::generate(x_.begin(), x_.end(), []() { return (double) std::rand() / RAND_MAX * 20 - 10; });
    std::generate(A.begin(), A.end(), [i = 0]() mutable {
        std::vector<double> temp(N);
        std::generate(temp.begin(), temp.end(), [i, j = 0]() mutable {
            if (i != j++) {
                return (double) std::rand() / RAND_MAX * 20 - 10;
            } else {
                return (double) std::rand() / RAND_MAX * 20 + 10 * N;
            }
        });
        ++i;
        return temp;
    });

    std::generate(b.begin(), b.end(), [&A, &x_, i = 0]() mutable {
        double temp = 0.0;
        for (int j = 0; j < x_.size(); j++) {
            temp += A[i][j] * x_[j];
        }
        ++i;
        return temp;
    });
/*
    std::vector<std::vector<double>> alphaA(N, std::vector<double>(N));
    std::copy(A.begin(), A.end(), alphaA.begin());
    std::for_each(alphaA.begin(), alphaA.end(), [i = 0](std::vector<double> &v) mutable {
        std::transform(v.begin(), v.end(), v.begin(), [vi = v[i], i, j = 0](double k) mutable {
            if (i != j++) {
                return k / vi;
            } else {
                return 0.0;
            }
        });
        ++i;
    });
*/
    return 0.0;
}

void resolve (std::vector<std::vector<double>> &A, std::vector<double> &b, std::vector<double> &x) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Bcast(b.data(), b.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    std::for_each(A.begin(), A.end(), [](std::vector<double> &v) {
        MPI_Bcast(v.data(), v.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    });

    int iter_count, offset = (N / size) * (rank);
    double max_norm;
    if (rank != size - 1) {
        iter_count = N / size;
    } else {
        iter_count = N - N / size * rank;
    }
    std::vector<double> xn(iter_count);
    std::vector<double> norm(size);
    std::fill(x.begin(), x.end(), 0);

    do {
        norm[rank] = 0.0;
        auto begin_iterator = xn.begin() + rank * (N / size);
        if (rank != size - 1) {
            auto end_iterator = xn.begin() + rank * (N / size);
        }

        std::generate(xn.begin(), xn.end(), [&A, &b, &x, i = offset]() mutable {
            double next_xni = -b[i];

            std::for_each(x.cbegin(), x.cend(), [&A, &next_xni, i, j = 0](double xi) mutable {
                if (i != j) {
                    next_xni += A[i][j] * xi;
                }
                j++;
            });

            return next_xni / -A[i][i++];
        });

        std::for_each(xn.cbegin(), xn.cend(), [&x, &norm, rank, i = offset](double xni) mutable {
            double diff = std::fabs(x[i++] - xni);
            if (diff > norm[rank]) {
                norm[rank] = diff;
            }
        });

        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Allgather(xn.data(), N / size, MPI_DOUBLE, x.data(), N / size, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Allgather(&norm[rank], 1, MPI_DOUBLE, norm.data(), 1, MPI_DOUBLE, MPI_COMM_WORLD);

        for (int i = (N / size) * (size); i < N; i++) {
            if (rank == size - 1) {
                x[i] = xn[i - ((N / size) * (rank))];
            }
            MPI_Bcast(&x[i], 1, MPI_DOUBLE, size - 1, MPI_COMM_WORLD);
        }

        max_norm = *std::max_element(norm.cbegin(), norm.cend());
    } while (max_norm > eps);
}

int main(int argc, char** argv) {
    std::vector<std::vector<double>> A(N, std::vector<double>(N)), alphaA(N, std::vector<double>(N));
    std::vector<double> x_(N), b(N), x(N);

    int size, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        std::cout << "Generated matrix norm (must be < 1) : " << generateData(A, b, x_) << "\n";
    }

    if (rank == 0) {
        std::cout << "Data size = " << N << " Eps = " << eps << "\n";
    }

    double total_time = 0.0;

    MPI_Barrier(MPI_COMM_WORLD);
    total_time -= MPI_Wtime();
    resolve(A, b, x);
    MPI_Barrier(MPI_COMM_WORLD);
    total_time += MPI_Wtime();

    if (rank == 0) std::cout << "Total execution time = " << total_time << "\n";

    if (rank == 0) {
        std::cout << "Ideal resolution: ";
        std::copy(x_.cbegin(), x_.cbegin() + 10,
                  std::ostream_iterator<double>(std::cout, " "));
        std::cout << "\n";

        std::cout << "Counted resolution: ";
        std::copy(x.cbegin(), x.cbegin() + 10,
                  std::ostream_iterator<double>(std::cout, " "));
        std::cout << "\n";
    }

    MPI_Finalize();
}

#include <iostream>
#include <vector>
#include <ctime>
#include <omp.h>

#define THREADS_NUM 4
#define GROUPS_NUM 1000000

void createMatrix (int groups_num, std::vector<std::vector<int>> &matrix) {
    int min_mark = 3;
    int max_mark = 5;
    int min_students = 15;
    int max_students = 30;
    srand ( time(nullptr) );

    for (int i = 0; i < groups_num; i++) {
        int students_in_group = rand() % (max_students - min_students + 1) + min_students;
        matrix.push_back(std::vector<int>());
        for (int j = 0; j < students_in_group; j++) {
            int random_mark = rand() % (max_mark - min_mark + 1) + min_mark;
            matrix[i].push_back(random_mark);
        }
    }
}

void printResult (std::ostream &out, std::vector<std::vector<int>> &matrix, std::vector<double> &mean_groups, double global_mean) {
    for (int i = 0; i < matrix.size(); i++) {
        out << "Group values: ";
        for (int j = 0; j < matrix[i].size(); j++) {
            out << matrix[i][j] << " ";
        }
        out << " Mean: " << mean_groups[i] << "\n";
    }
    out << "Global mean: " << global_mean;
}

int main() {
    omp_set_num_threads(THREADS_NUM);

    std::vector<std::vector<int>> matrix = std::vector<std::vector<int>>();
    createMatrix(GROUPS_NUM, matrix);

    std::vector<double> mean_groups = std::vector<double>(matrix.size());
    double sum = 0;
    int group_sum, i, j;
    unsigned t0;

    t0 = clock();
    for (i = 0; i < matrix.size(); i++) {
        group_sum = 0;
        for (j = 0; j < matrix[i].size(); j++) {
            group_sum += matrix[i][j];
        }
        mean_groups[i] = (double)group_sum / matrix[i].size();
        sum += mean_groups[i];
    }
    std::cout << "Execution time: " << clock() - t0 << "\n";

    t0 = clock();
#pragma omp parallel for private(i, j, group_sum) shared(matrix, mean_groups) reduction(+:sum)
    for (i = 0; i < matrix.size(); i++) {
        group_sum = 0;
        for (j = 0; j < matrix[i].size(); j++) {
            group_sum += matrix[i][j];
        }
        mean_groups[i] = (double)group_sum / matrix[i].size();
        sum += mean_groups[i];
    }
    std::cout << "Execution time with omp: " << clock() - t0 << "\n";

    //printResult(std::cout, matrix, mean_groups, sum / matrix.size());
}

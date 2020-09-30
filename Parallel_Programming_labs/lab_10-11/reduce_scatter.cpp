#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <mpi.h>

const int num_trials = 1000000;

void my_reduce_scatter(void* data, int* result, MPI_Datatype datatype,
                       int (*op)(int, int), MPI_Comm communicator) {
    int world_rank;
    MPI_Comm_rank(communicator, &world_rank);
    int world_size;
    MPI_Comm_size(communicator, &world_size);

    int recv_data, temp_result = 0;

    for (int i = 0; i < world_size; i++) {
            MPI_Send(data + i * sizeof(MPI_INT) / 2, 1, datatype, i, 0, communicator);
    }

    for (int i = 0; i < world_size; i++) {
        MPI_Recv(&recv_data, 1, datatype, i, 0, communicator, MPI_STATUS_IGNORE);
        temp_result = (*op)(temp_result, recv_data);
    }

    *result = temp_result;
}


int main (int argc, char *argv[]) {
    int rank, result, size;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int data_len = size;
    std::vector<int> send_data(data_len, 0), recvcounts(data_len, 1);

    MPI_Barrier(MPI_COMM_WORLD);

    std::srand(std::time(nullptr) + rank);
    std::generate(send_data.begin(), send_data.end(), []() { return rand() % 100; });

    MPI_Reduce_scatter(send_data.data(), &result, recvcounts.data(), MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    std::cout << "Check MPI_Reduce_scatter " <<"Rank: " << rank << " Data:";
    std::for_each(send_data.begin(), send_data.end(), [](int n) {
       std::cout << " " << n;
    });
    std::cout << " Result: " << result << "\n";


    MPI_Barrier(MPI_COMM_WORLD);


    std::generate(send_data.begin(), send_data.end(), []() { return rand() % 100; });

    my_reduce_scatter(send_data.data(), &result, MPI_INT, [](int a, int b) { return a + b; }, MPI_COMM_WORLD);

    std::cout << "Check my_reduce_scatter " <<"Rank: " << rank << " Data:";
    std::for_each(send_data.begin(), send_data.end(), [](int n) {
        std::cout << " " << n;
    });
    std::cout << " Result: " << result << "\n";

    double total_time = 0;
    if (rank == 0 ) std::cout << "Data size = " << data_len << " Trials = " << num_trials << "\n";

    for (int i = 0; i < num_trials; i++) {
        MPI_Barrier(MPI_COMM_WORLD);
        total_time -= MPI_Wtime();
        MPI_Reduce_scatter(send_data.data(), &result, recvcounts.data(), MPI_INT, MPI_SUM, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        total_time += MPI_Wtime();
    }
    if (rank == 0 ) std::cout << "Avg MPI_Reduce_scatter time = " << total_time << "\n";

    total_time = 0;
    for (int i = 0; i < num_trials; i++) {
        MPI_Barrier(MPI_COMM_WORLD);
        total_time -= MPI_Wtime();
        my_reduce_scatter(send_data.data(), &result, MPI_INT, [](int a, int b) { return a + b; }, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        total_time += MPI_Wtime();
    }
    if (rank == 0 ) std::cout << "Avg my_reduce_scatter time = " << total_time << "\n";

    MPI_Finalize();
}

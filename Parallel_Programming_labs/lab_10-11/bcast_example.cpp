#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <mpi.h>

const int data_len = 1000;
const int num_trials = 10000;

void my_bcast(void* data, int count, MPI_Datatype datatype, int root,
              MPI_Comm communicator) {
    int world_rank;
    MPI_Comm_rank(communicator, &world_rank);
    int world_size;
    MPI_Comm_size(communicator, &world_size);

    if (world_rank == root) {
        // If we are the root process, send our data to everyone
        int i;
        for (i = 0; i < world_size; i++) {
            if (i != world_rank) {
                MPI_Send(data, count, datatype, i, 0, communicator);
            }
        }
    } else {
        // If we are a receiver process, receive the data from the root
        MPI_Recv(data, count, datatype, root, 0, communicator,
                 MPI_STATUS_IGNORE);
    }
}


int main (int argc, char *argv[]) {
    int rank1, size1, rank2, size2;
    std::vector<int> data(data_len, 0);
    MPI_Comm comm1;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank1);
    MPI_Comm_size(MPI_COMM_WORLD, &size1);

    if (rank1 == 0 ) {
        std::cout << "Check MPI_Bcast\n";
        std::srand(std::time(nullptr));
        std::generate(data.begin(), data.end(), []() { return rand() % 100; });
    }

    MPI_Bcast(data.data(), data_len, MPI_INT, 0, MPI_COMM_WORLD);

    std::cout << "Check MPI_Bcast " <<"Rank: " << rank1 << " Data:";
    std::for_each(data.begin(), data.begin() + 10, [](int n) {
       std::cout << " " << n;
    });
    std::cout << "\n";


    MPI_Barrier(MPI_COMM_WORLD);


    if (rank1 == 0 ) {
        std::generate(data.begin(), data.end(), []() { return rand() % 100; });
    }

    my_bcast(data.data(), data_len, MPI_INT, 0, MPI_COMM_WORLD);

    std::cout << "Check my_bcast " <<"Rank: " << rank1 << " Data:";
    std::for_each(data.begin(), data.begin() + 10, [](int n) {
        std::cout << " " << n;
    });
    std::cout << "\n";


    MPI_Barrier(MPI_COMM_WORLD);
    double total_mpi_bcast_time = 0;
    if (rank1 == 0 ) std::cout << "Data size = " << data_len << " Trials = " << num_trials << "\n";

    for (int i = 0; i < num_trials; i++) {
        MPI_Barrier(MPI_COMM_WORLD);
        total_mpi_bcast_time -= MPI_Wtime();
        MPI_Bcast(data.data(), data_len, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        total_mpi_bcast_time += MPI_Wtime();
    }
    if (rank1 == 0 ) std::cout << "Avg MPI_Bcast time = " << total_mpi_bcast_time << "\n";

    total_mpi_bcast_time = 0;
    for (int i = 0; i < num_trials; i++) {
        MPI_Barrier(MPI_COMM_WORLD);
        total_mpi_bcast_time -= MPI_Wtime();
        my_bcast(data.data(), data_len, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        total_mpi_bcast_time += MPI_Wtime();
    }
    if (rank1 == 0 ) std::cout << "Avg my_bcast time = " << total_mpi_bcast_time << "\n";

    MPI_Finalize();
}

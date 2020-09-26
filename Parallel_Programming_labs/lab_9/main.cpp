#include <fstream>
#include <vector>
#include <iostream>
#include <mpi.h>

const std::string FILE_NAME = "data.binary";
const int DATA_LEN = 10;

int main(int argc, char** argv) {
    int process_Rank, size_Of_Cluster, message_Item, new_item;
    int bufsize, nints, offset, array_len;
    int64_t sum = 1;
    MPI_File fh;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size_Of_Cluster);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_Rank);

    bufsize = sizeof(int) * 10; // local number to read
    nints = bufsize/sizeof (int);

    offset = DATA_LEN / size_Of_Cluster * process_Rank;
    if (process_Rank == size_Of_Cluster - 1) {
        array_len = DATA_LEN - offset;
    } else {
        array_len = DATA_LEN / size_Of_Cluster;
    }
    std::vector<int> data(array_len);

    MPI_File_open(MPI_COMM_WORLD, FILE_NAME.data(), MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
    MPI_File_seek(fh, offset * 4, MPI_SEEK_CUR);
    MPI_File_read(fh, data.data(), array_len, MPI_INT, &status);
    MPI_File_close(&fh);

    for(int i = 0 ; i < data.size() ; i++) {
        std::cout << process_Rank << " " << data[i] << "\n";
        sum *= data[i];
    }

    std::cout << process_Rank << " Sum = " << sum << "\n";

    if(process_Rank != size_Of_Cluster - 1) {
        MPI_Send(&sum, 1, MPI_INT, size_Of_Cluster - 1, 1, MPI_COMM_WORLD);

    } else {
        for (int i = 0; i < size_Of_Cluster - 1; i++) {
            MPI_Recv(&new_item, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum *= new_item;
        }
        std::cout << sum << "\n";
    }

    MPI_Finalize();
    return 0;
}

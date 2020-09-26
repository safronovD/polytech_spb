#include <fstream>
#include <vector>
#include <iostream>
#include <ctime>

const std::string FILE_NAME = "data.binary";
const int DATA_LEN = 10;

int my_rand() {
    return std::rand() % 50;
}

int main() {
    std::vector<int> data(DATA_LEN);
    std::srand(std::time(nullptr));
    std::generate(data.begin(), data.end(), my_rand);

    for (int i = 0; i < data.size(); i++) {
        std::cout << (int)data[i] << " ";
    }
    std::cout << "\n";

    std::ofstream myfile = std::ofstream(FILE_NAME, std::ios::out | std::ios::binary);
    myfile.write((char*)data.data(), sizeof(data[0]) * data.size());
    myfile.close();

    std::ifstream ifs(FILE_NAME, std::ios::in | std::ios::binary);

    ifs.seekg(0, ifs.end);
    int file_len = ifs.tellg();

    std::vector<int> char_data(file_len / sizeof(int));
    ifs.seekg(0);
    ifs >> (char*)char_data.data();

    for (int i = 0; i < char_data.size(); i++) {
        std::cout << (int) char_data[i] << " ";
    }
    std::cout << "\n";
    ifs.close();

    return 0;
}

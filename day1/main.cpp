#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>


std::vector<std::tuple<int, int>> load_input_data(const std::filesystem::path path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        throw std::runtime_error("Unable to read");
    }

    std::vector<std::tuple<int, int>> results;
    std::string line;

    while(std::getline(file, line)) {
        std::ifstream stream(line);
        int token1, token2;

        if(stream >> token1 >> token2) {
            results.push_back(std::make_tuple(token1, token2));
        }
    }
    file.close();

    return results;
}


int main(int argc, char *argv[]) {
    const auto data = load_input_data("input.txt");

    for(const auto& item : data) {
        std::cout << std::get<0>(item) << " : " << std::get<1>(item) << std::endl;
    }

    return 0;
}

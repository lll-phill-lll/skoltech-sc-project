#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <fstream>
#include <sstream>

std::vector<std::vector<double>> mat_multiplication(const std::vector<std::vector<double>> &a,
                          const std::vector<std::vector<double>> &b) {
    std::vector<std::vector<double>> c(a.size(), std::vector<double>(b[0].size(), 0));
    for(int i = 0; i < a.size(); ++i) {
        if (i % 100 == 0) {
            std::cout << i << "column\n";
        }
        for (int j = 0; j < b[0].size(); ++j) {
            for (int k = 0; k < a[0].size(); ++k)
                c[i][j] += a[i][k] * b[k][j];
        }
    }

    return c;
}

std::vector<std::vector<double>> mat_multiplication_cache_friendly(const std::vector<std::vector<double>> &a,
                          const std::vector<std::vector<double>> &b) {
    std::vector<std::vector<double>> c(a.size(), std::vector<double>(b[0].size(), 0));
    for(int i = 0; i < a.size(); ++i) {
        if (i % 100 == 0) {
            std::cout << i << "column\n";
        }
        for (int j = 0; j < b[0].size(); ++j) {
            for (int k = 0; k < a[0].size(); ++k)
                c[i][j] += a[i][k] * b[j][k];
        }
    }
    return c;
}

std::vector<std::vector<int>> construct_random_matrix(int n, int m) {
    std::vector<std::vector<int>> mat(n, std::vector<int>(m));
    for (int i = 0; i != n; ++i) {
        for (int j = 0; j != m; ++j) {
            mat[i][j] = std::rand() % 1000;
        }
    }
    return mat;
}

std::vector<std::vector<double>> transpose_matrix(const std::vector<std::vector<double>> &mat) {
    std::vector<std::vector<double>> tr_mat(mat[0].size(), std::vector<double>(mat.size()));
    for (int i = 0; i != mat.size(); ++i) {
        for (int j = 0; j != mat[0].size(); ++j) {
            tr_mat[j][i] = mat[i][j];
        }
    }
    return tr_mat;
}
void print_matrix(const std::vector<std::vector<double>> &mat) {
    std::cout << "============\n";
    for (int i = 0; i != mat.size(); ++i) {
        for (int j = 0; j != mat[0].size(); ++j) {
            std::cout << mat[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "============\n";
}

std::vector<std::vector<double>> read_matrix(const std::string &filename, int size) {
    std::string line;
    std::ifstream file(filename);
    std::vector<std::vector<double>> mat(size, std::vector<double>(size, size));
    if (file.is_open()) {
        int i = 0;
		while (std::getline(file, line)) {
            std::istringstream iss(line); 
            std::vector<std::string> result { 
                std::istream_iterator<std::string>(iss), {} 
            }; 
            for (int j = 0; j != result.size(); ++j) {
                mat[i][j] = std::stod(result[j]);
            }
            ++i;
    	}
		file.close();
  	}
    return mat;
}

int main() {
    srand (time(nullptr));
    std::ofstream myfile;
    myfile.open("result.csv");

    myfile << "it nocache cache\n";

    for (int i = 300; i != 1000; i += 50) {
        std::cout << i << "\n";
        myfile << i << " ";
        std::string afile = "mat/a" + std::to_string(i) + ".csv";
        std::string bfile = "mat/b" + std::to_string(i) + ".csv";
        std::vector<std::vector<double>> a = read_matrix(std::string(afile), i);
        std::vector<std::vector<double>> b = read_matrix(std::string(bfile), i);

        auto b_t = transpose_matrix(b);

        auto start = std::chrono::system_clock::now();
        auto mat_res1 = mat_multiplication(a, b);
        auto end = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsed_seconds = end - start;

        std::cout << "First method finished, elapsed time: " << elapsed_seconds.count() << "s\n";
        myfile << elapsed_seconds.count() << " ";

        start = std::chrono::system_clock::now();
        auto mat_res2 = mat_multiplication_cache_friendly(a, b_t);
        end = std::chrono::system_clock::now();

        elapsed_seconds = end - start;

        std::cout << "Second method finished, elapsed time: " << elapsed_seconds.count() << "s\n";
        myfile << elapsed_seconds.count() << "\n";

    }
    myfile.close();
    return 0;
}

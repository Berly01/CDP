#include <iostream>
#include <chrono>
#include <random>

// Variables para generar numeros aleatorios
std::random_device rd;
std::default_random_engine engine(rd());
std::uniform_int_distribution<> generator(1, 1000);

using Matrix = std::vector<std::vector<int>>;
using Array = std::vector<int>;

Array get_random_array(const int&);
Matrix get_random_matrix(const int&);
void blocks_test(const int&);
void matrices_classic_multiplication_test(const int&);
void matrices_blocks_multiplication_test(const int&, const int&);

int main() {

    const auto N = 2000;

    //blocks_test(1000);
    //matrices_classic_multiplication_test(N);
    //matrices_blocks_multiplication_test(N, 128);

    return 0;
}

Array get_random_array(const int& N) {
    Array a(N, 0);

    for (auto& _value : a) {
        _value = generator(engine);
    }

    return a;
}

Matrix get_random_matrix(const int& N) {
    Matrix m(N, Array(N, 0));

    for (auto& _array : m) {
        for (auto& _value : _array) {
            _value = generator(engine);
        }
    }

    return m;
}

void blocks_test(const int& MAX) {
    // Inicializa valores
    auto A{ get_random_matrix(MAX) };
    auto x{ get_random_array(MAX) };
    auto y{ Array(MAX, 0) };

    // Primer bloque
    {
        auto begin{ std::chrono::high_resolution_clock::now() };
        for (int i{ 0 }; i < MAX; i++) {
            for (int j{ 0 }; j < MAX; j++) {
                y[i] += A[i][j] * x[j];
            }
        }
        auto end{ std::chrono::high_resolution_clock::now() };
        auto result{ std::chrono::duration_cast<std::chrono::milliseconds>(end - begin) };   
        std::cout << "First Block: " << result.count() << " Miliseconds" << '\n';
    }

    y = Array(MAX, 0);

    // Segundo bloque
    {
        auto begin = std::chrono::high_resolution_clock::now();
        for (int j{ 0 }; j < MAX; j++) {
            for (int i{ 0 }; i < MAX; i++) {
                y[i] += A[i][j] * x[j];
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto result = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        std::cout << "Second Block: " << result.count() << " Miliseconds" << '\n';
    }
}

void matrices_classic_multiplication_test(const int& N) {
    // Inicializa valores
    auto A{ get_random_matrix(N) };
    auto B{ get_random_matrix(N) };
    Matrix C(N, Array(N, 0));

    // Inicio de la medicion del tiempo de ejecucion
    auto begin{ std::chrono::high_resolution_clock::now() };

    // Multiplicacion clasica de matrices NxN
    // 3 bucles anidados
    for (int i{ 0 }; i < N; i++) {
        for (int j{ 0 }; j < N; j++) {
            for (int k{ 0 }; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // Fin de la medicion del tiempo de ejecucion
    auto end{ std::chrono::high_resolution_clock::now() };

    auto result{ std::chrono::duration_cast<std::chrono::milliseconds>(end - begin) };
    std::cout << "Matrices Classic Multiplication Test: " << result.count() << " Miliseconds" << '\n';
}

void matrices_blocks_multiplication_test(const int& N, const int& BLOCK_SIZE) {

    // Inicializa valores
    auto A{ get_random_matrix(N) };
    auto B{ get_random_matrix(N) };
    Matrix C(N, Array(N, 0));

    // Inicio de la medicion del tiempo de ejecucion
    auto begin{ std::chrono::high_resolution_clock::now() };

    // Multiplicacion por bloques de matrices NxN
    // 6 bucles anidados
    for (int i_block = 0; i_block < N; i_block += BLOCK_SIZE) {
        for (int j_block = 0; j_block < N; j_block += BLOCK_SIZE) {
            for (int k_block = 0; k_block < N; k_block += BLOCK_SIZE) {
                // Multiplicacion de los bloques A y B
                for (int i = i_block; i < std::min(i_block + BLOCK_SIZE, N); ++i) {
                    for (int j = j_block; j < std::min(j_block + BLOCK_SIZE, N); ++j) {
                        for (int k = k_block; k < std::min(k_block + BLOCK_SIZE, N); ++k) {
                            C[i][j] += A[i][k] * B[k][j];
                        }
                    }
                }
            }
        }
    }

    // Fin de la medicion del tiempo de ejecucion
    auto end{ std::chrono::high_resolution_clock::now() };
    auto result{ std::chrono::duration_cast<std::chrono::milliseconds>(end - begin) };
    std::cout << "Matrices Block Multiplication Test: " << result.count() << " Miliseconds" << '\n';
}

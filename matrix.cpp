#include <iostream>
#include <iomanip>
#include <ctime>

namespace {

constexpr int kRows = 20480;
constexpr int kColumns = 4096;
constexpr int kIterations = 10;

volatile char trash;
char matrix[kRows][kColumns];

}  // namespace

namespace benchmark {

double get_elapsed_seconds(const struct timespec& start, const struct timespec& end) {
    double sec = static_cast<double>(end.tv_sec - start.tv_sec);
    double nsec = static_cast<double>(end.tv_nsec - start.tv_nsec) / 1e9;
    return sec + nsec;
}

// Write the matrix one row at a time, following row-major memory layout.
void write_row() {
    for (int row = 0; row < kRows; ++row) {
        for (int col = 0; col < kColumns; ++col) {
            matrix[row][col] = 'A';
        }
    }
}

// Write the matrix one column at a time, traversing memory with large strides.
void write_column() {
    for (int col = 0; col < kColumns; ++col) {
        for (int row = 0; row < kRows; ++row) {
            matrix[row][col] = 'B';
        }
    }
}

// Read the matrix one row at a time, following row-major memory layout.
void read_row() {
    char temp = 0;
    for (int row = 0; row < kRows; ++row) {
        for (int col = 0; col < kColumns; ++col) {
            temp = matrix[row][col];
        }
    }
    trash = temp;
}

// Read the matrix one column at a time, traversing memory with large strides.
void read_column() {
    char temp = 0;
    for (int col = 0; col < kColumns; ++col) {
        for (int row = 0; row < kRows; ++row) {
            temp = matrix[row][col];
        }
    }
    trash = temp;
}

// Run an operation repeatedly and report its average elapsed time.
double measure_time(void (*operation)()) {
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < kIterations; ++i) {
        operation();
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    return get_elapsed_seconds(start, end) / kIterations;
}

}  // namespace benchmark

int main() {
    const double time_row_write = benchmark::measure_time(benchmark::write_row);
    const double time_column_write = benchmark::measure_time(benchmark::write_column);
    const double time_row_read = benchmark::measure_time(benchmark::read_row);
    const double time_column_read = benchmark::measure_time(benchmark::read_column);

    std::cout << "Time to write row: " << time_row_write << " seconds\n";
    std::cout << "Time to write column: " << time_column_write << " seconds\n";
    std::cout << "Time to read row: " << time_row_read << " seconds\n";
    std::cout << "Time to read column: " << time_column_read << " seconds\n";

    return 0;
}
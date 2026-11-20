#include <iostream>
#include <iomanip>
#include <ctime>
using namespace std;

const int ROWS = 20480;
const int COLS = 4096;
const int ITERATIONS = 10;
volatile char trash;
char matrix[ROWS][COLS];

double get_elapsed_seconds(const struct timespec& start, const struct timespec& end) {
    double sec = static_cast<double>(end.tv_sec - start.tv_sec);
    double nsec = static_cast<double>(end.tv_nsec - start.tv_nsec) / 1e9;
    return sec + nsec;
}

void writeRow()
{
    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col < COLS; ++col)
        {
            matrix[row][col] = 'A';
        }
    }
}

void writeCol()
{
    for (int col = 0; col < COLS; ++col)
    {
        for (int row = 0; row < ROWS; ++row)
        {
            matrix[row][col] = 'B';
        }
    }
}

void readRow()
{
    char temp = 0;
    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col < COLS; ++col)
        {
            temp = matrix[row][col];
        }
    }
    trash = temp;
}

void readCol()
{
    char temp = 0;
    for (int col = 0; col < COLS; ++col)
    {
        for (int row = 0; row < ROWS; ++row)
        {
            temp = matrix[row][col];
        }
    }
    trash = temp;
}

double measureTime(void (*func)()) {
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < ITERATIONS; ++i) {
        func();
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    return get_elapsed_seconds(start, end) / ITERATIONS;

}

int main ()
{
    double time_row_write = measureTime(writeRow);
    double time_col_write = measureTime(writeCol);
    double time_row_read = measureTime(readRow);
    double time_col_read = measureTime(readCol);

    cout << "Time to write row: " << time_row_write << " seconds" << endl;
    cout << "Time to write column: " << time_col_write << " seconds" << endl;
    cout << "Time to read row: " << time_row_read << " seconds" << endl;
    cout << "Time to read column: " << time_col_read << " seconds" << endl;

    return 0;
}
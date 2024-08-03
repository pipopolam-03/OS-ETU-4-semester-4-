#include <windows.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <mutex>
#include <ctime>
#include <omp.h>

using namespace std;

const int n = 100000000;
const int block_size = 2307150;
const int num_blocks = n / block_size;
long double pi = 0.0;
clock_t start_time, end_time;

mutex mtx;
int current_block = 0;

DWORD WINAPI calculation(LPVOID lpParam) {
    while (true) {
        int start, end;

        mtx.lock();
        start = current_block * block_size;
        end = (current_block == num_blocks) ? n : (current_block + 1) * block_size;
        current_block++;
        mtx.unlock();

        if (start >= n) {
            break;
        }

        long double local_pi = 0.0;
        for (int k = start; k < end; k++) {
            local_pi += 4.0 / (1.0 + ((k + 0.5) / n) * ((k + 0.5) / n));
        }

        mtx.lock();
        pi += local_pi / n;
        mtx.unlock();
    }

    return 0;
}

void standart(int num_threads) {
    vector<HANDLE> threads(num_threads);

    current_block = 0;
    pi = 0.0;

    for (int i = 0; i < num_threads; i++) {
        threads[i] = CreateThread(NULL, 0, calculation, NULL, CREATE_SUSPENDED, NULL);    //аттр. безоп. по умолчанию, размер стека по умолчанию, функция, данные в поток, приост., id потока
    }

    for (int i = 0; i < num_threads; i++) {
        ResumeThread(threads[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        WaitForSingleObject(threads[i], INFINITE);
        CloseHandle(threads[i]);
    }
}

void openmp(int num_threads) {
    double temp = 0.0;
    double local_pi = 0.0;

#pragma omp parallel num_threads(num_threads) private(temp, local_pi)
    {
#pragma omp for 
        for (long long int i = 0; i < n; i++) {
            temp = (i + 0.5) * (1.0 / n);
            local_pi += 4.0 / (1.0 + temp * temp);
        }

#pragma omp critical
        {
            pi += local_pi;
        }
    }
}

int main() {
    int flag = -1, size = 5;
    double elapsed_time;
    int num_threads[6] = { 1, 2, 4, 8, 12, 16 };
    cout << "Choose type of calculations:\n1 - standart\n2 - openmp\n0 - exit program\n";
    cin >> flag;
    cout << "Choose number of figures after point: ";
    cin >> size;
    if (flag == 1) {
        for (int i = 0; i < 6; i++) {
            start_time = clock();
            standart(num_threads[i]);
            end_time = clock();
            elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
            cout << num_threads[i] << " threads. ";
            cout << "Time spent: " << elapsed_time;
            cout << " pi: " << fixed << setprecision(size) << pi << endl;
        }
    }
    else if (flag == 2) {
        for (int i = 0; i < 6; i++) {
            pi = 0.0;
            start_time = clock();
            openmp(num_threads[i]);
            end_time = clock();
            elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
            cout << num_threads[i] << " threads. ";
            cout << "Time spent: " << elapsed_time;
            cout << " pi: " << fixed << setprecision(size) << pi/n << endl;
        }
    }
    else {
        exit(0);
    }

    return 0;
}

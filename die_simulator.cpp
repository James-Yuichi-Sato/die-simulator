// Generated with assistance of ChatGPT3.5
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

// Define a structure to pass data to threads
typedef struct {
    int max_rolls;
    int iterations;
} ThreadData;

// Function to simulate dice rolls
int roll_dice() {
    int roll_count = 0;
    int roll_result;

    // Seed the random number generator
    srand(time(NULL) ^ GetCurrentThreadId());

    do {
        // Roll the dice (generate a random number between 1 and 4)
        roll_result = rand() % 4 + 1;
        roll_count++;
    } while (roll_result == 1);

    return roll_count;
}

// Thread function to run the dice rolls
DWORD WINAPI run_rolls(LPVOID arg) {
    ThreadData* data = (ThreadData*)arg;
    int max_value = 0;

    for (int i = 0; i < data->iterations; i++) {
        int result = roll_dice();
        if (result > max_value) {
            max_value = result;
        }
    }

    // Store the max_value found in this thread
    data->max_rolls = max_value;
    return 0;
}

int main() {
    int x = 10000; // Number of times to run roll_dice
    HANDLE threads[100];
    ThreadData thread_data[100];
    int global_max = 0;

    // Create threads
    for (int i = 0; i < 100; i++) {
        thread_data[i].iterations = x / 100;
        thread_data[i].max_rolls = 0;
        threads[i] = CreateThread(NULL, 0, run_rolls, &thread_data[i], 0, NULL);
        if (threads[i] == NULL) {
            fprintf(stderr, "Error creating thread %d\n", i);
            return 1;
        }
    }

    // Wait for all threads to finish
    WaitForMultipleObjects(100, threads, TRUE, INFINITE);

    // Find the maximum rolls among all threads
    for (int i = 0; i < 100; i++) {
        CloseHandle(threads[i]);
        if (thread_data[i].max_rolls > global_max) {
            global_max = thread_data[i].max_rolls;
        }
    }

    // Print the maximum value found
    printf("The maximum number of rolls returned by roll_dice: %d\n", global_max);

    return 0;
}
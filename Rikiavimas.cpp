#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

struct Stats {
    long long comparisons = 0;
    long long moves = 0;
};

void insertionSort(vector<int>& arr, Stats& stats) {
    for (int i = 1; i < (int)arr.size(); i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0) {
            stats.comparisons++;

            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                stats.moves++;
                j--;
            }
            else {
                break;
            }
        }

        arr[j + 1] = key;
        stats.moves++;
    }
}

void merge(vector<int>& arr, int left, int mid, int right, Stats& stats) {
    vector<int> temp;

    int i = left;
    int j = mid + 1;

    while (i <= mid && j <= right) {
        stats.comparisons++;

        if (arr[i] <= arr[j]) {
            temp.push_back(arr[i]);
            stats.moves++;
            i++;
        }
        else {
            temp.push_back(arr[j]);
            stats.moves++;
            j++;
        }
    }

    while (i <= mid) {
        temp.push_back(arr[i]);
        stats.moves++;
        i++;
    }

    while (j <= right) {
        temp.push_back(arr[j]);
        stats.moves++;
        j++;
    }

    for (int k = 0; k < (int)temp.size(); k++) {
        arr[left + k] = temp[k];
        stats.moves++;
    }
}

void mergeSort(vector<int>& arr, int left, int right, Stats& stats) {
    if (left >= right) {
        return;
    }

    int mid = left + (right - left) / 2;

    mergeSort(arr, left, mid, stats);
    mergeSort(arr, mid + 1, right, stats);
    merge(arr, left, mid, right, stats);
}

vector<int> generateRandomData(int size) {
    vector<int> data(size);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 1000000);

    for (int i = 0; i < size; i++) {
        data[i] = dist(gen);
    }

    return data;
}

vector<int> generateSortedData(int size) {
    vector<int> data(size);

    for (int i = 0; i < size; i++) {
        data[i] = i + 1;
    }

    return data;
}

vector<int> generateReverseData(int size) {
    vector<int> data(size);

    for (int i = 0; i < size; i++) {
        data[i] = size - i;
    }

    return data;
}

long long testInsertionSort(vector<int> data, Stats& stats) {
    auto start = chrono::high_resolution_clock::now();

    insertionSort(data, stats);

    auto end = chrono::high_resolution_clock::now();

    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}

long long testMergeSort(vector<int> data, Stats& stats) {
    auto start = chrono::high_resolution_clock::now();

    mergeSort(data, 0, data.size() - 1, stats);

    auto end = chrono::high_resolution_clock::now();

    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}

void runTest(string dataType, vector<int> originalData) {
    const int repeatCount = 5;

    long long insertionTimeSum = 0;
    long long mergeTimeSum = 0;

    long long insertionComparisonsSum = 0;
    long long mergeComparisonsSum = 0;

    long long insertionMovesSum = 0;
    long long mergeMovesSum = 0;

    for (int i = 0; i < repeatCount; i++) {
        Stats insertionStats;
        Stats mergeStats;

        vector<int> dataForInsertion = originalData;
        vector<int> dataForMerge = originalData;

        insertionTimeSum += testInsertionSort(dataForInsertion, insertionStats);
        mergeTimeSum += testMergeSort(dataForMerge, mergeStats);

        insertionComparisonsSum += insertionStats.comparisons;
        mergeComparisonsSum += mergeStats.comparisons;

        insertionMovesSum += insertionStats.moves;
        mergeMovesSum += mergeStats.moves;
    }

    cout << "Duomenu tipas: " << dataType << endl;
    cout << "Elementu kiekis: " << originalData.size() << endl;

    cout << "Insertion sort vidutinis laikas: "
        << insertionTimeSum / repeatCount << " mikrosek." << endl;

    cout << "Merge sort vidutinis laikas: "
        << mergeTimeSum / repeatCount << " mikrosek." << endl;

    cout << "Insertion sort palyginimai: "
        << insertionComparisonsSum / repeatCount << endl;

    cout << "Merge sort palyginimai: "
        << mergeComparisonsSum / repeatCount << endl;

    cout << "Insertion sort perkelimai: "
        << insertionMovesSum / repeatCount << endl;

    cout << "Merge sort perkelimai: "
        << mergeMovesSum / repeatCount << endl;

    cout << "----------------------------------------" << endl;
}

int main() {
    vector<int> sizes = { 5000, 10000, 50000 };

    for (int size : sizes) {
        vector<int> randomData = generateRandomData(size);
        vector<int> reverseData = generateReverseData(size);
        vector<int> sortedData = generateSortedData(size);

        runTest("Atsitiktiniai duomenys", randomData);
        runTest("Atvirksciai surikiuoti duomenys", reverseData);
        runTest("Jau surikiuoti duomenys", sortedData);
    }

    return 0;
}
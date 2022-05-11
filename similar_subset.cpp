#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <functional>
#include <algorithm>
#include <ctime>
#include <cstring>

#define MAX_NUMBERS             100
#define MAX_PARTITIONS          100

using namespace std;

// input data
int cntNumbers, cntPartitions;
int a[MAX_NUMBERS];

// rem[i] = sum(a[i..n])
int rem[MAX_NUMBERS];

// temp solution
int sum[MAX_PARTITIONS], id[MAX_NUMBERS];

// best solution 
int bestDiff, bestId[MAX_NUMBERS];

// initialize solution by greedy algorithm
void initGreedySolution() {
        // arrange numbers in decreasing order
        sort(a, a + cntNumbers);
        reverse(a, a + cntNumbers);

        // calculate remaining sum
        rem[cntNumbers] = 0;
        for (int i = cntNumbers - 1; i >= 0; i--)
                rem[i] = rem[i + 1] + a[i];

        std::memset(sum, 0, sizeof(sum));
        for (int i = 0; i < cntNumbers; i++) {
                // add i-th number to smallest partition
                int k = 0;
                for (int j = 0; j < cntPartitions; j++)
                        if (sum[j] < sum[k]) k = j;
                id[i] = k;
                sum[k] += a[i];
        }

        // save greedy solution to best solution
        memcpy(bestId, id, sizeof(bestId));
        int maxSum = sum[0], minSum = sum[0];
        for (int i = 1; i < cntPartitions; i++) {
                if (sum[i] > maxSum) maxSum = sum[i];
                if (sum[i] < minSum) minSum = sum[i];
        }

        bestDiff = maxSum - minSum;
}

// Check function of A* search algorithm
bool check(int k) {
        if (bestDiff == 0) return false;

        int maxSum = sum[0], minSum = sum[0];
        for (int i = 1; i < cntPartitions; i++) {
                if (sum[i] > maxSum) maxSum = sum[i];
                if (sum[i] < minSum) minSum = sum[i];
        }

        if (k == cntNumbers) {
                return maxSum - minSum < bestDiff;
        }

        int req = 0;
        for (int i = 0; i < cntPartitions; i++) {
                req += max(maxSum - sum[i] - bestDiff, 0);
        }

        return req < rem[k];
}

// backtracking
void findSolution(int k) {
        if (!check(k)) return;

        // base case: this means we found better solution thanks to A* algorithm
        if (k == cntNumbers) {
                int maxSum = sum[0], minSum = sum[0];
                for (int i = 1; i < cntPartitions; i++) {
                        if (sum[i] > maxSum) maxSum = sum[i];
                        if (sum[i] < minSum) minSum = sum[i];
                }
                bestDiff = maxSum - minSum;
                memcpy(bestId, id, sizeof(bestId));
                // cout << "Solution updated: current diff = " << bestDiff << endl;
                return;
        }

        for (int i = 0; i < cntPartitions; i++) {
                // set k-th element to i-th partition
                sum[i] += a[k];
                id[k] = i;
                findSolution(k + 1);
                // restore backtrack state
                sum[i] -= a[k];
        }
}

int main() {
        cout << "Input N: "; cin >> cntNumbers;
        cout << "Input K: "; cin >> cntPartitions;
        cout << "Input " << cntNumbers << " numbers:\n";

        for (int i = 0; i < cntNumbers; i++) cin >> a[i];

        int start = clock();

        bestDiff = 0;
        initGreedySolution();

        memset(sum, 0, sizeof(sum));
        findSolution(0);

        // print solutions
        cout << "Best difference: " << bestDiff << endl;
        for (int i = 0; i < cntPartitions; i++) {
                cout << "Partition " << i + 1 << ":";
                int s = 0;
                for (int j = 0; j < cntNumbers; j++) {
                        if (bestId[j] == i) {
                                cout << " " << a[j];
                                s += a[j];
                        }
                }
                cout << ": sum = " << s << endl;
        }

        cout << "Elapsed time: " << clock() - start << "ms" << endl;

        return 0;
}
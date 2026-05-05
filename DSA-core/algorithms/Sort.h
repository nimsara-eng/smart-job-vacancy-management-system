#pragma once
#include <vector>
#include "../models/Job.h"

class Sort {
public:

    // Sort by salary - ascending
    static void bubbleSort(std::vector<Job>& jobs) {
        int n = jobs.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (jobs[j].salary > jobs[j + 1].salary) {
                    Job temp = jobs[j];
                    jobs[j] = jobs[j + 1];
                    jobs[j + 1] = temp;
                }
            }
        }
    }

    // Sort by salary - using quicksort
    static void quickSort(std::vector<Job>& jobs, int low, int high) {
        if (low < high) {
            int pivotIndex = partition(jobs, low, high);
            quickSort(jobs, low, pivotIndex - 1);
            quickSort(jobs, pivotIndex + 1, high);
        }
    }

    // Sort alphabetically by title
    static void sortByTitle(std::vector<Job>& jobs) {
        int n = jobs.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (jobs[j].title > jobs[j + 1].title) {
                    Job temp = jobs[j];
                    jobs[j] = jobs[j + 1];
                    jobs[j + 1] = temp;
                }
            }
        }
    }

    // Sort by category
    static void sortByCategory(std::vector<Job>& jobs) {
        int n = jobs.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (jobs[j].category > jobs[j + 1].category) {
                    Job temp = jobs[j];
                    jobs[j] = jobs[j + 1];
                    jobs[j + 1] = temp;
                }
            }
        }
    }

private:

    static int partition(std::vector<Job>& jobs, int low, int high) {
        float pivot = jobs[high].salary;
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (jobs[j].salary <= pivot) {
                i++;
                Job temp = jobs[i];
                jobs[i] = jobs[j];
                jobs[j] = temp;
            }
        }

        Job temp = jobs[i + 1];
        jobs[i + 1] = jobs[high];
        jobs[high] = temp;

        return i + 1;
    }
};
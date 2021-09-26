#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int arrival_time;
    int burst_time;
    int is_used; // 0 is no, 1 otherwise
} Process;

typedef struct {
    int arrival_time;
    int burst_time;
    int completion_time;
    int turn_around_time;
    int waiting_time;
} Result;

void swap(Process* first, Process* second){
    Process temp;

    temp = *first;
    *first = *second;
    *second = temp;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int main() {
    int processes_number;
    printf("Enter the number of processes: ");
    scanf("%d", &processes_number);

    Process processes_list[processes_number];
    Result results_list[processes_number];

    int num;
    printf("Enter Arrival time and Burst time for each processes:\n");
    for (int i = 0; i < processes_number; ++i) {
        printf("Enter Arrival time for process [%d]: ", i + 1);
        scanf("%d", &num);
        processes_list[i].arrival_time = num;
        printf("Enter Burst time for process [%d]: ", i + 1);
        scanf("%d", &num);
        processes_list[i].burst_time = num;

        processes_list[i].is_used = 0;

        results_list[i].completion_time = 0;
        results_list[i].turn_around_time = 0;
        results_list[i].waiting_time = 0;
    }

    // Bubble sort:
    for (int i = 0; i < processes_number; ++i) {
        for (int j = 0; j < processes_number - i - 1; ++j) {
            if (processes_list[j].arrival_time > processes_list[j + 1].arrival_time) {
                swap(&(processes_list[j]), &(processes_list[j + 1]));
            } else if (processes_list[j].arrival_time == processes_list[j + 1].arrival_time) {
                if (processes_list[i].burst_time > processes_list[j + 1].burst_time) {
                    swap(&(processes_list[j]), &(processes_list[j + 1]));
                }
            }
        }
    }

    results_list[0].waiting_time = 0;
    results_list[0].completion_time = processes_list[0].burst_time + processes_list[0].arrival_time;
    results_list[0].turn_around_time = processes_list[0].burst_time + results_list[0].waiting_time;
    results_list[0].arrival_time = processes_list[0].arrival_time;
    results_list[0].burst_time = processes_list[0].burst_time;
    processes_list[0].is_used = 1;

    int prev_pid = 0;
    int curr_pid = -1;
    int is_found = 0; // 0 - no, 1 - yes
    int min_burst_time = 9999;
    int min_arrival_time = 9999;
    int min_burst_time_not_found = 9999;
    for (int cnter = 0; cnter < processes_number - 1; ++cnter) {
        for (int i = 1; i < processes_number; ++i) {
//            printf("Arrival time = %d <= Completion time = %d, pid = %d\n", processes_list[i].arrival_time, results_list[prev_pid].completion_time, i);
//            printf("Burst time = %d <= Min burst time = %d, pid = %d\n", processes_list[i].burst_time, min_burst_time, i);
//            printf("Is_used = %d, pid = %d\n\n", processes_list[i].is_used, i);
            if (processes_list[i].is_used == 0 && processes_list[i].arrival_time <= results_list[prev_pid].completion_time && processes_list[i].burst_time < min_burst_time) {
                min_burst_time = processes_list[i].burst_time;
                curr_pid = i;
                is_found = 1;
            } else if (processes_list[i].is_used == 0 && processes_list[i].arrival_time <= min_arrival_time && processes_list[i].burst_time <= min_burst_time_not_found && is_found == 0) {
                min_arrival_time = processes_list[i].arrival_time;
                min_burst_time_not_found = processes_list[i].burst_time;

                curr_pid = i;
                is_found = 0;
            }
        }

        if (is_found == 1) {
            int completion_time = max(results_list[prev_pid].completion_time, processes_list[curr_pid].arrival_time) +
                                  processes_list[curr_pid].burst_time;
            int turn_around_time = completion_time - processes_list[curr_pid].arrival_time;
            int waiting_time = max(turn_around_time - processes_list[curr_pid].burst_time, 0);

            processes_list[curr_pid].is_used = 1;
            prev_pid++;

            results_list[prev_pid].waiting_time = waiting_time;
            results_list[prev_pid].turn_around_time = turn_around_time;
            results_list[prev_pid].completion_time = completion_time;
            results_list[prev_pid].arrival_time = processes_list[curr_pid].arrival_time;
            results_list[prev_pid].burst_time = processes_list[curr_pid].burst_time;

            min_burst_time = 9999;
            is_found = 0;
        } else {
            int completion_time = processes_list[curr_pid].arrival_time + processes_list[curr_pid].burst_time;
            int turn_around_time = completion_time - processes_list[curr_pid].arrival_time;
            int waiting_time = 0;

            processes_list[curr_pid].is_used = 1;
            prev_pid++;

            results_list[prev_pid].waiting_time = waiting_time;
            results_list[prev_pid].turn_around_time = turn_around_time;
            results_list[prev_pid].completion_time = completion_time;
            results_list[prev_pid].arrival_time = processes_list[curr_pid].arrival_time;
            results_list[prev_pid].burst_time = processes_list[curr_pid].burst_time;

            min_burst_time_not_found = 9999;
            min_arrival_time = 9999;
            is_found = 0;
        }
    }

    printf("P#  AT  BT  CT  TAT  WT\n\n");
    double average_turn_around_time = 0.0;
    double average_waiting_time = 0.0;
    for (int i = 0; i < processes_number; ++i) {
        printf("P%d  %d   %d   %d   %d    %d\n", i,
               results_list[i].arrival_time,
               results_list[i].burst_time,
               results_list[i].completion_time,
               results_list[i].turn_around_time,
               results_list[i].waiting_time);
        average_turn_around_time += results_list[i].turn_around_time;
        average_waiting_time += results_list[i].waiting_time;
    }

    average_turn_around_time = average_turn_around_time / processes_number;
    average_waiting_time = average_waiting_time / processes_number;

    printf("\nAverage Turn Around time = %f\nAverage Waiting time = %f", average_turn_around_time, average_waiting_time);
    return 0;
}
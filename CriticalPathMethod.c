#include <stdio.h>
#include <string.h>
#include <limits.h>

int activities;

// Structure to represent an activity in the project
struct Activity {
    int duration;
    int est, eft; // Earliest start time, Earliest finish time
    int lst, lft; // Latest start time, Latest finish time
    int slack;    // Slack time
};

// Function to find the maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Function to find the minimum of two integers
int min(int a, int b) {
    return (a < b) ? a : b;
}

// Forward pass to calculate EST and EFT
void forwardPass(int activities, int graph[activities][activities], struct Activity activity[activities]) {
    activity[0].est = 0; // Start node's EST is 0
    activity[0].eft = activity[0].duration; // EFT for start node

    for (int i = 1; i < activities; i++) {
        activity[i].est = 0;
        for (int j = 0; j < activities; j++) {
            if (graph[j][i] == 1) { // There is a dependency
                activity[i].est = max(activity[i].est, activity[j].eft);
            }
        }
        activity[i].eft = activity[i].est + activity[i].duration;
    }
}

// Backward pass to calculate LST and LFT
void backwardPass(int activities, int graph[activities][activities], struct Activity activity[activities]) {
    activity[activities - 1].lft = activity[activities - 1].eft; // End node's LFT equals to its EFT
    activity[activities - 1].lst = activity[activities - 1].lft - activity[activities - 1].duration;

    for (int i = activities - 2; i >= 0; i--) {
        activity[i].lft = INT_MAX;
        for (int j = 0; j < activities; j++) {
            if (graph[i][j] == 1) { // There is a dependency
                activity[i].lft = min(activity[i].lft, activity[j].lst);
            }
        }
        activity[i].lst = activity[i].lft - activity[i].duration;
    }
}

// Function to calculate slack for each activity
void calculateSlack(int activities, struct Activity activity[activities]) {
    for (int i = 0; i < activities; i++) {
        activity[i].slack = activity[i].lst - activity[i].est;
    }
}

// Function to print the critical path
void printCriticalPath(int activities, struct Activity activity[activities]) {
    printf("\nCritical Path: ");
    for (int i = 0; i < activities; i++) {
        if (activity[i].slack == 0)
            printf("%c ", 'A' + i);
    }
    printf("\n");
}

// Main function
int main() {
    printf("Enter the number of activities: ");
    scanf("%d", &activities);

    int graph[activities][activities];
    struct Activity activity[activities];

    // Adjacency matrix for project network
    printf("Enter the adjacency matrix:\n");
    for (int i = 0; i < activities; i++) {
        for (int j = 0; j < activities; j++) {
            scanf("%d", &graph[i][j]);
        }
    }

    // Duration for each activity
    printf("Enter duration for each activity:\n");
    for (int i = 0; i < activities; i++) {
        printf("Activity %c: ", 'A' + i);
        scanf("%d", &activity[i].duration);
    }

    // Perform forward and backward passes
    forwardPass(activities, graph, activity);
    backwardPass(activities, graph, activity);
    calculateSlack(activities, activity);

    // Options for the manager
    printf("\nOptions:\n");
    printf("1. Print Critical Path\n");
    printf("2. Details for a particular activity\n");
    printf("3. Shortest completion time\n");

    int choice, task;
    printf("Enter option number: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printCriticalPath(activities, activity);
            break;

        case 2:
            printf("Enter task number (1-%d): ", activities);
            scanf("%d", &task);
            if (task < 1 || task > activities) {
                printf("Invalid task number!\n");
                break;
            }
            task--; // Convert to zero-based index
            printf("Details of Activity %c:\n", 'A' + task);
            printf("EST: %d\t EFT: %d\t LST: %d\t LFT: %d\n", activity[task].est, activity[task].eft, activity[task].lst, activity[task].lft);
            printf("Slack: %d\n", activity[task].slack);
            break;

        case 3:
            printf("Shortest completion time: %d\n", activity[activities - 1].eft);
            break;

        default:
            printf("Wrong Choice\n");
    }

    return 0;
}

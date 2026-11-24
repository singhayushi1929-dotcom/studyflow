/*
 * StudyFlow - Smart Student Study Planner
 * Core mini-project: pure C (console application)
 * Build: gcc studyflow.c -o studyflow
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SUBJECTS 20
#define MAX_TASKS 100
#define SUBJECT_NAME 50
#define TASK_TITLE 100
#define DATA_FILE "studyflow_data.txt"

typedef struct {
    char name[SUBJECT_NAME];
    int minutes;
} Subject;

typedef struct {
    int subjectIndex;
    char title[TASK_TITLE];
    int completed;
} Task;

Subject subjects[MAX_SUBJECTS];
Task tasks[MAX_TASKS];
int subjectCount = 0, taskCount = 0;

void readLine(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin) == NULL) buffer[0] = '\0';
    buffer[strcspn(buffer, "\n")] = '\0';
}

int readInt(const char *prompt) {
    char line[40], *end;
    long value;
    while (1) {
        readLine(prompt, line, sizeof(line));
        value = strtol(line, &end, 10);
        if (line[0] != '\0' && *end == '\0') return (int)value;
        puts("Please enter a valid whole number.");
    }
}

void pauseScreen(void) {
    puts("\nPress Enter to return to the menu...");
    getchar();
}

void addSubject(void) {
    if (subjectCount >= MAX_SUBJECTS) { puts("Subject limit reached."); return; }
    char name[SUBJECT_NAME];
    readLine("Subject name: ", name, sizeof(name));
    if (!name[0]) { puts("Name cannot be empty."); return; }
    for (int i = 0; i < subjectCount; i++) {
        if (strcmp(subjects[i].name, name) == 0) { puts("That subject already exists."); return; }
    }
    strcpy(subjects[subjectCount].name, name);
    subjects[subjectCount].minutes = 0;
    subjectCount++;
    puts("Subject added!");
}

void viewSubjects(void) {
    if (!subjectCount) { puts("No subjects yet. Add a subject first."); return; }
    puts("\n--- YOUR SUBJECTS ---");
    for (int i = 0; i < subjectCount; i++)
        printf("%2d. %-30s | Study time: %d min\n", i + 1, subjects[i].name, subjects[i].minutes);
}

void addTask(void) {
    if (taskCount >= MAX_TASKS) { puts("Task limit reached."); return; }
    if (!subjectCount) { puts("Add a subject before creating a task."); return; }
    viewSubjects();
    int choice = readInt("Choose subject number: ");
    if (choice < 1 || choice > subjectCount) { puts("Invalid subject number."); return; }
    char title[TASK_TITLE];
    readLine("Task description: ", title, sizeof(title));
    if (!title[0]) { puts("Task cannot be empty."); return; }
    tasks[taskCount].subjectIndex = choice - 1;
    strcpy(tasks[taskCount].title, title);
    tasks[taskCount].completed = 0;
    taskCount++;
    puts("Task added!");
}

void viewTasks(void) {
    if (!taskCount) { puts("No tasks added yet."); return; }
    puts("\n--- STUDY TASKS ---");
    for (int i = 0; i < taskCount; i++) {
        const char *subject = (tasks[i].subjectIndex >= 0 && tasks[i].subjectIndex < subjectCount)
            ? subjects[tasks[i].subjectIndex].name : "Unknown";
        printf("%2d. [%s] %-35s (%s)\n", i + 1, tasks[i].completed ? "DONE" : "TODO",
               tasks[i].title, subject);
    }
}

void markTaskComplete(void) {
    if (!taskCount) { puts("No tasks available."); return; }
    viewTasks();
    int choice = readInt("Task number to mark complete: ");
    if (choice < 1 || choice > taskCount) { puts("Invalid task number."); return; }
    tasks[choice - 1].completed = 1;
    puts("Great! Task marked complete.");
}

void recordStudyTime(void) {
    if (!subjectCount) { puts("Add a subject first."); return; }
    viewSubjects();
    int choice = readInt("Choose subject number: ");
    if (choice < 1 || choice > subjectCount) { puts("Invalid subject number."); return; }
    int minutes = readInt("Minutes studied (enter 1 or more): ");
    if (minutes <= 0) { puts("Please enter a positive number."); return; }
    subjects[choice - 1].minutes += minutes;
    printf("Added %d minutes to %s.\n", minutes, subjects[choice - 1].name);
}

void progressReport(void) {
    int completed = 0, totalMinutes = 0;
    for (int i = 0; i < taskCount; i++) if (tasks[i].completed) completed++;
    for (int i = 0; i < subjectCount; i++) totalMinutes += subjects[i].minutes;
    printf("\n========== STUDYFLOW REPORT ==========\n");
    printf("Subjects added       : %d\n", subjectCount);
    printf("Tasks created       : %d\n", taskCount);
    printf("Tasks completed     : %d\n", completed);
    printf("Tasks remaining     : %d\n", taskCount - completed);
    printf("Total study time    : %d minutes (%d hr %d min)\n",
           totalMinutes, totalMinutes / 60, totalMinutes % 60);
    if (taskCount)
        printf("Task completion    : %.1f%%\n", (completed * 100.0) / taskCount);
    else
        puts("Task completion    : Add tasks to calculate progress.");
    puts("-------------------------------------");
}

void saveData(void) {
    FILE *file = fopen(DATA_FILE, "w");
    if (!file) { puts("Could not open data file for saving."); return; }
    for (int i = 0; i < subjectCount; i++)
        fprintf(file, "S|%s|%d\n", subjects[i].name, subjects[i].minutes);
    for (int i = 0; i < taskCount; i++)
        fprintf(file, "T|%d|%d|%s\n", tasks[i].subjectIndex, tasks[i].completed, tasks[i].title);
    fclose(file);
    printf("Saved successfully to %s\n", DATA_FILE);
}

void loadData(void) {
    FILE *file = fopen(DATA_FILE, "r");
    if (!file) { puts("No saved file found. Starting with an empty planner."); return; }
    subjectCount = 0; taskCount = 0;
    char line[220];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'S' && subjectCount < MAX_SUBJECTS) {
            char name[SUBJECT_NAME]; int mins;
            if (sscanf(line, "S|%49[^|]|%d", name, &mins) == 2) {
                strcpy(subjects[subjectCount].name, name);
                subjects[subjectCount].minutes = mins;
                subjectCount++;
            }
        } else if (line[0] == 'T' && taskCount < MAX_TASKS) {
            int subject, done; char title[TASK_TITLE];
            if (sscanf(line, "T|%d|%d|%99[^\n]", &subject, &done, title) == 3) {
                if (subject >= 0 && subject < subjectCount) {
                    tasks[taskCount].subjectIndex = subject;
                    tasks[taskCount].completed = done ? 1 : 0;
                    strcpy(tasks[taskCount].title, title);
                    taskCount++;
                }
            }
        }
    }
    fclose(file);
    printf("Loaded %d subjects and %d tasks.\n", subjectCount, taskCount);
}

void showMenu(void) {
    puts("\n========================================");
    puts("       STUDYFLOW - STUDY PLANNER");
    puts("========================================");
    puts(" 1. Add Subject");
    puts(" 2. View Subjects");
    puts(" 3. Add Study Task");
    puts(" 4. View Study Tasks");
    puts(" 5. Mark Task as Completed");
    puts(" 6. Record Study Time");
    puts(" 7. View Progress Report");
    puts(" 8. Save Data");
    puts(" 9. Exit");
    puts("----------------------------------------");
}

int main(void) {
    loadData();
    int choice;
    do {
        showMenu();
        choice = readInt("Enter your choice: ");
        switch (choice) {
            case 1: addSubject(); break;
            case 2: viewSubjects(); break;
            case 3: addTask(); break;
            case 4: viewTasks(); break;
            case 5: markTaskComplete(); break;
            case 6: recordStudyTime(); break;
            case 7: progressReport(); break;
            case 8: saveData(); break;
            case 9: saveData(); puts("Thanks for using StudyFlow. Happy studying!"); break;
            default: puts("Please select an option from 1 to 9.");
        }
        if (choice != 9) pauseScreen();
    } while (choice != 9);
    return 0;
}

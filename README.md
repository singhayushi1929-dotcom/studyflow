# StudyFlow — Smart Student Study Planner

**A beginner-friendly C mini-project** that helps students organize subjects, manage study tasks, record study time, and review progress.

## Language / tools
- C (console program)
- GCC compiler
- Standard C library
- Text-file storage for persistence

## Features
- Add and list subjects
- Add and list tasks under subjects
- Mark tasks complete
- Record study minutes per subject
- View progress summary and completion percentage
- Save and automatically load data from `studyflow_data.txt`

## C concepts demonstrated
Variables, arrays, structures, functions, loops, conditional statements, input handling, and file I/O.

## Compile and run
### Windows (MinGW GCC)
```bash
gcc studyflow.c -o studyflow.exe
studyflow.exe
```

### Linux / macOS
```bash
gcc studyflow.c -o studyflow
./studyflow
```

## Menu
1. Add Subject
2. View Subjects
3. Add Study Task
4. View Study Tasks
5. Mark Task as Completed
6. Record Study Time
7. View Progress Report
8. Save Data
9. Exit

Data is stored in a text file in the same folder as the program. This is a console-based C project; no HTML, CSS, or JavaScript is required.

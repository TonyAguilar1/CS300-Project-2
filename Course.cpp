//============================================================================
// Name        : Course.cpp
// Author      : Tony Aguilar
// Version     : 1.0
// Copyright   : Copyright © 2024 SNHU COCE
// Description : PROJECT 2
//============================================================================




#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <vector>


using namespace std;

// Define a structure to hold course information
struct Course {
    string courseNumber; // Unique identifier
    string courseTitle;
    vector<string> preRequisites; // Changed to vector to handle multiple prerequisites

    Course() {
        courseNumber = "";
        courseTitle = "";
        preRequisites = {};
    }
};

void displayCourse(const Course& course) {
    // Display course details
    cout << course.courseNumber << ": " << course.courseTitle << " | ";
    for (const string& prereq : course.preRequisites) {
        cout << prereq << " ";
    }
    cout << endl;
}

void loadCourses(const string& csvPath, vector<Course>& courses) {
    // Load courses from CSV file
    cout << "Loading CSV file " << csvPath << endl;

    ifstream file(csvPath);
    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Course course;

        // Get the course number
        if (getline(ss, token, ',')) {
            course.courseNumber = token;
        }

        // Get the course title
        if (getline(ss, token, ',')) {
            course.courseTitle = token;
        }

        // Get the prerequisites
        while (getline(ss, token, ',')) {
            if (!token.empty()) {
                course.preRequisites.push_back(token);
            }
        }

        courses.push_back(course); // Add course to the vector
    }

    file.close();
}

int partition(vector<Course>& courses, int begin, int end) {
    // Partition function for quick sort
    int low = begin;
    int hi = end;
    string pivot = courses[low + (hi - low) / 2].courseTitle;

    while (low <= hi) {
        // Increase low index while less than pivot
        while (low <= hi && courses[low].courseTitle < pivot) {
            ++low;
        }
        // Decrease high index while greater than pivot
        while (low <= hi && courses[hi].courseTitle > pivot) {
            --hi;
        }
        // Swap elements if low index is less than or equal to high index
        if (low <= hi) {
            std::swap(courses[low], courses[hi]);
            ++low;
            --hi;
        }
    }

    return low;
}

void quickSort(vector<Course>& courses, int begin, int end) {
    // Quick sort function
    if (begin >= end) {
        return;
    }

    int p = partition(courses, begin, end);

    if (begin < p - 1) {
        quickSort(courses, begin, p - 1); // Sort left side
    }
    if (p < end) {
        quickSort(courses, p, end); // Sort right side
    }
}

void printCourses(const vector<Course>& courses) {
    // Print the sorted courses
    for (const auto& course : courses) {
        cout << "Course Number: " << course.courseNumber << endl;
        cout << "Course Title: " << course.courseTitle << endl;
        cout << "PreRequisites: ";
        for (const string& prereq : course.preRequisites) {
            cout << prereq << " ";
        }
        cout << endl;
        cout << "_______________________" << endl;
    }
}

Course searchCourse(const vector<Course>& courses, const string& courseNumber) {
    // Search for a course by course number
    for (const auto& course : courses) {
        if (course.courseNumber == courseNumber) {
            return course;
        }
    }
    return Course(); // Return empty course if not found
}

int main(int argc, char* argv[]) {
    string csvPath, courseKey;
    switch (argc) {
    case 2:
        csvPath = argv[1]; // Get CSV path from arguments
        break;
    case 3:
        csvPath = argv[1]; // Get CSV path from arguments
        courseKey = argv[2]; // Get course key from arguments
        break;
    default:
        csvPath = "CS 300 ABCU_Advising_Program_Input.csv"; // Default CSV path
    }

    clock_t ticks;

    vector<Course> courseList;

    int choice = 0;

    while (choice != 9) {
        // Display menu
        cout << "Menu:" << endl;
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "What would you like to do: ";
        cin >> choice;

        switch (choice) {
        case 1:
            // Load courses from CSV
            ticks = clock();
            loadCourses(csvPath, courseList);
            cout << courseList.size() << " courses read" << endl;
            ticks = clock() - ticks;
            cout << "time: " << ticks << " milliseconds" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            // Print sorted course list
            quickSort(courseList, 0, courseList.size() - 1); // Sort the courses
            cout << "Here is a sample schedule." << endl;
            printCourses(courseList);
            break;

        case 3:
            // Search and print course details
        {
            cout << "What course do you want to know about? ";
            cin >> courseKey;
            ticks = clock();
            Course course = searchCourse(courseList, courseKey);
            ticks = clock() - ticks;

            if (!course.courseNumber.empty()) {
                displayCourse(course);
            }
            else {
                cout << "Course " << courseKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
        }
        break;

        default:
            cout << choice << " is not a valid option." << endl;
            break;
        }
    }

    cout << "Thank you for using the course planner!" << endl;

    return 0;
}

//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Neo Huynh
// Version     : 1.0
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

struct Course { // struct for the course, stores the infomation of the course number, course titel, and prereqs
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

struct Node { // strcut for the node, has course and node info
    Course course;
    Node* left;
    Node* right;

    Node(Course c) { // default contructor, gives basic info for the nodes
        course = c; 
        left = right = nullptr;
    }
};

class BinarySearchTree { // class for the BST
private: 
    Node* root;

    void addNode(Node* node, Course course) { // Recursively adds a course into the BST in the sorted order
        if (course.courseNumber < node->course.courseNumber) {
            if (node->left == nullptr) {
                node->left = new Node(course);
            }
            else {
                addNode(node->left, course);
            }
        }
        else {
            if (node->right == nullptr) {
                node->right = new Node(course);
            }
            else {
                addNode(node->right, course);
            }
        }
    }
    
    void inOrder(Node* node) { // In-order traversal prints courses in alphanumeric order
        if (node == nullptr) {
            return;
        }

        inOrder(node->left);
        cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
        inOrder(node->right);
    }

    Course search(Node* node, string courseNumber) { // searches for a course by the course number in the BST
        if (node == nullptr) {
            return Course{};
        }

        if (node->course.courseNumber == courseNumber) {
            return node->course;
        }
        if (courseNumber < node->course.courseNumber) {
            return search(node->left, courseNumber);
        }
        return search(node->right, courseNumber);
    }
public: // constructor inits the empty BST
    BinarySearchTree() {
        root = nullptr;
    }

    void insert(Course course) { // inserts a course into the bst
        if (root == nullptr) {
            root = new Node(course);
        }
        else {
            addNode(root, course);
        }
    }

    void printCourseList() { // prints all the courses in sorted order
        cout << endl << "Course List: " << endl;
        inOrder(root);
    }

    void printCourse(string courseNumber) { // prints detailed info about a inputted course
        Course c = search(root, courseNumber);

        if (c.courseNumber == "") {
            cout << "Course not found." << endl;
            return;
        }

        cout << endl << c.courseNumber << ", " << c.courseTitle << endl;
        
        cout << "Prerequisites: ";

        if (c.prerequisites.empty()) {
            cout << "None";
        }
        else {
            for (int i = 0; i < c.prerequisites.size(); i++) {
                cout << c.prerequisites[i] << " ";
            }
        }

        cout << endl;
    }

};

BinarySearchTree bst; // global bst

string trim(string str) { // removes leading and trailing whitespace/newline characters
    while (!str.empty() && (str.front() == ' ' || str.front() == '\r'))
        str.erase(str.begin());

    while (!str.empty() && (str.back() == ' ' || str.back() == '\r'))
        str.pop_back();

    return str;
}

void loadCourses(string filename) { // reads the CSV file and loads course data into BST
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Could not open file." << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;

        Course course;

        getline(ss, token, ',');
        course.courseNumber = trim(token);

        getline(ss, token, ',');
        course.courseTitle = trim(token);

        while (getline(ss, token, ',')) {
            token = trim(token);
            if (!token.empty()) {
                course.prerequisites.push_back(token);
            }
        }

        bst.insert(course);
    }

    cout << "Courses loaded successfully." << endl;
}

string upper(string str) { // converts lowercase to uppercase, used for option 3 since csci400 didn't initally work
    for (char& c : str) {
        c = toupper(c);
    }
    return str;
}

int main(int argc, char* argv[]) {

    string filename;
    string courseNumber;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        switch (choice) {

        case 1:
            cout << "Enter File Name: " << endl;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, filename);
            loadCourses(filename);
            break;

        case 2:
            bst.printCourseList();
            break;

        case 3:
            cout << "Enter Course number: ";
            cin >> courseNumber;
            courseNumber = upper(trim(courseNumber));
            bst.printCourse(courseNumber);
            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;
        default:
            cout << choice << " is not a valid option." << endl;
            break;
        }
    }

    return 0;
}

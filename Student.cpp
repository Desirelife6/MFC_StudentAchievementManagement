//
// Created by Desirelife on 2019/7/30.
//
#include <iostream>
#include <iomanip>
#include "Student.h"
#include "Course.h"

#define ERROR -1;
using namespace std;

Student::Student(int courseNum, string studentId, string studentName, vector<Course> studentCourses, double weightedScore) {
    this->courseNum = courseNum;
    this->studentId = studentId;
    this->studentName = studentName;
    this->studentCourses = studentCourses;
    this->weightedScore = weightedScore;
}

double Student::getSingleCourseScore(const string &courseName) {
    for (Course course: studentCourses) {
        if (course.searchByCourseName(courseName)) {
            return course.score;
        }
    }
    cout << "û�����ſγ̣��������Ĺؼ���ƴд" << endl;
    return ERROR
}

void Student::countWeightedScore() {
    double creditSum = 0;
    double countingScore = 0;
    for (const Course &course: studentCourses) {
        creditSum += course.credit;
    }
    for (const Course &course: studentCourses) {
        double weight = course.credit / creditSum;
        countingScore += course.score * weight;
        this->weightedScore = countingScore;
    }
}

bool Student::searchByStudentId(string studentId) {
    return this->studentId == studentId;
}

bool Student::searchByStudentName(string studentName) {
    return this->studentName == studentName;
}

Student *Student::searchStudentByKeyword(const string &searchKeyword) {
    if (searchByStudentId(searchKeyword) || searchByStudentName(searchKeyword)) {
        Student *studentPtr = this;
        return studentPtr;
    }
    return nullptr;
}

Course *Student::getSingleCourse() {
    int choice;
    Course *courseChosen = nullptr;
    cout << "����Ҫѡ�����ſγ�? " << endl;
    for(int i = 0; i < studentCourses.size(); i++){
        cout << i + 1 << "." << studentCourses[i].courseName << endl;
    }
    cin >> choice;
    courseChosen = &studentCourses[choice - 1];
//    switch (choice){
//        case 1:
//            courseChosen = &studentCourses[0];
//            break;
//        case 2:
//            courseChosen = &studentCourses[1];
//            break;
//        default:
//            cout << "Check your choice!" << endl;
//    }
    return courseChosen;
}

void Student::display() {
    cout << "ѧ��������" << studentName << "\t";
    cout << "ѧ�ţ�" << studentId << endl;
    cout << " ��Ŀ" << setw(8) << "ѧ��" << setw(8) << "�ɼ�" << setw(8) << "����" << endl;
    for (const Course &course: studentCourses) {
        cout << course.courseName << setw(8) << course.credit << setw(8) << course.score << setw(8) << endl;
    }
    cout << "��Ȩ�ɼ���" << weightedScore << endl;
    cout << endl;
}



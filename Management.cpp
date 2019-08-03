//
// Created by Desirelife on 2019/7/30.
//

#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <map>
#include <algorithm>
#include <io.h>
#include "Management.h"

using namespace std;

void Management::addStudent() {
    Student s;
    Course course;
    cout << "������Ҫ��ӵ�ѧ��ѧ��: ";
    cin >> s.studentId;
    for (Student student: students) {
        if (student.searchByStudentId(s.studentId)) {
            cout << "��ѧ����Ϣ�Ѿ����ڣ�" << endl;
            return;
        }
    }
    cout << "����ѧ������: ";
    cin >> s.studentName;
    cout << "����ѧ���Ŀγ���Ŀ: ";
    cin >> s.courseNum;
    for(int i = 0; i < s.courseNum; i++) {
        cout << "��������γ����ƣ�ѧ�ֺͳɼ����Կո������: ";
        cin >> course.courseName >> course.credit >> course.score;
        s.studentCourses.push_back(course);
    }
    s.countWeightedScore();
    students.push_back(s);
    cout << "����ӵ�ѧ����ϢΪ: " << endl;
    s.display();
    storeFile();
}

Student *Management::searchStudentByKeyword() {
    string searchKeyword;
    cout << "���������ѯ��ѧ��������ѧ��: ";
    cin >> searchKeyword;
    for (Student &student: students) {
        Student *studentPtr = student.searchStudentByKeyword(searchKeyword);
        if (studentPtr != nullptr) return studentPtr;
    }
    cout << "�޸�ѧ�����������Ĺؼ���ƴд" << endl;
    return nullptr;
}

void Management::showSingleStudent() {
    Student *student = searchStudentByKeyword();
    if (student != nullptr) {
        cout << "----------����ѯ��ѧ����Ϣ---------- " << endl;
        student->display();
        storeFile();
        return;
    }
}

void Management::deleteStudent() {
    string searchKeyword;
    cout << "������ɾ����ѧ��ѧ�Ż�����: ";
    cin >> searchKeyword;
    for (int i = 0; i < students.size(); i++) {
        if (students[i].searchStudentByKeyword(searchKeyword)) {
            students.erase(students.begin() + i);
            cout << "�ɹ�ɾ��!" << endl;
            storeFile();
            return;
        }
    }
    cout << "û�д�ѧ��! ��������ؼ���!" << endl;
}

void Management::alterScore() {
    Student *student = searchStudentByKeyword();
    if(student == nullptr)  return;
    Course *courseChosen = student->getSingleCourse();
    if(courseChosen == nullptr)  return;
    cout << "�����µ�" << courseChosen->courseName << "�ɼ�: ";
    cin >> courseChosen->score;
    student->countWeightedScore();
    cout << "�޸ĺ�ĸ�����ϢΪ: " << endl;
    student->display();
    storeFile();
}

bool cmp(const Student &a, const Student &b) {
    return a.weightedScore > b.weightedScore;
}

bool singleCmp(pair<pair<string, string>, double> &a, pair<pair<string, string>, double> &b) {
    return a.second > b.second;
}

void Management::getRankingBySingleCourseScore() {
    string searchKeyword;
    cout << "���������ѯ�Ŀγ����ƣ�";
    cin >> searchKeyword;
    map<pair<string, string>, double> scores;
    vector<pair<pair<string, string>, double>> sortedScores;
    for (Student &student: students) {
        double score = student.getSingleCourseScore(searchKeyword);
        if (score == -1)
            return;
        scores.insert(pair<pair<string, string>, double>(pair<string, string>(student.studentId, student.studentName),
                                                         score));
        sortedScores.emplace_back(pair<string, string>(student.studentId, student.studentName),
                                  score);
    }
    sort(sortedScores.begin(), sortedScores.end(), singleCmp);
    cout << "----------" << searchKeyword << "�ĳɼ�����----------" << endl;
    cout << "ѧ��\t" << "����\t" << "�ɼ�\t" << endl;
    for (auto &iter : sortedScores) {
        cout << iter.first.first << "\t" << iter.first.second << "\t" << iter.second << endl;
    }
}

void Management::getRankingByWeightedScore() {
    vector<Student> sortedStudents;
    for (Student &student: students) {
        student.countWeightedScore();
        sortedStudents.push_back(student);
    }
    sort(sortedStudents.begin(), sortedStudents.end(), cmp);
    for (auto &student: sortedStudents) {
        cout << student.studentId << "\t" << student.studentName << "\t" << student.weightedScore << endl;
    }
}

void Management::storeFile() {
    ofstream outfile(R"(F:\CLion\CLionProjects\AchievementManagement\StudentsInfo.txt)");
    if (!outfile) {
        cout << "No data!" << endl;
        return;
    }
    for (int i = 0; i < students.size(); i++) {
        outfile << students[i].studentId << " " << students[i].studentName << " " << students[i].courseNum << " ";
        for(int j = 0; j < students[j].courseNum; j++){
            outfile << students[i].studentCourses[j].courseName << " " << students[i].studentCourses[j].credit << " " << students[i].studentCourses[j].score << " ";
        }
        if(i == students.size() - 1){
            outfile << students[i].weightedScore;
        } else{
            outfile << students[i].weightedScore << endl;
        }
    }
    outfile.close();
}

void Management::loadFile() {
    Student s;
    ifstream infile(R"(F:\CLion\CLionProjects\AchievementManagement\StudentsInfo.txt)");
    if (!infile.is_open()) {
        cout << "No data!" << endl;
        return;
    }
    int coursesNum;
    double weightedScore;
    string studentId, studentName;
    Course course;
    vector<Course> courses;

    while(!infile.eof()){
        infile >> studentId >> studentName >> coursesNum;
        for(int i = 0; i < coursesNum; i++) {
            infile >> course.courseName >> course.credit >> course.score;
            courses.push_back(course);
        }
        infile >> weightedScore;
        Student student = Student(coursesNum, studentId, studentName, courses, weightedScore);
        students.push_back(student);
        courses.clear();
    }
    infile.close();
}

void Management::showAllStudents() {
    cout << "----------����ѧ���ɼ�ͳ��----------" << endl;
    for (Student student: students) {
        student.display();
    }
}



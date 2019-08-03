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
        double score;
        cin >> course.courseName >> course.credit >> score;
        course.setScore(score);
        s.addCourseToList(course);
        if(courses.find(course.courseName) == courses.end()){
            courses[course.courseName] = course;
            courses[course.courseName].num = 1;
            courses[course.courseName].gpaSum = course.getGpa();
        } else {
            score += courses[course.courseName].getScore();
            courses[course.courseName].setScore(score);
            courses[course.courseName].num++;
            courses[course.courseName].gpaSum += course.getGpa();
        }
    }
    s.setCreditsSum();
    s.setWeightedScore();
    s.setGpa();
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
        cout << endl;
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
    double score;
    cin >> score;
    courseChosen->setScore(score);
    courseChosen->setGpa();
    student->setWeightedScore();
    student->setGpa();
    cout << "�޸ĺ�ĸ�����ϢΪ: " << endl;
    student->display();
    storeFile();
}

bool cmp(Student &a, Student &b) {
    double aws = a.getWeightedScore();
    double bws = b.getWeightedScore();
    return aws > bws;
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
    cout << endl;
    cout << "ѧ��\t" << "����\t" << "�ɼ�\t" << endl;
    for (auto &iter : sortedScores) {
        cout << iter.first.first << "\t" << iter.first.second << "\t" << iter.second << endl;
    }
}

void Management::getRankingByWeightedScore() {
    vector<Student> sortedStudents;
    for (Student &student: students) {
        student.setWeightedScore();
        student.setGpa();
        sortedStudents.push_back(student);
    }
    sort(sortedStudents.begin(), sortedStudents.end(), cmp);
    cout << "----------��Ȩ�ɼ�����----------" << endl;
    cout << endl;
    cout << "ѧ��\t" << "����\t" << "��Ȩ�ɼ�\t" << "ƽ������" << endl;
    for (auto &student: sortedStudents) {
        cout << student.studentId << "\t" << student.studentName << "\t" << student.getWeightedScore() << "\t" << student.getGpa() << endl;
    }
    storeFile();
}

void Management::storeFile() {
    ofstream outfile(R"(F:\CLion\CLionProjects\AchievementManagement\StudentsInfo.txt)");
    if (!outfile) {
        cout << "No data!" << endl;
        return;
    }
    for (int i = 0; i < students.size(); i++) {
        outfile << students[i].studentId << " " << students[i].studentName << " " << students[i].courseNum << " ";
        for(int j = 0; j < students[i].courseNum; j++){
            outfile << students[i].getCourseFromList(j)->courseName << " " << students[i].getCourseFromList(j)->credit << " " << students[i].getCourseFromList(j)->getScore() << " ";
        }
        if(i == students.size() - 1){
            outfile << students[i].getWeightedScore() << " " << students[i].getCreditsSum();
        } else{
            outfile << students[i].getWeightedScore() << " " << students[i].getCreditsSum() << endl;
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
    int coursesNumbers;
    double weightedScore,creditSum;
    string studentId, studentName;
    Course course;
    vector<Course> tempCourses;

    while(!infile.eof()){
        infile >> studentId >> studentName >> coursesNumbers;
        for(int i = 0; i < coursesNumbers; i++) {
            double score;
            infile >> course.courseName >> course.credit >> score;
            course.setScore(score);
            course.setGpa();
            tempCourses.push_back(course);
            if(courses.find(course.courseName) == courses.end()){
                courses[course.courseName] = course;
                courses[course.courseName].num = 1;
                courses[course.courseName].gpaSum = course.getGpa();
            } else {
                score += courses[course.courseName].getScore();
                courses[course.courseName].setScore(score);
                courses[course.courseName].num++;
                courses[course.courseName].gpaSum += course.getGpa();
            }
        }
        infile >> weightedScore >> creditSum;
        Student student = Student(coursesNumbers, studentId, studentName, tempCourses, weightedScore, creditSum);
        students.push_back(student);
        tempCourses.clear();
    }
    infile.close();
}

void Management::showAllStudents() {
    cout << "----------����ѧ���ɼ�ͳ��----------" << endl;
    cout << endl;
    for (Student& student: students) {
        student.setGpa();
        student.display();
    }
}

void Management::showAllCourses() {
    cout << "----------���п�Ŀ��Ϣͳ��----------" << endl;
    cout << endl;
    map<string, Course>::iterator iter;
    for (iter = courses.begin(); iter != courses.end(); iter++) {
        double averageScore = iter->second.getScore() / iter->second.num;
        iter->second.setScore(averageScore);
        cout << "��Ŀ���ƣ�" << iter->first << "\t" << "��Ŀѧ�֣�" << iter->second.credit << endl;
        cout << "���ƽ���ɼ�: " << averageScore << "\t";
        cout << "�꼶ƽ�����㣺" << iter->second.gpaSum / iter->second.num << endl;
        cout << endl;
    }
}

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

Student* judgeReaptedName(vector<Student*> searchResults);

void Management::addStudent() {
    int courseNum;
    string studentId,studentName;
    vector<Course> addCourses;
    cout << "������Ҫ��ӵ�ѧ��ѧ��: ";
    cin >> studentId;
    for (Student student: students) {
        if (student.searchByStudentId(studentId)) {
            cout << "��ѧ����Ϣ�Ѿ����ڣ�" << endl;
            return;
        }
    }
    cout << "����ѧ������: ";
    cin >> studentName;
    cout << "����ѧ���Ŀγ���Ŀ: ";
    cin >> courseNum;
    for(int i = 0; i < courseNum; i++) {
        cout << "��������γ����ƣ�ѧ�ֺͳɼ����Կո������: ";
        string courseName;
        double score, credit;
        cin >> courseName >> credit >> score;
        //ʹ��builder���ģʽʵ���� Course��Ķ���
        Course course = CourseBuilder()
                .courseName(courseName)
                .credit(credit)
                .score(score)
                .gpa()
                .build();
        addCourses.push_back(course);
        countCourses(course, score);    //ͳ���¼���Ŀγ���Ϣ�����Ը��¸ÿγ��꼶ƽ���ɼ�
    }
    //ʹ��builder���ģʽʵ���� Student��Ķ���
    Student student = StudentBuilder()
            .courseNum(courseNum)
            .studentId(studentId)
            .studentName(studentName)
            .studentCourses(addCourses)
            .creditsSum()
            .weightedScore()
            .gpa()
            .build();
    students.push_back(student);
    cout << "����ӵ�ѧ����ϢΪ: " << endl;
    student.display();
    storeFile();        //�洢��Ϣ
}

/*ʹ��vector������ѧ������ʱ��չʾ���в�ѯ���*/
vector<Student*> Management::searchStudentByKeyword() {
    string searchKeyword;
    cout << "���������ѯ��ѧ��������ѧ��: ";
    cin >> searchKeyword;
    vector<Student*> searchResults;
    for (Student &student: students) {
        Student *studentPtr = student.searchStudentByKeyword(searchKeyword);    //����Student��ķ������в�ѯ
        if (studentPtr != nullptr) {
            searchResults.push_back(studentPtr);
        }
    }
    if(searchResults.empty()){
        cout << "�޸�ѧ�����������Ĺؼ���ƴд" << endl;
    }
    return searchResults;
}

void Management::showSingleStudent() {
    vector<Student*> student = searchStudentByKeyword();        //����Student��� searchStudentByKeyword�������в�ѯ
    if (! student.empty()) {
        cout << "----------����ѯ��ѧ����Ϣ---------- " << endl;
        cout << endl;
        for(int i = 0; i < student.size(); i++){
            student[i]->display();
        }
        storeFile();
        return;
    }
}

void Management::deleteStudent() {
    string searchKeyword;
    cout << "������ɾ����ѧ��ѧ�Ż�����: ";
    cin >> searchKeyword;
    /*�˴������߼���searchStudentByKeyword�����ƣ������ڽ�����ͬ����ֱ�ӵ���*/
    vector<Student*> searchResults;
    Student* studentPtr = nullptr;
    for (Student &student: students) {
        studentPtr = student.searchStudentByKeyword(searchKeyword);    //����Student��ķ������в�ѯ
        if (studentPtr != nullptr) {
            searchResults.push_back(studentPtr);
        }
    }
    if(searchResults.empty()) {
        cout << "�޸�ѧ�����������Ĺؼ���ƴд" << endl;
        return;
    }
    /*���������ж�*/
    studentPtr = judgeReaptedName(searchResults);
    if(studentPtr == nullptr)   return;

    for (int i = 0; i < students.size(); i++) {
        if (students[i].searchStudentByKeyword(studentPtr->studentId)) {
            students.erase(students.begin() + i);
            cout << "�ɹ�ɾ��!" << endl;
            storeFile();
            return;
        }
    }
}

void Management::alterScore() {
    /*����Student��� searchStudentByKeyword�� getSingleCourse����ȷ��Ҫ�޸ĵ�ѧ���Ϳγ�*/
    vector<Student*> searchResults = searchStudentByKeyword();
    if(searchResults.empty()) return;

    /*�ж�ѧ�����������*/
    Student* studentPtr = judgeReaptedName(searchResults);
    if(studentPtr == nullptr)   return;
    /*ѡ��ѧ����Կγ̵Ĳ���*/
    Course *courseChosen = studentPtr->getSingleCourse();
    if(courseChosen == nullptr)  return;
    cout << "�����µ�" << courseChosen->courseName << "�ɼ�: ";
    double score;
    cin >> score;
    /*�Կγ����óɼ��ͼ��㣬 ��ѧ�������Ȩ�ͼ���*/
    courseChosen->setScore(score);
    courseChosen->setGpa();
    studentPtr->setWeightedScore();
    studentPtr->setGpa();
    cout << "--------�޸ĺ�ĸ�����ϢΪ----------- " << endl;
    cout << endl;
    studentPtr->display();
    storeFile();
}

/**
 * cmp �� singleCmpΪ�Գɼ�����ʱ�ĸ���������������д sort���������ڲ���Ҫ���ʳ�Ա������δ��.h������
 */
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
    map<pair<string, string>, double> scores;       //����map��pairʵ����Ԫ��洢�γ̺�ѧ����Ϣ
    vector<pair<pair<string, string>, double>> sortedScores;    //����vector����дsort������������
    /*����ѧ���б��ȡ�ɼ���Ϣ*/
    for (Student &student: students) {
        double score = student.getSingleCourseScore(searchKeyword);
        if (score == 0)
            continue;
        scores.insert(pair<pair<string, string>, double>(pair<string, string>(student.studentId, student.studentName),
                                                         score));
        sortedScores.emplace_back(pair<string, string>(student.studentId, student.studentName),
                                  score);
    }
    sort(sortedScores.begin(), sortedScores.end(), singleCmp);      //sort��������
    //���������
    cout << "----------" << searchKeyword << "�ĳɼ�����----------" << endl;
    cout << endl;
    cout << "ѧ��\t" << "����\t" << "�ɼ�\t" << endl;
    for (auto &iter : sortedScores) {       //���õ��������б���
        cout << iter.first.first << "\t" << iter.first.second << "\t" << iter.second << endl;
    }
}

void Management::getRankingByWeightedScore() {
    vector<Student> sortedStudents;
    for (Student &student: students) {      //������ȡ�ɼ���Ϣ
        sortedStudents.push_back(student);
    }
    sort(sortedStudents.begin(), sortedStudents.end(), cmp);        //��дsort��������
    cout << "----------��Ȩ�ɼ�����----------" << endl;
    cout << endl;
    cout << "ѧ��\t" << "����\t" << "��Ȩ�ɼ�\t" << "ƽ������" << endl;
    for (auto &student: sortedStudents) {
        cout << student.studentId << "\t" << student.studentName << "\t" << student.getWeightedScore() << "\t" << student.getGpa() << endl;
    }
    storeFile();
}

void Management::showAllStudents() {
    cout << "----------����ѧ���ɼ�ͳ��----------" << endl;
    cout << endl;
    for (Student& student: students) {
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

void Management::storeFile() {
    ofstream outfile(R"(F:\CLion\CLionProjects\AchievementManagement\StudentsInfo.txt)");
    if (!outfile) {
        cout << "No data!" << endl;
        return;
    }
    for (int i = 0; i < students.size(); i++) {
        outfile << students[i].studentId << " " << students[i].studentName << " " << students[i].courseNum << endl;
        for(int j = 0; j < students[i].courseNum; j++){     //��ʽ���洢��ʽ
            if(i == students.size() - 1){
                if(j == students[i].courseNum - 1){
                    outfile << students[i].getCourseFromList(j)->courseName << " " << students[i].getCourseFromList(j)->credit << " " << students[i].getCourseFromList(j)->getScore();
                }else{
                    outfile << students[i].getCourseFromList(j)->courseName << " " << students[i].getCourseFromList(j)->credit << " " << students[i].getCourseFromList(j)->getScore() << endl;
                }
            } else{
                outfile << students[i].getCourseFromList(j)->courseName << " " << students[i].getCourseFromList(j)->credit << " " << students[i].getCourseFromList(j)->getScore() << endl;
            }
        }

    }
    outfile.close();
}

void Management::loadFile() {
    ifstream infile(R"(F:\CLion\CLionProjects\AchievementManagement\StudentsInfo.txt)");
    if (!infile.is_open()) {
        cout << "No data!" << endl;
        return;
    }
    int coursesNumbers;
    string studentId, studentName;
    vector<Course> tempCourses;
    courses.clear();
    while(!infile.eof()){
        infile >> studentId >> studentName >> coursesNumbers;
        for(int i = 0; i < coursesNumbers; i++) {
            string courseName;
            double score, credit;
            infile >> courseName >> credit >> score;
            Course course = CourseBuilder()
                    .courseName(courseName)
                    .credit(credit)
                    .score(score)
                    .gpa()
                    .build();
            tempCourses.push_back(course);
            countCourses(course, score);        //ͳ�ƿγ���Ϣ���Լ����꼶ƽ���ɼ�
        }
        Student student = StudentBuilder()
                .courseNum(coursesNumbers)
                .studentId(studentId)
                .studentName(studentName)
                .studentCourses(tempCourses)
                .creditsSum()
                .weightedScore()
                .gpa()
                .build();
        students.push_back(student);
        tempCourses.clear();
    }
    infile.close();
}

/*���������������ѧ�Ŷ����ж�*/
Student* judgeReaptedName(vector<Student*> searchResults){
    Student* studentPtr;
    if(searchResults.size() > 1){     //�ж��Ƿ���ѧ������
        int choice;
        cout << "����Ҫѡ���ѧ���ǣ�" << endl;
        for(int i = 0; i < searchResults.size(); i++){
            cout << i + 1 << "." << searchResults[i]->studentId << " " << searchResults[i]->studentName << endl;
        }
        cout << "��������ѡ��" ;
        cin >> choice;
        if (choice < 1 || choice > searchResults.size()){     //������ܳ��ֵ�������������������������ı���
            cout << "��������ѡ���Ƿ�����" << endl;
            return nullptr;
        }
        studentPtr = searchResults[choice - 1];      //ȷ����ѡ���ѧ��
    } else{
        studentPtr = searchResults[0];
    }
    return studentPtr;
}

/*��ÿ�ų��ֵĿγ���Ϣ���뵽ͳ�����пγ���Ϣ�� map��*/
void Management::countCourses(Course course, double score){
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



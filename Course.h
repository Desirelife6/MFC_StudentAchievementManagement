//
// Created by Desirelife on 2019/8/2.
//

#ifndef ACHIEVEMENTMANAGEMENT_COURSE_H
#define ACHIEVEMENTMANAGEMENT_COURSE_H

#include <string>

using namespace std;

class Course {
public:
    string courseName;
    double credit;
    double score;
public:
    explicit Course(string courseName = "0", double credit = 0);
    bool searchByCourseName(const string &searchName);
};


#endif //ACHIEVEMENTMANAGEMENT_COURSE_H
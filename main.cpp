#include <iostream>
#include "Student.h"
#include "Management.h"
using namespace std;

int main()
{
    //TODO: quite a lot of work
    Management management;
    management.loadFile();
    int n = 1000;
    char choice;
    string mathScore;
    while(n--){
        cout << "1. ���ѧ��" << endl;
        cout << "2. չʾ����ѧ������Ϣ" << endl;
        cout << "3. ͨ��������ѧ�Ų�ѯѧ����Ϣ" << endl;
        cout << "4. ͨ��������ѧ��ɾ��ѧ����Ϣ" << endl;
        cout << "5. �޸�ĳѧ���ɼ�" << endl;
        cout << "6. ���Ƴɼ�����" << endl;
        cout << "7. ��Ȩ�ɼ�����" << endl;
        cout << "0. ��ȫ�˳�" << endl;
        cout << "����������ѡ��: ";
        choice = getchar();
        switch (choice){
            case '1':
                management.addStudent();
                break;
            case '2':
                management.showAllStudents();
                break;
            case '3':
                management.showSingleStudent();
                break;
            case '4':
                management.deleteStudent();
                break;
            case '5':
                management.alterScore();
                break;
            case '6':
                management.getRankingBySingleCourseScore();
                break;
            case '7':
                management.getRankingByWeightedScore();
                break;
            case '0':
                management.storeFile();
                return 0;
            default:
                continue;
        }
        cout << endl;
        cout << "----------Press Enter to Continue----------" << endl;
        getchar();
    }
    return 0;
}

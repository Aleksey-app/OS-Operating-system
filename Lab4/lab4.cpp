#include<iostream>                                          // штатная C++  ввод вывод и т.д                              
#include<unistd.h>                                          // для функции sleep и pid_t
using std::cout;
using std::endl;
int main(int one, char* argv[]) {
    pid_t lab4;                                             // идентификатор процесса
    // getpid() получение идентификатора текущего процесса,
    // getppid() получение идентификатора родительского процесса.
    cout << "\nprog2: Дочерний процесс PID: " << getpid() << endl;
    cout << "prog2: Родительский процесс PID: " << getppid() << endl;
    cout << "Получено значение " << one << " аргументов." << endl;
    for (int i = 0; i < one; i++) {
        cout << i << " : " << argv[i] << endl;
        sleep(1);}
    return 0;                                               // сформировал код завершения
}

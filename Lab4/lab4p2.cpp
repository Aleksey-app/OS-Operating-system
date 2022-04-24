#include<iostream>                                          // штатная C++  ввод вывод и т.д                              
#include<unistd.h>                                          // для функции sleep и write&read 
#include <sys/wait.h>                                       // для функции waitpid
using std::cout;
using std::endl;
int main(){
    const char* file = "lab4";                              
    int error = 0;
    const char* ar = "test";
    const char* ar2 = "45678";
    const char* ar3 = "lab5";
    const char* ar4 = "4444";
    pid_t lab4;                                             // идентификатор процесса
    lab4 = fork();                                          // уже 2 процесса
    if (lab4 == -1) {
         perror("lab4[-]");
         exit(EXIT_FAILURE);}                               // обработал ошибку и завершил работу программы
    if (lab4 == 0) {                                        // дочерний процесс
        cout <<  "\x1B[0;31;40mprog: Дочерний процесс PID: " << getpid() <<"\x1b[0m" << endl;
        sleep(1);
        error = execlp(file, ar, ar2, ar3, ar4, NULL);      // функция запуска программ по заданию execlp
        if (error == -1) {
           perror("EXEClp[-]");
           exit(EXIT_FAILURE);}
         }else{
             cout << "\n\x1B[0;31;40mprog: Родительский процесс PID: " << getpid() <<"\x1b[0m" << endl;
             int status = 0;                                // результат завершения дочернего процесса
             while (waitpid(lab4, &status, WNOHANG) == 0) {
                 cout << "\x1B[0;31;40mЖдём"<<"\x1b[0m" << endl;
                 sleep(1);}
              if (WIFEXITED(status)) {                      // возвращает истинное значение если потомок нормально завершился
                 cout << "\n\x1B[0;31;40mprog: Дочерний процесс успешно завершился с кодом: " << WEXITSTATUS(status) <<"\x1b[0m" << endl;
             }else if (WIFSTOPPED(status)) {
                 cout << "\n\x1B[0;31;40mprog: Дочерний процесс завершился с кодом: " << WSTOPSIG(status) <<"\x1b[0m" << endl;}
         }   
 return 0;  
}

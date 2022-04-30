#include<iostream>                                          // штатная C++  ввод вывод и т.д                              
#include<unistd.h>                                          // для функции sleep и write&read 
#include <semaphore.h>                                      // для семафора, sem_open();и флагов для неё
#include <sys/wait.h>                                       // для функции sem_open();и флагов для неё
#include <stdio.h>                                          // для функции fopen();открывает файл и связывает его с потоком; + fflush();
#include <fcntl.h>                                          // для функции sem_open();и флагов для неё
using std::string;
using std::cout;
using std::endl;
sem_t* traffic;
const char* s = "task";                                     // именованный семафор
FILE *fil;                                                  //.....
const char* name = "C++.txt";                               // имя
struct stream{
    bool flag;                                              // флаг выполнения цикла
    int num;                                                // вывод цифра
    string txt;                                             // вывод строк
};
void* record(void* arg) {
    stream *f1 = (stream*) arg;
    cout << f1->txt << endl;
    while (f1->flag)
    {   
         if (!sem_wait(traffic)) {                          // входе в критический участок
         for (int i = 0; i <= 4; i++) {
             cout << f1->num;
             fputc(f1->num,fil);                            // выводит символ int в поток
             if (fflush(fil) != 0) {                        // записывает все буферизированные данные
                 perror("fflush[-]");
                 exit(EXIT_FAILURE);}
            usleep(1000);
         }
         cout << endl;
         if (sem_post(traffic) == -1) {                    // выход из критического участка
             perror("sem_post[-]");
             cout << "Разблокировать семафор не удалось " << endl;
             if (sem_close(traffic) == -1) {                // именованный семафор закрывается
                perror("sem_close[-]");
             }else{sem_close(traffic);
             cout << "sem_close ok" << endl;}
             if (sem_unlink(s) == -1) {                     // именованный семафор удаляется
                perror("sem_unlink[-]");
             }else{sem_unlink(s);
             cout << "sem_unlink ok" << endl;}
            exit(EXIT_FAILURE);}
        sleep(1);}
    }
    f1->txt = "Поток закончил работу";
    pthread_exit(arg);
}
int main() {
    cout <<"\x1B[0;31;40mПрограмма начала работу..." <<"\x1b[0m";
    fil = fopen(name, "a+");                                // a+ Открыть или создать для чтения и дописывания (записи в конец файла).
    traffic = sem_open(s, O_CREAT, 0644, 1);                // O_CREAT - семафор создается, если его не существует; открывается, если есть
    pthread_t id4321;
    stream arg = {true, 2, "Поток 2 начал работу..."};
    if (pthread_create(&id4321,NULL,record,(void*) &arg) == -1) {
        perror("pthread_create[-]");
        exit(EXIT_FAILURE);}
    cout <<"\n"<<"\x1B[0;31;40mПрограмма ждёт нажатия клавиши enter..."<<"\x1b[0m" << endl;
    void *status;
    getchar();
    arg.flag = false;
    cout <<"\n"<<"\x1B[0;31;40mКлавиша нажата."<<"\x1b[0m" << endl;
    int q = pthread_join(id4321,&status);                   // если поток не завершить
    if (q != 0) {
         perror("pthread_join[-]");
             if (sem_close(traffic) == -1) {                // именованный семафор закрывается
                perror("sem_close[-]");
             }else{sem_close(traffic);
             cout << "sem_close ok" << endl;}
             if (sem_unlink(s) == -1) {                     // именованный семафор удаляется
                perror("sem_unlink[-]");
             }else{sem_unlink(s);
             cout << "sem_unlink ok" << endl;}
            exit(EXIT_FAILURE);}
    stream *ex = (stream*) status;
    cout << endl << ex->num << " " << ex->txt << endl;
    sem_close(traffic);
    sem_unlink(s);
    cout <<"\x1B[0;31;40mПрограмма завершила работу."<<"\x1b[0m" << endl;
    return 0;
}

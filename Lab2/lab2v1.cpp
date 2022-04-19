#include<iostream>
#include<unistd.h>  //для функции sleep
#include <semaphore.h>  //для семафора
using std::string;
using std::cout;
using std::endl;
struct stream{
    bool flag;  //флаг выполнения цикла
    int num;    //вывод цифра
    string txt; //вывод строка
}st;
sem_t traffic;
void* process (void* arg){
    stream *f1 = (stream*) arg;
    cout << f1->txt;
    sleep(2);
    while (f1->flag)
    { 
     if (!sem_wait(&traffic)) {
         for (int i = 0; i <= 4; i++) {
             cout <<f1->num;
             usleep(10000);
         }
         cout <<endl; 
         if (sem_post(&traffic) == -1) {
             perror("[-]");}
     }
     sleep(1);
    }
    cout <<"\n"<<f1->txt;
    f1->txt = "Поток закончил работу";
    pthread_exit(arg);
}
int main() {
    cout <<"\x1B[0;31;40mПрограмма начала работу..." <<  endl;
    cout <<"С блокирующей операцией захвата семафора (sem_wait())."<<"\x1b[0m"<< endl;
    if (sem_init(&traffic, 0, 1) !=0){
        perror("[-]");
    }
    int error=0, error2=0;
    pthread_t id4321;  //id первого потока
    stream arg1 = {true, 1, "Поток 1 начал работу..."};
    error=pthread_create(&id4321,NULL,process,(void*) &arg1);
    if (error){
        perror("[-]");
    }
    pthread_t id4322; //id второго потока
    stream arg2 = {true, 2, "Поток 2 начал работу..."};
    error2=pthread_create(&id4322,NULL,process,(void*) &arg2);
    if (error2){
        perror("[-]");
    }
    sleep(1);
    cout <<"\n"<<"\x1B[0;31;40mПрограмма ждёт нажатия клавиши..."<<"\x1b[0m" << endl;
    void *status;
    getchar();
    arg1.txt ="Клавиша нажата.";
    arg1.flag = false;
    pthread_join(id4321,&status);
    stream *ex = (stream*) status;
    cout <<endl<<"1 " << ex->txt;
    arg2.txt =" ";
    arg2.flag = false;
    pthread_join(id4322,&status);
    ex = (stream*) status;
    cout << endl<<"2 " << ex->txt << endl;
    cout <<"\x1B[0;31;40mПрограмма завершила работу."<<"\x1b[0m" << endl;
    sem_close(&traffic);
    return 0;
}

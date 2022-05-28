#include <iostream>                                         // штатная C++  ввод вывод и т.д 
#include <stdio.h>                                          // штатная C  ввод вывод и т.д
#include <unistd.h>                                         // для функции sleep
#include <mqueue.h>                                         // очереди сообщений 
using std::cout;
using std::endl;
mqd_t info7;                                                // идентификатор очереди сообщений
unsigned prio;                                              // приоритет сообщения
char str[16] = {0};                                         // буфер
void* stream (void* arg) {
    struct timespec ts;
    while (!(* (bool*) arg)) { 
    ts.tv_sec = time(NULL) + 3;
    ts.tv_nsec = 0;
       // принять сообщение из очереди сообщений в буфер
       if (mq_timedreceive(info7, str, 64, &prio, &ts) != -1) {
            // вывести результат работы функции на экран
            cout << "Сообщение : " << str << " приоритет : " << prio << endl;} 
           else{
                cout << "Тайм-аут 3c при ожидании сообщения" << endl;}
                
        sleep(1);}                                          // задержать на время 1 сек =)
   pthread_exit(arg);    
}
int main() {
    bool flag = false;                                      // флаг завершения потока
    const char* name = "/roster";                           // имя очереди сообщений
    struct mq_attr attr, old_attr;                          // парметры очереди 
    attr.mq_maxmsg = 4;                                     // максимальное число сообщений в очереди в один момент времени
    attr.mq_msgsize = 4*sizeof(str);                        // максимальный размер очереди(байты)
    pthread_t id4321;                                       // идентификатор потока
    cout <<"\x1B[38;5;45mПрограмма чтения начала работу..." <<"\x1b[0m" << endl;
    // создать (или открыть, если существует) очередь сообщений
    if ((info7 = mq_open(name, O_CREAT | O_RDONLY, 0700, &attr)) == (mqd_t)-1) {
        perror("mq_open[-]");
        exit(EXIT_FAILURE);}
    if (pthread_create(&id4321,NULL,stream, &flag) == -1) { // создать поток
        perror("pthread_create[-]");
        exit(EXIT_FAILURE);}
    cout <<"\n"<<"\x1B[38;5;45mПрограмма ждёт нажатия клавиши enter..."<<"\x1b[0m" << endl;
    getchar();                                              // ждать нажатия клавиши
    flag = true;                                            // установить флаг завершения потока
    int sum = pthread_join(id4321, NULL);                   // ожидание завершения потока
    if (sum != 0) {
        perror("pthread_join[-]");
        exit(EXIT_FAILURE);}
    if (mq_close(info7) == -1) {                            // закрыть очередь сообщений
        perror("mq_close[-]");
        exit(EXIT_FAILURE);}
    if (mq_unlink(name) == -1) {                            // удалить очередь сообщений; очередь сообщений удаляется из системы
        perror("mq_unlink[-]");}
    cout <<"\x1B[38;5;45mПрограмма чтения завершила работу."<<"\x1b[0m" << endl;
    return 0;
}

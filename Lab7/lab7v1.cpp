#include <iostream>                                         // штатная C++  ввод вывод и т.д                              
#include <stdio.h>                                          // штатная C  ввод вывод и т.д
#include <unistd.h>                                         // для функции sleep
#include <netdb.h>                                          // для gethostbyname(const char *name) - получение информации о хосте по его имени
#include <arpa/inet.h>                                      // для inet_ntop - преобразование адресов IPv4 и IPv6 из двоичной в текстовую форму
#include <mqueue.h>                                         // очереди сообщений 
using std::cout;
using std::endl;
mqd_t info7;                                                // идентификатор очереди сообщений
unsigned pro = 1;                                           // приоритет чем больше тем выше
char str[16] = {0};                                         // 16 т.к IPv4 при IPv6 32
void* stream (void* arg) {
    struct hostent* ip = NULL;                              // создание пустой структуры
    ip = gethostbyname("www.yandex.ru");
    if (NULL == ip) {                                       // проверка заполнения структуры если структура пустая == ошибка gethostbyname();
        perror("gethostbyname[-]");
        exit(EXIT_FAILURE);}
    cout << "\n"<< "--- Name:  " << ip->h_name << endl;
    while (!(* (bool*) arg)) {   
    for (int i = 0; ip->h_addr_list[i]; i++) {
            inet_ntop(AF_INET, ip->h_addr_list[i], str, sizeof(str)-1);
        if (mq_send(info7, str, sizeof(str), pro) == -1) {  // записать результат работы функции в очередь сообщений
            perror("mq_send[-]");
            exit(EXIT_FAILURE);}
            else{
                cout << "ok mq_send" << " "<< str << endl;} // вывести результат работы функции на экран
    sleep(1);}                                              // задержать на время 1 сек
    }
   pthread_exit(arg);    
}
int main() {
    bool flag = 0;                                          // флаг завершения потока
    const char* name = "/roster";                           // имя очереди сообщений
    struct mq_attr queue;                                   // парметры очереди могут быть установлены только при создании
    queue.mq_maxmsg = 4;                                    // максимальное число сообщений в очереди в один момент времени
    queue.mq_msgsize = 4*sizeof(str);                       // максимальный размер очереди(байты)
    cout <<"\x1B[0;31;40mПрограмма начала работу..." <<"\x1b[0m" << endl;
    pthread_t id4321;                                       // идентификатор потока
    // создать (или открыть, если существует) очередь сообщений;
    if ((info7 = mq_open(name, O_CREAT | O_WRONLY | O_NONBLOCK, 0700, &queue)) == (mqd_t)-1) {
            perror("mq_open[-]");
            exit(EXIT_FAILURE);}
    if (pthread_create(&id4321,NULL,stream, &flag) == -1) { // создать поток
        perror("pthread_create[-]");
        exit(EXIT_FAILURE);}
    cout <<"\n"<<"\x1B[0;31;40mПрограмма ждёт нажатия клавиши enter..."<<"\x1b[0m" << endl;
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
        perror("mq_unlink[-]");
        exit(EXIT_FAILURE);}
    cout <<"\x1B[0;31;40mПрограмма завершила работу."<<"\x1b[0m" << endl;
    return 0;
}

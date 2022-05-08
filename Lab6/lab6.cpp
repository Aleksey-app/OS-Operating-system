#include <iostream>                                         // штатная C++  ввод вывод и т.д                              
#include <unistd.h>                                         // для функции sleep и write&read и ftruncate
#include <semaphore.h>                                      // для семафора, sem_open();и флагов для неё
#include <sys/wait.h>                                       // для функции sem_open();и флагов для неё
#include <stdio.h>                                          // для функции fopen();открывает файл и связывает его с потоком; + fflush();
#include <netdb.h>                                          // для gethostbyname(const char *name) - получение информации о хосте по его имени
#include <arpa/inet.h>                                      // для inet_ntop - преобразование адресов IPv4 и IPv6 из двоичной в текстовую форму
#include <fcntl.h>                                          // для функции sem_open();и флагов для неё
#include <sys/mman.h>                                       // для функции shm_open(); и.....
#include <cstring>                                          // для функции memcpy();
using std::cout;
using std::endl;
sem_t* sem_writer;                                          // идентификатор семафора записи
sem_t* sem_read;                                            // идентификатор семафора чтения
int fd;                                                     // идентификатор разделяемой памяти
char* adr;                                                  // локальный адрес
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
            cout << str << endl;                            // вывести результат работы функции на экран
            memcpy(adr, &str, sizeof(str));                 // скопировать результат работы функции в разделяемую память

            if (sem_post(sem_writer) == -1) {               // выход из критического участка, освободить семафор записи
                perror("sem_writer[-]");
                cout << "Разблокировать семафор не удалось " << endl;
                exit(EXIT_FAILURE);}
            if (sem_trywait(sem_read) == -1) {              // входе в критический участок, ждать семафора чтения
                cout << "Ожидание освобождения ресурса" << endl;
                perror("sem_read[-]");}
    sleep(1);}
    }
   pthread_exit(arg);    
}
int main() {
    cout <<"\x1B[0;31;40mПрограмма начала работу..." <<"\x1b[0m" << endl;
    const char* w = "S_Writer";                                 // имя семафора записи
    const char* s = "S_Read";                               // имя семафора чтения
    const char* my = "/my_memory";                          // имя участка разделяемой памяти
    bool flag = 0;                                           // флаг завершения потока
    pthread_t id4321;                                           // идентификатор потока
// создать (или открыть, если существует) разделяемую память
    fd = shm_open("/my_memory", O_CREAT|O_RDWR, 0777);
    if (ftruncate(fd, sizeof(str)) == -1) {                 // обрезать разделяемую память до требуемого размера
        perror("ftruncate[-]");
        exit(EXIT_FAILURE);}
// отобразить разделяемую память на локальный адрес
    adr = (char*)mmap(NULL, sizeof(str), PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0);
    sem_writer = sem_open(w, O_CREAT, 0777, 0);             // создать (или открыть, если существует) семафор записи
    sem_read = sem_open(s, O_CREAT, 0777, 0);            // создать (или открыть, если существует) семафор чтения
    // создать поток
    if (pthread_create(&id4321,NULL,stream, &flag) == -1) {
        perror("pthread_create[-]");
        exit(EXIT_FAILURE);}
    cout <<"\n"<<"\x1B[0;31;40mПрограмма ждёт нажатия клавиши enter..."<<"\x1b[0m" << endl;
    getchar();                                              // ждать нажатия клавиши
    flag = 1;                                               // установить флаг завершения потока
    pthread_join(id4321, NULL);                             // ожидание завершения потока
    sem_close(sem_read);                                    // закрыть семафор чтения
    sem_unlink(s);                                          // удалить именованный семафор чтения 
    sem_close(sem_writer);                                  // закрыть семафор записи
    sem_unlink(w);                                          // удалить именованный семафор записи
    munmap(adr, sizeof(char*));                             // удаление отображения разделяемой памяти на локальный адрес
    close(fd);                                              // закрытие участка разделяемой памяти
    shm_unlink(my);                                         // удалить разделяемую память
    cout <<"\x1B[0;31;40mПрограмма завершила работу."<<"\x1b[0m" << endl;
return 0;    
}

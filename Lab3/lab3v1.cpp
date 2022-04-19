#include<iostream>                                          // штатная C++  ввод вывод и т.д                              
#include<unistd.h>                                          // для функции sleep и write&read
#include <netdb.h>                                          // для gethostbyname(const char *name) - получение информации о хосте по его имени; Функция 10.
#include <arpa/inet.h>                                      // для inet_ntop - преобразование адресов IPv4 и IPv6 из двоичной в текстовую форму 
#include <fcntl.h>                                          // для 0_NONBLOCK и ему подобных флагов
using std::string;
using std::cout;
using std::endl;
struct stream{
    bool flag;  //флаг выполнения цикла
    int num;    //вывод цифра
    string txt; //вывод строка
};
int request[2];                                             // создание переменной массива фа-й дескриптора 
void* writer(void* arg) {                                   // поток записи
    stream *f1 = (stream*) arg;
    cout << f1->txt;
    struct hostent* ip = NULL;                              // создание пустой структуры
    ip = gethostbyname("www.yandex.ru");
    if (NULL == ip) {                                       // проверка заполнения структуры
        perror("Struct[-]");
    }
    sleep(1);
    char str[16] = {0};                                     // 16 т.к IPv4 при IPv6 32                                  
    if (f1->flag == true){
        cout << "\n"<< "--- Name:  " << ip->h_name << endl;}
    while (f1->flag)
    { 
       for (int i = 0; ip->h_addr_list[i]; i++) {
            inet_ntop(AF_INET, ip->h_addr_list[i], str, sizeof(str)-1);
      if (write(request[f1->num], &str, sizeof(str)) != -1){
            cout <<"write ip: " << endl;} 
          else{
              perror("StWrite[-]");}
         sleep(1);}
    }
    cout <<"\n"<<f1->txt << endl;
    f1->txt = "Поток записи закончил работу";
    pthread_exit(arg);
}
void* print(void* arg) {                                    // поток чтения(печати).
    stream *f1 = (stream*) arg;
    cout << f1->txt;
    sleep(1);
    char buf[16];
    while (f1->flag)
    { 
       if (read(request[f1->num], &buf, 16) != -1) {
            cout << "read ip address: " << buf << endl;
        } else {
            perror("StRaed[-]");
        }
            sleep(1);
    }
    f1->txt = "Поток чтения закончил работу";
    pthread_exit(arg);
}
int main() {
    cout <<"\x1B[0;31;40mПрограмма начала работу..." <<  endl; 
    if (pipe(request) != -1);                               // создание блокируемого неименовонного канала; Вариант 3, 1.
        else{
            perror("pipe[-]");}
    /*if (pipe2(request, O_NONBLOCK) != -1);                 // создание неблокируемого неименованного канала;
        else{perror("pipe2[-]");} */
    int error=0, error2=0; 
    pthread_t id4321;  
    stream arg1 = {true, 1, "Поток записи начал работу..."};
    error=pthread_create(&id4321,NULL,writer,(void*) &arg1);
    if (error){
        perror("CreWriter[-]");}
    pthread_t id4322; 
    stream arg2 = {true, 0, "Поток чтения начал работу..."};
    error2=pthread_create(&id4322,NULL,print,(void*) &arg2);
    if (error2){
        perror("CrePrint[-]");}
    sleep(1);
    cout <<"\n"<<"\x1B[0;31;40mПрограмма ждёт нажатия клавиши enter..."<<"\x1b[0m" << endl;
    void *status;
    getchar();
    arg1.flag = false;
    arg1.txt ="Клавиша нажата.";
    pthread_join(id4321,&status);
    close(request[arg1.num]);
    stream *ex = (stream*) status;
    cout <<endl << ex->txt << endl;
    close(request[arg2.num]);
    arg2.flag = false;
    pthread_join(id4322,&status);
    ex = (stream*) status;
    cout << endl << ex->txt << endl;
    cout <<"\x1B[0;31;40mПрограмма завершила работу."<<"\x1b[0m" << endl;
    return 0;
}

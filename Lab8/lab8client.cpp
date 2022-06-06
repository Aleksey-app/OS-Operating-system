#include <iostream>                                         // штатная C++  ввод вывод и т.д 
#include <stdio.h>                                          // штатная C  ввод вывод и т.д
#include <unistd.h>                                         // для функции sleep
#include <vector>                                           // вектор динамический массив
#include <sys/socket.h>                                     // для socket(); etc
#include <thread>                                           // для работы с потоками =) так проще 
#include <fcntl.h>                                          // для работы fchtl и флагов
#include <arpa/inet.h>                                      // для struct sockaddr_in
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::thread;
struct sockaddr_in server_addr;
socklen_t a = sizeof(server_addr);
struct client {
    bool flag = true;                                       // флаг выполнения цикла
    int SerSocket;                                          // cокет для работы с сервером
};
void SendF(client& elim) {                                  // функция передачи запросов
    int num = 0;
    string line = "Запрос: ";
     while (elim.flag) {
         vector<char>sendO(line.begin(), line.end());
         for (auto& xz: std::to_string(num)) {              // функции конвертации C++11
             sendO.push_back(xz);}                          // добавил номер запроса
     // передать запрос в сокет
     int q = sendto(elim.SerSocket, &sendO[0], sendO.size(), MSG_CONFIRM, (struct sockaddr*)&server_addr, a);
      if (q == -1) {
           perror("send[-]");
      }else{
          /*cout << "Отправлено " << q << " байт" << endl;*/}
        ++num;
        sleep(1);
     }
}
void ReceivingReplies(client& elim) {                       // функция приема ответов
    while (elim.flag) {
       const int vec = 68;
       vector<char>buf(vec);
       int q = recv(elim.SerSocket, &buf[0], vec, 0);
       if (q == -1) {
           perror("recv[-]");
           sleep(1);
       }
       else if (q == 0) {
           cout << "Cоединение было разорвано" << endl;
           sleep(1);}
           else{
               /*cout << "Принято " << q << " байт" << endl;*/
               for (auto& buf2 : buf) {
                   cout << buf2;}
                   cout << endl;
                   sleep(1);}
    }
}
void connection(client& elim) {                             // функция установления соединения
    while (elim.flag) {
        int q = connect(elim.SerSocket, (struct sockaddr*) &server_addr, sizeof(server_addr));
        if (q == -1) {
            perror("connect[-]");
            sleep(3);
        }else{
            cout << "Установлено соединение с сервером" << endl;
            thread Se(SendF, std::ref(elim));
            thread RR(ReceivingReplies, std::ref(elim));
            Se.join();
            RR.join();}
    }
}
int main() {
    client elim;
    cout << "Приложение начало работать" << endl;
    elim.SerSocket = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(elim.SerSocket, F_SETFL, O_NONBLOCK);             // для снятия блокировки работы с сокетами
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1234);
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);   // адрес интерфейса внутренней петли
    thread Co(connection, std::ref(elim));
    cout <<"\n"<<"Для завершения работы приложение ждёт нажатия клавиши enter..." << endl;
    getchar();
    elim.flag = false;
    cout <<"\n"<<"Клавиша enter нажата."  << endl;
    Co.join();
    close(elim.SerSocket);
    cout <<"\n" << "Приложение закончило работу" << endl;
return 0;
}

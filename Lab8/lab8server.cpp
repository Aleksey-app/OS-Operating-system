#include <iostream>                                         // штатная C++  ввод вывод и т.д 
#include <stdio.h>                                          // штатная C  ввод вывод и т.д
#include <unistd.h>                                         // для функции sleep
#include <mutex>                                            // для МЬЮТЕКСОВ
#include <vector>                                           // вектор динамический массив
#include <queue>                                            // очередь FIFO
#include <netdb.h>                                          // для gethostbyname(const char *name) - получение информации о хосте по его имени
#include <arpa/inet.h>                                      // для inet_ntop - преобразование адресов IPv4 и IPv6 из двоичной в текстовую форму
#include <sys/socket.h>                                     // для socket(); etc
#include <thread>                                           // для работы с потоками =) так проще 
#include <fcntl.h>                                          // для работы fchtl и флагов
using std::cout;
using std::endl;
using std::queue;
using std::string;
using std::vector;
using std::mutex;
using std::thread;
using std::ref;
struct sockaddr addr;                                       // записывается адрес сокета клиента, который установил соединение с сервером.
socklen_t a = sizeof(addr);                                 // размер структуры
struct arg {
    bool flag = true;                                       // флаг выполнения цикла
    int hearing;                                            // идентификатор «слушающего» сокета
    int customer;                                           // идентификатор сокета для работы с клиентом
    mutex traffic;                                          // идентификатор мьютекса
    queue <string> query;                                   // идентификатор очереди запросов на обработку
};
void SerReceivingRequests(arg& elim) {                      // функция приема запросов
    const int vec = 16;
    while (elim.flag) {
        vector<char>str(vec);
      string one;
      int q = recv(elim.customer, &str[0], vec, 0);         // принять запрос из сокета
      if (q == -1) {
           perror("recv[-]");
           sleep(1);
      }
      else if (q == 0) {
          cout << "Связь потерна..." << endl;
          sleep(1);
      }
      else{
          one.append(str.begin(), str.end());
            elim.traffic.lock();                            // вход в критический участок
             cout << one;                                   // вывести результат на экран
             elim.query.push(one);                          // положить запрос в очередь на обработку
            elim.traffic.unlock();                          // выход из критического участка
            sleep(1);
        }
    }
}
void SerProcessingReply(arg& elim) {                        // функция обработки запросов и передачи ответов
    while (elim.flag) {
        elim.traffic.lock();                                // входе в критический участок
        if(!elim.query.empty()) {                           // если очередь не пуста
            string message = elim.query.front();            // извлёк 1 элемент
            elim.query.pop();                               // удалил элемент из начала очереди
            elim.traffic.unlock();                          // выход из критического участка
            char str[16];
            struct hostent* ip = NULL;
            ip = gethostbyname("www.yandex.ru");            // выполняем функцию, которую требует задание
            for (int i = 0; ip->h_addr_list[i]; i++) {      // функция возвращает структуру из нескольких полей
            inet_ntop(AF_INET, ip->h_addr_list[i], str, sizeof(str)-1);
            string line = string(str) + " ";
            vector<char> buf (line.begin(), line.end());
            socklen_t sizecliAddr=sizeof(struct sockaddr_in);
            int sendcount = sendto(elim.customer, &buf[0], buf.size(),MSG_CONFIRM,(struct sockaddr*)&addr, a);
            if (sendcount == -1) {
                perror("sendto error[-]");} 
            else{
                cout << " OK" << " ";}
            }
            cout << endl;
        }
        else{
            elim.traffic.unlock();
            sleep(1);}
    }
}
void SerConnection(arg& elim) {                             // ожидания соединений
    while (elim.flag) {                                     // идентификатор потока приема запросов
        // прием соединения от клиента
        elim.customer = accept(elim.hearing, &addr, &a);
        /*cout << addr.sa_family << "Семейство адресов, AF_xxx " << endl;//тестовая штука
        for (int i = 0; i < 15; i++) {
           cout << addr.sa_data[i]; 
        }
        cout << " 14 байтов для хранения адреса" << endl;*/
        if (elim.customer == -1) {
            perror("accept[-]");
            sleep(3);
        }else{
            cout << "\nСоединение установлено! " << endl;
        // создать поток приема запросов
        thread Requests(SerReceivingRequests, ref(elim));
        // создать поток обработки запросов и передачи ответов
        thread Reply(SerProcessingReply, ref(elim));
        Requests.join();                                    // приостанавливает работу потока приема запросов
        Reply.join();                                       // приостанавливает работу потока обработки запросов и передачи ответов
        }
        shutdown(elim.customer,2);                          // запретить передачу данных
        close(elim.customer);                               // чтобы освободить связанные с ним системные ресурсы
    }
}
int main() {
    cout <<"\x1B[0;31;40mСервер начала работу..." <<"\x1b[0m" << endl;
    arg elim;
    elim.hearing = socket(AF_INET, SOCK_STREAM, 0);         // создал «слушающий» сокет
    fcntl(elim.hearing, F_SETFL, O_NONBLOCK);               // для снятия блокировки работы с сокетами
    struct sockaddr_in ipConnection;                        // переменная структуры с адресом
    ipConnection.sin_family = AF_INET;                      // cемейство адресов
    ipConnection.sin_port = htons(1234);                    // в данный момент номер порта
    ipConnection.sin_addr.s_addr = htonl(INADDR_ANY);       // соединяться с клиентами через любой интерфейс(в структуре sockaddr_in стркутура in_addr и там поле)
    // привязать «слушающий» сокет к адресу
    bind(elim.hearing, (struct sockaddr*) &ipConnection, sizeof(ipConnection));
    int debug = 1;
    // для отладки пример вызова
    setsockopt(elim.hearing, SOL_SOCKET, SO_REUSEADDR, &debug, sizeof(debug));
    listen(elim.hearing, 5);                                // перевести сокет в состояние прослушивания
    thread Connection(SerConnection, ref(elim));            // создать поток ожидания соединений
    cout <<"\n"<<"\x1B[0;31;40mДля завершения работы сервер ждёт нажатия клавиши enter..."<<"\x1b[0m" << endl;
    getchar();
    elim.flag = false;
    cout <<"\n"<<"\x1B[0;31;40mКлавиша enter нажата. " << "\x1b[0m" << endl;
    Connection.join();                                      // ждать завершения потока ожидания соединений
    sleep(1);
    close(elim.hearing);
    close(elim.customer);
    cout <<"\n"<<"\x1B[0;31;40mРабота сервера остановлена. " << "\x1b[0m" << endl;
    return 0;
}

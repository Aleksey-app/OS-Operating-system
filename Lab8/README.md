# Практическая работа №8
**Знакомство с механизмом взаимодействия удаленных процессов – сокетами и с системными вызовами, обеспечивающими установление соединения, разъединение, а также передачу и прием данных.**
>Написать комплект из двух программ, одна из которых выполняет функции сервера, а вторая выполняет функции клиента.
Клиент, после установления соединения с сервером, посылает серверу запросы с периодом 1 сек.
Сервер принимает запросы, обрабатывает их и отправляет ответы клиенту. Под обработкой будем понимать выполнение функции, ранее выбранной для других работ. Клиент принимает ответы и выводит их на экран.


С каждым сокет связываются три атрибута: домен, тип и протокол. Эти атрибуты задаются при создании сокета и остаются неизменными на протяжении всего времени его существования. Для создания сокета используется функция socket, имеющая следующий прототип.
```c
int socket(int domain, int type, int protocol);
```
- Где: domain - определяет тип коммуникационного протокола;
    |Тип|Описание|
    |:--|:------:|
    |AF_UNIX|Локальные сокеты для передачи данных используется файловая система ввода/вывода Unix|
    |AF_INET|Cокеты протокола интернет. Сокеты, размещённые в этом домене, могут использоваться для работы в любой IP-сети|
    |AF_IPX|Для протоколов Novell|
    |AF_INET6|Cокеты протокола интернет. IPv6|
- type –	определяет тип передачи;
    |Флаг|Описание|
    |:---|:------:|
    |SOCK_STREAM|Передача потока данных с предварительной установкой соединения. Обеспечивается надёжный канал передачи данных, при котором фрагменты отправленного блока не теряются, не переупорядочиваются и не дублируются|
    |SOCK_DGRAM|Передача данных в виде отдельных сообщений (датаграмм). Предварительная установка соединения не требуется. Обмен данными происходит быстрее, но является ненадёжным: сообщения могут теряться в пути, дублироваться и переупорядочиваться|
    |SOCK_RAW|Этот тип присваивается низкоуровневым (т. н. "сырым") сокетам. Их отличие от обычных сокетов состоит в том, что с их помощью программа может взять на себя формирование некоторых заголовков, добавляемых к сообщению|
- protocol - определяет протокол, используемый для передачи данных. Можно передать 0, что соответствует протоколу по умолчанию. 

**Для явного связывания сокета с некоторым адресом используется функция**
```c
int bind(int sockfd, struct sockaddr *addr, int addrlen);
```
- Где: sockfd - дескриптор сокета;
- addr - содержит указатель на структуру с адресом;
```c
struct sockaddr {
    unsigned short    sa_family;    // Семейство адресов, AF_xxx
    char              sa_data[14];  // 14 байтов для хранения адреса
};
```
Поле sa_family содержит идентификатор домена, тот же, что и первый параметр функции socket. В зависимости от значения этого поля по-разному интерпретируется содержимое массива sa_data. Разумеется, работать с этим массивом напрямую не очень удобно, поэтому вы можете использовать вместо sockaddr одну из альтернативных структур вида sockaddr_XX (XX - суффикс, обозначающий домен: "un" - Unix, "in" - Internet и т. д.). 
```c
struct sockaddr_in {
    short int          sin_family;  // Семейство адресов
    unsigned short int sin_port;    // Номер порта
    struct in_addr     sin_addr;    // IP-адрес
    unsigned char      sin_zero[8]; // "Дополнение" до размера структуры sockaddr
};
```
Здесь поле sin_family соответствует полю sa_family в sockaddr, в sin_port записывается номер порта, а в sin_addr - IP-адрес хоста. Поле sin_addr само является структурой, которая имеет вид:
```c
struct in_addr {
    unsigned long s_addr;
};
```
  &#9989;При указании IP-адреса и номера порта необходимо преобразовать число из порядка хоста в сетевой. Для этого используются функции ```htons (Host TO Network Short)``` и ```htonl (Host TO Network Long)```. Обратное преобразование выполняют функции ```ntohs``` и ```ntohl```
  
 &#9989;При отладке, возможно, придется завершать и снова запускать программу. Если сокет «привязан», то повторный вызов ```bind();``` может быть выполнен через довольно большой таймаут. Чтобы не ждать, надо придать сокету свойство SO_REUSEADDR вызовом 
```c
setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
```
- Где: sockfd - дескриптор сокета;
- int optval = 1 аргумент должен быть отличен от нуля, чтобы включить логическую опцию

**Установка соединения (сервер)**

Установка соединения на стороне сервера состоит из четырёх этапов, ни один из которых не может быть опущен. Сначала сокет создаётся и привязывается к локальному адресу. Если компьютер имеет несколько сетевых интерфейсов с различными IP-адресами, вы можете принимать соединения только с одного из них, передав его адрес функции ```bind(); ```
Что касается номера порта, вы можете задать конкретный номер или 0 (в этом случае система сама выберет произвольный неиспользуемый в данный момент номер порта).На следующем шаге создаётся очередь запросов на соединение. При этом сокет переводится в режим ожидания запросов со стороны клиентов. Всё это выполняет функция
```c
int listen(int sockfd, int backlog);
```
- Где: sockfd - дескриптор сокета;
- backlog - размер очереди запросов; **константа SOMAXCONN (128 в Linux 2.0 & 2.2)**
BSD и некоторые её потомки ограничивают размер очереди до 5.
Каждый раз, когда очередной клиент пытается соединиться с сервером, его запрос ставится в очередь, так как сервер может быть занят обработкой других запросов. Если очередь заполнена, все последующие запросы будут игнорироваться. Когда сервер готов обслужить очередной запрос, он использует функцию 
```c
int accept(int sockfd, void *addr, int *addrlen);
```
- Где: sockfd - Cлушающий сокет, после вызова он остаётся в слушающем состоянии и может принимать другие соединения;
- addr - записывается адрес сокета клиента, который установил соединение с сервером;
- addrlen - изначально записывается размер структуры; функция accept записывает туда длину, которая реально была использована. Если вас не интересует адрес клиента, вы можете просто передать NULL в качестве второго и третьего параметров.

 &#9989;Полученный от ```accept();``` новый сокет связан с тем же самым адресом, что и слушающий сокет. Адрес TCP-сокета не обязан быть уникальным в Internet-домене. Уникальными должны быть только соединения, для идентификации которых используются два адреса сокетов, между которыми происходит обмен данными.

**Установка соединения (клиент)**

На стороне клиента для установления соединения используется функция
```c
int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);
```
- Где: sockfd - сокет, который будет использоваться для обмена данными с сервером;
- serv_addr - содержит указатель на структуру с адресом сервера;
- addrlen - длину этой структуры;
Обычно сокет не требуется предварительно привязывать к локальному адресу, так как функция ```connect();``` сделает это за вас, подобрав подходящий свободный порт. Вы можете принудительно назначить клиентскому сокету некоторый номер порта, используя ```bind();``` перед вызовом ```connect();``` Делать это следует в случае, когда сервер соединяется только с клиентами, использующими определённый порт. В остальных случаях проще и надёжнее предоставить системе выбрать порт за вас.

**Отправка данных**
```c
send(int  sockfd , const void * buf , size_t  len , int  flags );
```
- Где: sockfd - как всегда, дескриптор сокета, через который мы отправляем данные;
- buf - указатель на буфер с данными;
- len - длина буфера в байтах;
- flags - набор битовых флагов, управляющих работой функции (если флаги не используются, передайте функции 0).
    |Флаг|Описание|
    |:---|:------:|
    |MSG_DONTROUTE|Запрещает маршрутизацию пакетов. Нижележащие транспортные слои могут проигнорировать этот флаг|
    |MSG_CONFIRM|Сообщает (на уровне связи), что процесс пересылки произошел: вы получаете успешный ответ с другой стороны. Если уровень связи не получает его, он регулярно перепроверяет сеть (например посредством однонаправленной передачи ARP). Это работает с сокетами SOCK_DGRAM и SOCK_RAW|
    |MSG_OOB|Используется для отправки,приёма срочных данных, если сокет это поддерживает (как, например, сокеты типа SOCK_STREAM); протокол более низкого уровня также должен поддерживать внепотоковые данные|
    
 &#9989;Функция ```send();``` возвращает число байтов, которое на самом деле было отправлено (или -1 в случае ошибки).
 
**Обмен датаграммами (отправка данных)**
```c
int sendto(int sockfd, const void *buf, int len, unsigned int flags, const struct sockaddr *to, int tolen);
int recvfrom(int sockfd, void *buf, int len, unsigned int flags, struct sockaddr *from, int *fromlen);
```
- Функция ```sendto();``` очень похожа на ```send();``` Два дополнительных параметра to и tolen используются для указания адреса получателя. Для задания адреса используется структура sockaddr.
- Функция ```recvfrom();``` работает аналогично ```recv();``` Получив очередное сообщение, она записывает его адрес в структуру, на которую ссылается from, а записанное количество байт - в переменную, адресуемую указателем fromlen.

&#9989; Датаграммы используются в программах довольно редко. В большинстве случаев надёжность передачи критична для приложения, и вместо изобретения собственного надёжного протокола поверх UDP программисты предпочитают использовать TCP. Тем не менее, иногда датаграммы оказываются полезны. Например, их удобно использовать при транслировании звука или видео по сети в реальном времени, особенно при широковещательном транслировании.

**Чтения данных**
```c
int recv(int sockfd, void *buf, int len, int flags);
```
- Где: sockfd - как всегда, дескриптор сокета, через который мы принимаем данные;
- buf - указатель на буфер с данными;
- len - длина буфера в байтах;
- flags - набор битовых флагов, **MSG_PEEK** позволяет "подсмотреть" данные, полученные от удалённого хоста, не удаляя их из системного буфера (это означает, что при следующем обращении к ```recv();``` вы получите те же самые данные).

&#9989;Существует особый случай, при котором recv возвращает 0. Это означает, что соединение было разорвано.

**Закрытие сокета**
```c
#include <unistd.h>
int close(int fd);
```
- Где: fd - как всегда, дескриптор сокета;

Закончив обмен данными, закройте сокет с помощью функции close. Это приведёт к разрыву соединения.

Вы также можете запретить передачу данных в каком-то одном направлении
```c
int shutdown(int sockfd, int how);
```
- Где: how - может принимать одно из следующих значений;
- 0 - запретить чтение из сокета;
- 1 - запретить запись в сокет;
- 2 - запретить и то и другое.

Хотя после вызова shutdown с параметром how, равным 2, вы больше не сможете использовать сокет для обмена данными, вам всё равно потребуется вызвать close, чтобы освободить связанные с ним системные ресурсы.

**Пример работы программы функция**
```c
struct hostent *gethostbyname(const char *name);
```

**Сервер**

![server](https://user-images.githubusercontent.com/71906604/172451624-2afa36c2-3918-496d-96e4-253e25a174aa.png)

**Клиент**

![client](https://user-images.githubusercontent.com/71906604/172452171-99f702d4-1741-4065-a822-39ae9a4c2ad5.png)

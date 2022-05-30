# Практическая работа №7
**Знакомство с механизмом взаимодействия процессов – очередями сообщений и с системными вызовами, обеспечивающими создание, закрытие, удаление очередей сообщений, а также передачу и прием сообщений.**
>Написать комплект из двух программ, одна из которых передает сообщение в очередь сообщений (стандарта очереди POSIX), а вторая – принимает сообщения из очереди сообщений. Проверить работу функций с блокировкой и без блокировки.
>Функция, которая выполняется на передающей стороне
```c 
struct hostent *gethostbyname(const char *name); 
```
   **Существует несколько видов программных интерфейсов для создания очередей сообщений.
 Рассмотрим программный интерфейс POSIX.**
 
 ***Функции для работы с очередью сообщений POSIX***
 
   |Метод|Описание|
   |:---|:------:|
   |mq_open|Открытие/создание очереди сообщений|
   |mq_getattr|Получение атрибутов очереди сообщений|
   |mq_setattr|Установка атрибутов очереди сообщений|
   |mq_send|Отправка сообщения в очередь|
   |mq_timedsend|Отправка сообщения в очередь. Блокируется в течение заданного времени|
   |mq_receive|Приём сообщения из очереди|
   |mq_timedreceive|Приём сообщения из очереди. Блокируется в течение заданного времени|
   |mq_close|Закрытие очереди сообщений|
   |mq_unlink|Удаление очереди сообщений|
   |mq_notify|Регистрация для получения уведомления всякий раз, когда получено сообщение в пустой очереди сообщений|

   **Очередь сообщений создается следующим вызовом:**
```c
mqd_t mq_open(const char *name, int oflag, mode_t mode, struct mq_attr *attr);
```
- Где: name – имя очереди сообщений;
- oflag – флаг;
    |Флаг|Описание|
    |:---|:------:|
    |O_RDONLY|Открыть очередь только для получения сообщений|
    |O_WRONLY|Открыть очередь только для отправки сообщений|
    |O_RDWR|Открыть очередь для отправки и получения сообщений|
    |O_NONBLOCK|Открыть очередь в неблокирующем режиме|
    |O_CREAT|Открыть или создать если не существует|
- mode – права доступа к очереди сообщений;
- attr – параметры очереди сообщений.
Структура mq_attr хранит в себе эти четыре атрибута:
```c
struct mq_attr {
 long mq_flags;      // флаг очереди: 0, O_NONBLOCK 
 long mq_maxmsg;    // максимальное количество сообщений в очереди
 long mq_msgsize;   // максимальный размер сообщения (в байтах)
 long mq_curmsgs;   // текущее количество сообщений в очереди
};
```
Указатель на такую структуру может быть передан в качестве четвертого аргумента ```mq_open```, 
что дает возможность установить параметры ```mq_maxmsg``` и ```mq_msgsize``` в момент создания очереди. 
Другие два поля структуры функцией ```mq_open``` игнорируются.

**Получение атрибутов очереди сообщений (РЕАЛЬНОЕ ВРЕМЯ)**
```c
mq_getattr(mqd_t mqdes, struct mq_attr *mqstat); 
```
- Где: mqdes - идентификатор очереди сообщений;
- mqstat - возвращает структуру struct mq_attr *mqstat;

**Установка атрибутов очереди сообщений (РЕАЛЬНОЕ ВРЕМЯ)**
```c
mq_setattr(mqd_t mqdes, const struct mq_attr *restrict mqstat, struct mq_attr *restrict omqstat);
```
- Где: mqdes - идентификатор очереди сообщений;
- mqstat - новые атрибуты очереди, но фактически используется только поле mqflags той структуры на которую указывает mqstat, 
что дает возможность сбрасывать или устанавливать флаг запрета блокировки. Другие три поля структуры игнорируются: 
максимальное количество сообщений в очереди и максимальный размер сообщения могут быть установлены только в момент создания очереди, 
а количество сообщений в очереди можно только считать, но не изменить.
- omqstat -  если omqstat не равен NULL, функция возвращает предыдущие значения атрибутов очереди (mq_flags, mq_maxmsg, mq_msgsize) и текущий статус очереди (mq_curmsgs).

**Отправка сообщения в очередь**
```c
mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned msg_prio);
```
- Где: mqdes –	идентификатор очереди сообщений;
- msg_ptr –	указатель на сообщение;
- msg_len –	длина сообщения;
- msg_prio –	приоритет сообщения, чем больше, число, тем выше приоритет. Сообщение с высоким приоритетом вставляется перед сообщением с низким приоритетом. Для
Сообщений с одинаковым приоритетом работает принцип первый вошёл, первый вышел.FIFO

**Отправка сообщения в очередь. Блокируется в течение заданного времени**
```c
mq_timedsend(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned msg_prio, const struct timespec *abs_timeout);
```
- Где: mqdes –	идентификатор очереди сообщений;
- msg_ptr –	указатель на сообщение;
- msg_len –	длина сообщения;
- msg_prio –	приоритет сообщения...;
- abs_timeout - время задается способом, аналогичным с мьютексами и неименованными семафорами.

**Приём сообщения из очереди**
```c
mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned *msg_prio);
```
- Где: mqdes –	идентификатор очереди сообщений;
- msg_ptr –	указатель на буфер для приема сообщения;
- msg_len –	размер буфера;
- msg_prio –	приоритет сообщения;

**Закрытие очереди сообщений**
```c
mq_close(mqd_t mqdes);
```

**Удаление очереди сообщений**
```c
mq_unlink(const char *name);
```

**Регистрация для получения уведомления всякий раз, когда получено сообщение в пустой очереди сообщений**
```c
mq_notify(mqd_t mqdes , const struct sigevent * sevp );
```
- Где: mqdes –	идентификатор очереди сообщений;
- sevp - если не равен NULL, то регистрирует вызывающий процесс для получения уведомления о сообщении.

sigevent — структура для уведомления от асинхронных процедур
Структура sigevent используется в различных программных интерфейсах для описания способа, которым нужно уведомлять процесс о событии (например, окончание асинхронного запроса, истечение таймера или поступление сообщения).

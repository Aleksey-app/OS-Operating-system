***СПбГЭТУ «ЛЭТИ»
Факультет компьютерных технологий и информатики (ФКТИ) 2022***
# Практическая работа №1
**Знакомство с базовой структурой многопоточной программы и с системными вызовами, обеспечивающими создание и завершение потоков.**
# Практическая работа №2
**Знакомство со средствами синхронизации потоков – неименованными семафорами, а также с системными вызовами, обеспечивающими создание, закрытие, захват и освобождение неименованных семафоров.**
# Практическая работа №3
**Знакомство со средством взаимодействия потоков и процессов – неименованными каналами и с системными вызовами, обеспечивающими создание и закрытие неименованных каналов, а также передачу и прием данных через неименованные каналы.**
# Практическая работа №4
**Знакомство с основными системными вызовами, обеспечивающими создание процессов.**
# Практическая работа №5
**Знакомство со средством синхронизации процессов - именованными семафорами и с системными вызовами, обеспечивающими создание, открытие, закрытие и удаление именованных семафоров, а также захват и освобождение именованных семафоров.**
# Практическая работа №6
**Знакомство с механизмом обмена данными между процессами – разделяемой памятью и с системными вызовами, обеспечивающими создание разделяемой памяти, отображения ее на локальную память, передачу данных, чтение данных, закрытие и удаление разделяемой памяти.**
>Написать программу, содержащую две программы и запускать их в двух терминалах. Комплект из двух программ, одна из которых посылает данные в разделяемую память, а вторая – читает эти данные. Поскольку механизм разделяемой памяти не содержит средств синхронизации записи и чтения, для синхронизации требуется применить механизм именованных семафоров. В программах необходимо использовать только функции ```sem_wait()```

   **Выбор стандарта POSIX**
   
   **В стандарте POSIX участок разделяемой памяти создается:**

```int shm_open(const char *name, int oflag, mode_t mode)``` 
- Где: name – имя участка разделяемой памяти;
- oflag –	флаги, определяющие тип создаваемого участка разделяемой памяти;

  |Флаг|Описание|
  |:---|:------:|
  |O_RDWR|Открывает объект для чтения и записи|
  |O_CREAT|Создает объект разделяемой памяти, если он еще не существует. Открывается, если есть|
  |O_EXCL|Если также было указано O_CREAT и объект разделяемой памяти с заданным name уже существует, то возвращается ошибка|
  |O_RDONLY|Открыть объект для чтения. Объект разделяемой памяти, открытый таким образом, может быть обработан mmap() только для чтения (PROT_READ)|
  |O_TRUNC|Если объект разделяемой памяти уже существует, то обрезать его до 0 байтов|
- mode – права доступа к участку разделяемой памяти.

    **Установка размера участка разделяемой памяти:**
    
```int ftruncate(int fd, off_t length)```
- Где: fd -	дескриптор разделяемой памяти, полученный как результат вызова функции ```shm_open();```
- length – требуемый размер разделяемой памяти.

    **Отображение разделяемой памяти на локальный адрес создается вызовом:**
    
```void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)```
- Где: addr –	начальный адрес отображения;
- length – размер отображения;
- prot – параметр, определяющий права чтения/записи отображения;

    **Атрибуты защиты**
    |Параметр|Описание|
    |:-------|:------:|
    |PROT_EXEC|Содержимое памяти может быть исполнено|
    |PROT_READ|Доступ на чтение|
    |PROT_WRITE|Доступ на запись|
    |PROT_NONE|Доступ запрещен|
- flags –	параметр, определяющий правила видимости отображения процессами;

    **Флаги,определяют способ отображения памяти. Эти флаги разбиваются на две части. Первая часть означает тип флага**
    |Тип отображения памяти|Описание|
    |:---------------------|:------:|
    |MAP_SHARED|Отображение совместно используется вызывающими процессами|
    |MAP_PRIVATE|Отображение используется только вызывающим процессом. Этот тип выделяет системную память и создает копию объекта|
    |MAP_ANONYMOUS|Аналогичен типу MAP_PRIVATE, за исключением того, что параметр fd не используется (должен быть установлен в значение NOFD), а выделяемая память заполняется нулями|
    
    **Вторая часть модификатор типа отображения памяти будут ли изменения отображения видимы другим процессам, отображающим ту же область, и будут ли изменения перенесены в отображённый файл.**

    |Модификатор типа отображения памяти|Описание|
    |:----------------------------------|:------:|
    |MAP_SHARED|Сделать отображение общим. Изменения отображения видимы всем процессам, отображающим ту же область и (если отображение выполняется из файла) изменения заносятся в отображённый файл|
    |MAP_PRIVATE|Создать закрытое отображение с механизмом копирования при записи. Изменения отображения невидимы другим процессам, отображающим тот же файл, и сам файл не изменяется|
    
- fd – дескриптор разделяемой памяти;
- offset – смещение на участке разделяемой памяти относительно начального адреса.

    **Удаление отображения разделяемой памяти на локальный адрес:**
    
```int munmap(void *addr, size_t length)```
- Где: addr –	локальный адрес отображения;
- length - размер отображения.

    **Закрытие участка разделяемой памяти:**

```int close(int fd)```
- Где: fd –	дескриптор разделяемой памяти.

    **Удаление участка разделяемой памяти:**

```int shm_unlink(const char *name)```
- Где: name –	имя участка разделяемой памяти.

Функция, которая выполняется на передающей стороне
```c
struct hostent *gethostbyname(const char *name)
```
# Практическая работа №7
**Знакомство с механизмом взаимодействия процессов – очередями сообщений и с системными вызовами, обеспечивающими создание, закрытие, удаление очередей сообщений, а также передачу и прием сообщений.**
>Написать комплект из двух программ, одна из которых передает сообщение в очередь сообщений (стандарта очереди POSIX), а вторая – принимает сообщения из очереди сообщений. Проверить работу функций с блокировкой и без блокировки.
>Функция, которая выполняется на передающей стороне
```c 
struct hostent *gethostbyname(const char *name) 
```
   **Существует несколько видов программных интерфейсов для создания очередей сообщений.
 Рассмотрим программный интерфейс POSIX.**

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
# Практическая работа №8
**Знакомство с механизмом взаимодействия удаленных процессов – сокетами и с системными вызовами, обеспечивающими установление соединения, разъединение, а также передачу и прием данных.**

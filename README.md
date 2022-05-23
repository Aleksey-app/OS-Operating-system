***СПбГЭТУ «ЛЭТИ»
Факультет компьютерных технологий и информатики (ФКТИ) 2022***
# Практическая работа №1
**Знакомство с базовой структурой многопоточной программы и с системными вызовами, обеспечивающими создание и завершение потоков.**
>Написать программу, содержащую два потока (в дополнение к основному потоку). Каждый из потоков должен выводить определенное число на экран.
При запуске потоков передать в них адреса флагов завершения, при этом объявить флаги завершения локальными в функции main().При завершении потоков выставить некоторые значения кодов завершения, а затем прочитать эти коды завершения в функции main() и вывести на экран.
# Практическая работа №2
**Знакомство со средствами синхронизации потоков – неименованными семафорами, а также с системными вызовами, обеспечивающими создание, закрытие, захват и освобождение неименованных семафоров.**
>Написать программу, содержащую два потока, осуществляющих координированный доступ к разделяемому ресурсу. В качестве разделяемого ресурса в данной работе выбирается экран.

### 111111111122222222221111111111222222222211111111112222222222
- Реализовать три экземпляра программы:
1. С блокирующей операцией захвата семафора ```int sem_wait(sem_t *sem)``` При входе в критический участок, ```int sem_post(sem_t *sem)``` При выходе из критического участка.
2. С операцией проверки захвата семафора ```int sem_trywait(sem_t *sem)``` 
3. С блокировкой на время операции захвата ```int clock_gettime(clockid_t clk_id, struct timespec *tp)```
# Практическая работа №3
**Знакомство со средством взаимодействия потоков и процессов – неименованными каналами и с системными вызовами, обеспечивающими создание и закрытие неименованных каналов, а также передачу и прием данных через неименованные каналы.**
>Написать программу, содержащую два потока, один из которых пишет данные в неименованный канал (pipe, pipe2), а другой читает данные из канала.
>Использовать fcntl функцию для установления флагов состояния дескрипторов.
1. Взаимодействие потоков через неименованные каналы;
2. В качестве сообщения необходимо передавать результат выполнения некоторой функции ОС. ```(struct hostent *gethostbyname(const char *name)``` - получение информации о хосте по его имени; Из структуры выбрать ip-адрес и его передавать.
# Практическая работа №4
**Знакомство с основными системными вызовами, обеспечивающими создание процессов.**
>Написать программу, которая при запуске принимает аргументы командной строки, а затем в цикле выводит каждый аргумент на экран с задержкой в одну секунду.
1. Программа  должна выводить на экран свой идентификатор и идентификатор процесса-родителя.
2. Программа  должна формировать код завершения.
3. Программа  будет выполнять роль дочернего процесса.
4. Написать программу 2, которая запускает 1 программу в качестве дочернего процесса с помощью вызовов ```fork()``` и ```int execlp(const char *file, const char *arg, ...)```.
5. Программа 2 должна вывести на экран идентификатор процесса-родителя, свой идентификатор и идентификатор дочернего процесса.
6. Программа 2 должна сформировать набор параметров для передачи в дочерний процесс аргументов командной строки.
7. Программа 2 должна ожидать завершения дочернего процесса, проверяя событие завершения каждую половину секунды, а по завершению дочернего процесса вывести на экран код завершения.
8. Программа 2 будет выполнять роль родительского процесса. 
### Вариант функции exec()  Функция, загружает и запускает другие программы.
```c    
    int execve(const char *pathname, char *const argv [], char *const envp[]);
    int execl(const char *pathname, const char *arg, ...);
    int execlp(const char *file, const char *arg, ...);
    int execle(const char *pathname, const char *arg,..., char * const envp[]);
    int execv(const char *pathname, char *const argv[]);
    int execvp(const char *file, char *const argv[]);
    int execvpe(const char *file, char *const argv[], char *const envp[])
```
**При реализации задания необходимо учесть особенности варианта функции execlp()**
1. Суффиксы **l, v, p** и **e**, добавляемые к имени семейства exec... обозначают, что данная функция будет работать с некоторыми особенностями:

-  &#9989; **p** - определяет, что функция будет искать "дочернюю" программу в директориях, определяемых переменной среды DOS PATH. Без суффикса p поиск будет  производиться только в рабочем каталоге. Если параметр path  не  содержит  маршрута, то поиск производится в текущей директории, а затем по маршрутaм, определяемым переменной окружения PATH.

-  &#9989; **l** - показывает, что адресные указатели (arg0, arg1,..., argn) передаются, как отдельные аргументы. Обычно суффикс l употребляется, когда число передаваемых аргументов заранее вам известно.

-  &#9989; **v** - показывает, что адресные указатели (arg[0],arg[1],...arg[n]) передаются, как массив указателей. Обычно, суффикс v используется, когда передаeтся переменное число аргументов.

-  &#9989; **e** - показывает, что "дочернему" процессу может быть передан аргумент envp, который позволяет выбирать среду "дочернего" процесса. Без суффикса e "дочерний" процесс унаследует среду "родительского" процесса.

**Каждая функция семейства exec... должна иметь один из двух суффиксов, определяющих аргументы (либо l, либо v). Суффиксы определения маршрута доступа (PATH) и унаследования операционной среды (p и e) являются необязательными.**
-  &#9989;  Пояснение: Используйте функцию putenv();  Для изменения параметров среды для программы. 
```c
    #include <iostream>
    #include <cstdlib>
    int main() {
      putenv("PATH=C:\");
      cout<<getenv("PATH");}
 ```
# Практическая работа №5
**Знакомство со средством синхронизации процессов - именованными семафорами и с системными вызовами, обеспечивающими создание, открытие, закрытие и удаление именованных семафоров, а также захват и освобождение именованных семафоров.**
>Написать программу, содержащую две программы, осуществляющих координированный доступ к разделяемому ресурсу. Необходимо выбрать общий ресурс – файл и осуществлять запись данных в него из двух программ.

   **В стандарте POSIX именованный семафор создается следующим вызовом:**
     
```sem_t *sem_open(const char *name, int oflag, mode_t mode, unsigned int value)```
- Где: name – имя семафора;
- oflag – флаг, управляющий операцией создания семафора;

    |Флаг|Описание|
    |:---|:------:|
    |O_CREAT|Cемафор создаётся, если ещё не существует. Владельцем (ID пользователя) семафора устанавливается эффективный ID пользователя вызывающего процесса|
- mode – права доступа к семафору;

    |Флаг|Символьные константы, используемые в mode|
    |:---|:---------------------------------------:|
    |S_IRWXU|00700 пользователь (владелец файла) имеет права на чтение, запись и выполнение файла|
    |S_IRUSR|00400 пользователь имеет права на чтение файла|
    |S_IWUSR|00200 пользователь имеет права на запись в файл|
    |S_IXUSR|00100 пользователь имеет права на выполнение файла|
    |S_IRWXG|00070 группа имеет права на чтение, запись и выполнение файла|
    |S_IRGRP|00040 группа имеет права на чтение файла|
    |S_IWGRP|00020 группа имеет права на запись в файл|
    |S_IXGRP|00010 группа имеет права на выполнение файла|
    |S_IRWXO|00007 все остальные имеют права на чтение, запись и выполнение файла|
    |S_IROTH|00004 все остальные имеют права на чтение файла|
    |S_IWOTH|00002 все остальные имеют права на запись в файл|
    |S_IXOTH|00001 все остальные имеют права на выполнение файла|
- value –	начальное состояние семафора.
1. Реально надо написать одну программу, скопировать ее с другим именем и поменять в ней запись символа ‘1’ на запись символа ‘2’.
- Каждая программа запускается в отдельном терминале.
- Но для наблюдения за работой программ в каждой программе необходимо сделать вывод символа не только в файл, но и на экран своего терминала.

      Для наблюдения за записью в файл надо открыть третий терминал и ввести в нем следующую команду:
      tail –f filename. Где filename – это имя файла, в который вы пишете символы.
2. Обе программы должны создать (или открыть, если создан) один и тот же именованный семафор, должны создать (или открыть, если создан) один и тот же файл, но записывать в файл разные символы.

      ```В двух разных программах семафор становится общим, если у него одинаковое имя – первый параметр функции sem_open(). Обратите внимание на требование к имени семафора – оно должно быть в форме /somename, т.е. первый символ - слэш.```
- Необходимо убедиться, что в случае отсутствия именованного семафора процессы выводят символы в файл в произвольном порядке, например, так:

### 121212121212121212121212121212121212121212121212121212
- В случае использования именованного семафора процессы выводят символы в файл в определенном порядке, например, так:
### 111111111122222222221111111111222222222211111111112222
- Поскольку в этой работе мы используем семафорную операцию ```sem_wait()```(с блокировкой), то при завершении одной программы вторая может зависнуть в этой операции. Выходом из такой ситуации является аварийное завершение программы (ctrl + с). В этом случае семафор может оказаться не удалённым и, более того, может оказаться в запертом состоянии. Тогда повторно запустить программу не удастся. В ОС Linux семафоры – это файлы в каталоге /dev/shm. Вы можете войти в этот каталог и вручную удалить семафор.

**При реализации задания необходимо реализовать программу с потоками.**
- Это когда цикл записи символа в файл и на экран помещен в отдельный поток. Функция getchar() в main() выполняется с блокировкой, а поток создается в единственном экземпляре. Поскольку поток один, то появляется закономерный вопрос – может быть он вообще не нужен. Строго говоря, их два – один работает, другой следит за нажатием клавиши.
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

Функция, которая выполняется на передающей стороне ```struct hostent *gethostbyname(const char *name)``` - получение информации о хосте по его имени;
# Практическая работа №7
**Знакомство с механизмом взаимодействия процессов – очередями сообщений и с системными вызовами, обеспечивающими создание, закрытие, удаление очередей сообщений, а также передачу и прием сообщений.**
>Написать комплект из двух программ, одна из которых передает сообщение в очередь сообщений (стандарта очереди POSIX), а вторая – принимает сообщения из очереди сообщений. Проверить работу функций с блокировкой и без блокировки.
>Функция, которая выполняется на передающей стороне
```c 
struct hostent *gethostbyname(const char *name) 
```


# Практическая работа №6
**Знакомство с механизмом обмена данными между процессами – разделяемой памятью и с системными вызовами, обеспечивающими создание разделяемой памяти, отображения ее на локальную память, передачу данных, чтение данных, закрытие и удаление разделяемой памяти.**
>Написать программу, содержащую две программы и запускать их в двух терминалах. Комплект из двух программ, одна из которых посылает данные в разделяемую память, а вторая – читает эти данные. Поскольку механизм разделяемой памяти не содержит средств синхронизации записи и чтения, для синхронизации требуется применить механизм именованных семафоров. В программах необходимо использовать только функции ```sem_wait()```

   **Выбор стандарта POSIX**
   
   **В стандарте POSIX участок разделяемой памяти создается:**

```c
int shm_open(const char *name, int oflag, mode_t mode);
``` 
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
    
```c
int ftruncate(int fd, off_t length);
```
- Где: fd -	дескриптор разделяемой памяти, полученный как результат вызова функции 
```c
shm_open();
```
- length – требуемый размер разделяемой памяти.

    **Отображение разделяемой памяти на локальный адрес создается вызовом:**
    
```c
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
```
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
    
```c
int munmap(void *addr, size_t length);
```
- Где: addr –	локальный адрес отображения;
- length - размер отображения.

    **Закрытие участка разделяемой памяти:**

```c
int close(int fd);
```
- Где: fd –	дескриптор разделяемой памяти.

    **Удаление участка разделяемой памяти:**

```c
int shm_unlink(const char *name);
```
- Где: name –	имя участка разделяемой памяти.

Функция, которая выполняется на передающей стороне
```c
struct hostent *gethostbyname(const char *name)
```

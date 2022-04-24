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
1. С блокирующей операцией захвата семафора int sem_wait(sem_t *sem) При входе в критический участок, int sem_post(sem_t *sem) При выходе из критического участка.
2. С операцией проверки захвата семафора int sem_trywait(sem_t *sem) 
3. С блокировкой на время операции захвата int clock_gettime(clockid_t clk_id, struct timespec *tp)
# Практическая работа №3
**Знакомство со средством взаимодействия потоков и процессов – неименованными каналами и с системными вызовами, обеспечивающими создание и закрытие неименованных каналов, а также передачу и прием данных через неименованные каналы.**
>Написать программу, содержащую два потока, один из которых пишет данные в неименованный канал (pipe, pipe2), а другой читает данные из канала.
>Использовать fcntl функцию для установления флагов состояния дескрипторов.
1. Взаимодействие потоков через неименованные каналы;
2. В качестве сообщения необходимо передавать результат выполнения некоторой функции ОС. (struct hostent *gethostbyname(const char *name) - получение информации о хосте по его имени; Из структуры выбрать ip-адрес и его передавать.
# Практическая работа №4
**Знакомство с основными системными вызовами, обеспечивающими создание процессов.**
>Написать программу, которая при запуске принимает аргументы командной строки, а затем в цикле выводит каждый аргумент на экран с задержкой в одну секунду.
1. Программа  должна выводить на экран свой идентификатор и идентификатор процесса-родителя.
2. Программа  должна формировать код завершения.
3. Программа  будет выполнять роль дочернего процесса.
4. Написать программу 2, которая запускает 1 программу в качестве дочернего процесса с помощью вызовов fork() и int execlp(const char *file, const char *arg, ...).
5. Программа 2 должна вывести на экран идентификатор процесса-родителя, свой идентификатор и идентификатор дочернего процесса.
6. Программа 2 должна сформировать набор параметров для передачи в дочерний процесс аргументов командной строки.
7. Программа 2 должна ожидать завершения дочернего процесса, проверяя событие завершения каждую половину секунды, а по завершению дочернего процесса вывести на экран код завершения.
8. Программа 2 будет выполнять роль родительского процесса. 
### Вариант функции exec()  Функция, загружает и запускает другие программы.  . 
  
    int execve(const char *pathname, char *const argv [], char *const envp[]);
    int execl(const char *pathname, const char *arg, ...),
    int execlp(const char *file, const char *arg, ...),
    int execle(const char *pathname, const char *arg,..., char * const envp[]),
    int execv(const char *pathname, char *const argv[]),
    int execvp(const char *file, char *const argv[]),
    int execvpe(const char *file, char *const argv[], char *const envp[]).
**При реализации задания необходимо учесть особенности варианта функции execlp()**
1. Суффиксы **l, v, p** и **e**, добавляемые к имени семейства exec... обозначают, что данная функция будет работать с некоторыми особенностями:

-  &#9989; **p** - определяет, что функция будет искать "дочернюю" программу в директориях, определяемых переменной среды DOS PATH. Без суффикса p поиск будет  производиться только в рабочем каталоге. Если параметр path  не  содержит  маршрута, то поиск производится в текущей директории, а затем по маршрутaм, определяемым переменной окружения PATH.

-  &#9989; **l** - показывает, что адресные указатели (arg0, arg1,..., argn) передаются, как отдельные аргументы. Обычно суффикс l употребляется, когда число передаваемых аргументов заранее вам известно.

-  &#9989; **v** - показывает, что адресные указатели (arg[0],arg[1],...arg[n]) передаются, как массив указателей. Обычно, суффикс v используется, когда передаeтся переменное число аргументов.

-  &#9989; **e** - показывает, что "дочернему" процессу может быть передан аргумент envp, который позволяет выбирать среду "дочернего" процесса. Без суффикса e "дочерний" процесс унаследует среду "родительского" процесса.

**Каждая функция семейства exec... должна иметь один из двух суффиксов, определяющих аргументы (либо l, либо v). Суффиксы определения маршрута доступа (PATH) и унаследования операционной среды (p и e) являются необязательными.**
# Практическая работа №5
**Знакомство со средством синхронизации процессов - именованными семафорами и с системными вызовами, обеспечивающими создание, открытие, закрытие и удаление именованных семафоров, а также захват и освобождение именованных семафоров.**

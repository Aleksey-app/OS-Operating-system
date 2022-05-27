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
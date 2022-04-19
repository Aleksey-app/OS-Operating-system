#include<unistd.h>
#include<iostream>
using namespace std;
struct stream{
    bool x;
    int y;
    string z;
}st;
static void* process1(void* flag1){     
    stream *f1 = (stream*) flag1;
    cout << "\x1B[38;5;45m"<< f1->z << endl;
    while (f1->x)
    {
        cout <<"\x1B[38;5;45m" <<f1->y;
        usleep(10000);
    }
    cout <<"\n"<<"\x1B[38;5;45m"<<f1->z;
    f1->z = "Поток 1 закончил работу";
    pthread_exit(flag1);
}
static void* process2(void* flag2){
    stream *f2 = (stream*) flag2;
    cout << "\x1B[38;5;60m"<< f2->z << endl;
    while(f2->x)
    {
        cout <<"\x1B[38;5;60m"<< f2->y;
        usleep(10000);
    }
    cout <<"\n"<<"\x1B[38;5;60m"<<f2->z;
    f2->z = "Поток 2 закончил работу";
    pthread_exit(flag2);
}
int main(){
    int error = 0, error1 = 0;
	cout <<"\x1B[0;31;40mПрограмма начала работу..."<< endl;
    pthread_t id4321;
    stream flag1 = {true, 1, "Поток 1 начал работу..."};
    error=pthread_create(&id4321,NULL,process1,(void*) &flag1);
    if (error){
        perror("[-]");
    }
    pthread_t id4322;
    stream flag2 = {true, 2, "Поток 2 начал работу..."};
    error1=pthread_create(&id4322,NULL,process2,(void*) &flag2);
    if (error1){
        perror("[-]");
    }
    void *status;
    cout <<"\x1B[0;31;40mПрограмма ждёт нажатия клавиши..."<< endl;
    getchar();
    flag1 = {false, 1, "Клавиша нажата."};
    pthread_join(id4321,&status);
    stream *ex = (stream*) status;
    cout <<endl <<"\x1B[38;5;45m"<< ex->x << " " << ex->z << endl;
    flag2 = {false, 2, "Клавиша нажата."};
    pthread_join(id4322,&status);
    ex = (stream*) status;
    cout << endl << "\x1B[38;5;60m"<< ex->x << " " << ex->z << endl;
    cout <<" \x1b[0mПрограмма завершила работу."<< endl;
	return 0;
}

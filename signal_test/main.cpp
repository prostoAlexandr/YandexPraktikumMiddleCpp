#include <sys/time.h>
#include <unistd.h>
#include <iostream>

int main() {
    struct itimerval timer;
    timer.it_value.tv_sec = 1;  // Сработает через 1 секунду
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    std::cout << "Заводим таймер и засыпаем..." << std::endl;
    setitimer(ITIMER_REAL, &timer, NULL);

    while(true) {
        sleep(1);
        std::cout << "Этот текст никогда не напечатается" << std::endl;
    }
    return 0;
}
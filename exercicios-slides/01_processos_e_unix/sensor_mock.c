#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main() {
    for(;;) {
        printf("Sensor mock: %d\n", rand() % 100);
        Sleep(1000);
    }

    return 0;
}
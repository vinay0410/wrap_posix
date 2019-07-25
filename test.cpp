#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/shm.h> 
#include <sys/stat.h> 
#include <iostream>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <errno.h>


int main() {
    for (int i = 0; i < 1000; i++) {
        sem_t* sem = sem_open("/abcd", O_CREAT, 0777, 1);
        //sem_close(sem);
    }

    sleep(40);
}
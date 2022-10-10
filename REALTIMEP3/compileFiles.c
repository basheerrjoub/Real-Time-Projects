#include<stdio.h>

int main() {
    system("gcc parent.c -o parent -Llocal -lrt -pthread");
    system("gcc fac.c -o fac -Llocal -lrt -pthread");
    system("gcc detergent.c -o detergent -Llocal -lrt -pthread");
    return 0;

}
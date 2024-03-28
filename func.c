#include <stdio.h>

int countPowersOfTwo(int num) {
    int count = 0;
    while (num > 0) {
        if (num % 2 == 0) {
            num /= 2;
            count++;
        } else {
            break;
        }
    }
    return count;
}

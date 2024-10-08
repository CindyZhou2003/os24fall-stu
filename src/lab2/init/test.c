#include "printk.h"
#include "defs.h"

// Please do not modify

// void test() {
    // while (1);
// }

void test() {
    int i=0;
    while (1) {
        if ((++i) % 100000000 == 0) {
            printk("kernel is running!\n");
            i=0;
        }
    }
}


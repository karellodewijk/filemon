#include "filemon.h"
#include <stdio.h>

void file_changed_callback(char* path) {
    printf("File changed: path\n");
}

int main() {
    filemon_init();
    filemon_monitor_modify("./", &file_changed_callback);
    filemon_wait();
    return 0;
}

#include "filemon.h"
#include <stdio.h>

void file_changed_callback(const char* path) {
    printf("File changed: %s\n", path);
    //filemon_stop_monitoring("./");
    filemon_interrupt();
}

int main() {
    filemon_init();
    filemon_monitor_modify("./", &file_changed_callback);
    filemon_wait();
    filemon_monitor_modify("./", &file_changed_callback);
    filemon_wait();
    filemon_destruct();
    printf("Program ended\n");
    return 0;
}

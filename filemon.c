#include "filemon.h"

filemon_state* global_state = 0;

void filemon_init() {
    global_state = filemon_init_state();
}

void filemon_monitor_create (char* path, void (*f) (char* path)) {
    filemon_monitor_create_state(global_state, path, f);
}

void filemon_monitor_create_recursive (char* path, void (*f) (char* path)) {
    filemon_monitor_create_recursive_state(global_state, path, f);
}

void filemon_monitor_delete (char* path, void (*f) (char* path)) {
    filemon_monitor_delete_state(global_state, path, f);
}

void filemon_monitor_delete_recursive (char* path, void (*f) (char* path)) {
    filemon_monitor_delete_recursive_state(global_state, path, f);
}

void filemon_monitor_modify (char* path, void (*f) (char* path)) {
    filemon_monitor_modify_state(global_state, path, f);
}

void filemon_monitor_modify_recursive (char* path, void (*f) (char* path)) {
     filemon_monitor_modify_recursive_state(global_state, path, f);
}

void filemon_monitor_all (char* path, void (*f) (char* path, filemon_flag flags)) {
    filemon_monitor_all_state(global_state, path, f);
}

void filemon_monitor_all_recursive (char* path, void (*f) (char* path, filemon_flag flags)) {
    filemon_monitor_all_recursive_state(global_state, path, f);
}

void filemon_stop_monitoring (char* path) {
     filemon_stop_monitoring_state(global_state, path);
}

void filemon_wait () {
    filemon_wait_state(global_state);
}

void filemon_wait_nonblock () {
    filemon_wait_nonblock_state(global_state);
}

void filemon_interrupt () {
    filemon_interrupt_state(global_state);
}


void filemon_destruct() {
    filemon_destruct_state(global_state);
}

#if defined(__linux__)
    #define USE_INOTIFY
#endif

typedef struct {
    char* path;
    filemon_flag flags;
    void (*callback_function) (char* path);
} filemon_callback;

//inotify implementation

#ifdef USE_INOTIFY
    #include <linux/inotify.h>

    struct filemon_state_ {

    };

    filemon_state* filemon_init_state() {

    }

    void filemon_monitor_create_state (filemon_state* state, char* path, void (*f) (char* path)) {

    }

    void filemon_monitor_create_recursive_state (filemon_state* state, char* path, void (*f) (char* path)) {

    }

    void filemon_monitor_delete_state (filemon_state* state, char* path, void (*f) (char* path)) {

    }

    void filemon_monitor_delete_recursive_state (filemon_state* state, char* path, void (*f) (char* path)) {

    }

    void filemon_monitor_modify_state (filemon_state* state, char* path, void (*f) (char* path)) {

    }

    void filemon_monitor_modify_recursive_state (filemon_state* state, char* path, void (*f) (char* path)) {

    }

    void filemon_monitor_all_state (filemon_state* state, char* path, void (*f) (char* path, filemon_flag flags)) {

    }

    void filemon_monitor_all_recursive_state (filemon_state* state, char* path, void (*f) (char* path, filemon_flag flags)) {

    }

    void filemon_stop_monitoring_state (filemon_state* state, char* path) {

    }

    void filemon_destruct_state (filemon_state* state) {

    }

    void filemon_wait_state (filemon_state* state) {

    }

    void filemon_wait_nonblock_state (filemon_state* state) {

    }

    void filemon_interrupt_state (filemon_state* state) {

    }

#endif //USE_INOTIFY

#include "filemon.h"
#include <stdio.h>

filemon_state* global_state = 0;

int filemon_init() {
    global_state = filemon_init_state();
    if (!global_state) 
        return -1;
    else
        return 0;
}

int filemon_monitor_create (char* path, void (*f) (char* path)) {
    return filemon_monitor_create_state(global_state, path, f);
}

int filemon_monitor_create_recursive (char* path, void (*f) (char* path)) {
    return filemon_monitor_create_recursive_state(global_state, path, f);
}

int filemon_monitor_delete (char* path, void (*f) (char* path)) {
    return filemon_monitor_delete_state(global_state, path, f);
}

int filemon_monitor_delete_recursive (char* path, void (*f) (char* path)) {
    return filemon_monitor_delete_recursive_state(global_state, path, f);
}

int filemon_monitor_modify (char* path, void (*f) (char* path)) {
    return filemon_monitor_modify_state(global_state, path, f);
}

int filemon_monitor_modify_recursive (char* path, void (*f) (char* path)) {
     return filemon_monitor_modify_recursive_state(global_state, path, f);
}

int filemon_monitor_all (char* path, void (*f) (char* path, filemon_flag flags)) {
    return filemon_monitor_all_state(global_state, path, f);
}

int filemon_monitor_all_recursive (char* path, void (*f) (char* path, filemon_flag flags)) {
    return filemon_monitor_all_recursive_state(global_state, path, f);
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
    //requires both inotify and epoll to be available

    #include <linux/inotify.h>
    #include <sys/epoll.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <limits.h>
    #include <assert.h>
    #include <glib.h>

    #define MAX_EVENTS 64
    #define MAX_INOTIFY_EVENT_SIZE  ( sizeof (struct inotify_event) + NAME_MAX + 1 ) /*size of one event*/

    struct filemon_state_ {
        int inotify_fd;
        char inotify_buffer[MAX_INOTIFY_EVENT_SIZE];

        int epoll_fd;
        struct epoll_event epoll_ev;
        struct epoll_event events[MAX_EVENTS];

        GTree* entry_by_path_lookup;
        GTree* entry_by_wfd_lookup;
    };

    filemon_state* filemon_init_state() {
        filemon_state* state = malloc(sizeof(filemon_state));
        if (state == 0) return 0;

        state->inotify_fd = inotify_init();

        if ( state->inotify_fd < 0 ) {
            perror( "inotify failed to initialize\n" );
        }

        state->epoll_fd = epoll_create(sizeof(state->epoll_fd));
        if (state->epoll_fd < 0)
            perror("Could not initialized epoll file descriptor\n");

        state->epoll_ev.events = EPOLLIN;
        int cfg;
        cfg = epoll_ctl(state->epoll_fd, EPOLL_CTL_ADD, state->inotify_fd, &state->epoll_ev);
        if (cfg < 0)
            perror("Could not configure the epoll interface\n");

        state->entry_by_path_lookup = g_tree_new((GCompareFunc)g_ascii_strcasecmp);

        return state;
    }

    int filemon_monitor_create_state (filemon_state* state, char* path, void (*f) (char* path)) {

    }

    int filemon_monitor_create_recursive_state (filemon_state* state, char* path, void (*f) (char* path)) {

    }

    int filemon_monitor_delete_state (filemon_state* state, char* path, void (*f) (char* path)) {

    }

    int filemon_monitor_delete_recursive_state (filemon_state* state, char* path, void (*f) (char* path)) {

    }

    int filemon_monitor_modify_state (filemon_state* state, char* path, void (*f) (char* path)) {
        int watch_fd = inotify_add_watch(state->inotify_fd, path, IN_CLOSE_WRITE);
        printf("Monitoring: %s\n", path);
        if (watch_fd < 0) {
            printf("Couldn't add watch to %s\n", path);
        }
    }

    int filemon_monitor_modify_recursive_state (filemon_state* state, char* path, void (*f) (char* path)) {

    }

    int filemon_monitor_all_state (filemon_state* state, char* path, void (*f) (char* path, filemon_flag flags)) {

    }

    int filemon_monitor_all_recursive_state (filemon_state* state, char* path, void (*f) (char* path, filemon_flag flags)) {

    }

    void filemon_stop_monitoring_state (filemon_state* state, char* path) {
        close(state->epoll_fd);
        close(state->inotify_fd);
    }

    void filemon_destruct_state (filemon_state* state) {

    }

    void filemon_wait_state (filemon_state* state) {
        for (;;) {
            int nr_of_events = epoll_wait(state->epoll_fd, state->events, MAX_EVENTS, -1);
            int i;
            for (i = 0; i < nr_of_events; ++i) {
                int j = 0;
                int length = read(state->inotify_fd, state->inotify_buffer, MAX_INOTIFY_EVENT_SIZE);

                if ( length < 0 )
                    perror( "inotify read error.\n" );
                
                while (j < length) {
                    struct inotify_event* event = (struct inotify_event*) &state->inotify_buffer[j];
                    if (event->mask & IN_IGNORED) { //Watch was removed explicitly (inotify_rm_watch) or automatically (file was deleted, or file system was unmounted). 
                        //cleanup
                    }


                    printf("%s\n", event->name);
                    j += sizeof(struct inotify_event) + event->len; //move to next event
                }
            } 
        }
    }


    void filemon_wait_nonblock_state (filemon_state* state) {

    }

    void filemon_interrupt_state (filemon_state* state) {
        
    }

#endif //USE_INOTIFY

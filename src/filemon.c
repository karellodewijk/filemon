#include "filemon.h"
#include <stdio.h>

filemon_state* global_state = NULL;

int filemon_init() {
    global_state = filemon_init_state();
    if (!global_state) 
        return -1;
    else
        return 0;
}

int filemon_monitor_create (const char* path, void (*f) (const char* path)) {
    return filemon_monitor_create_state(global_state, path, f);
}

int filemon_monitor_create_recursive (const char* path, void (*f) (const char* path)) {
    return filemon_monitor_create_recursive_state(global_state, path, f);
}

int filemon_monitor_delete (const char* path, void (*f) (const char* path)) {
    return filemon_monitor_delete_state(global_state, path, f);
}

int filemon_monitor_delete_recursive (const char* path, void (*f) (const char* path)) {
    return filemon_monitor_delete_recursive_state(global_state, path, f);
}

int filemon_monitor_modify (const char* path, void (*f) (const char* path)) {
    return filemon_monitor_modify_state(global_state, path, f);
}

int filemon_monitor_modify_recursive (const char* path, void (*f) (const char* path)) {
     return filemon_monitor_modify_recursive_state(global_state, path, f);
}

int filemon_monitor_all (const char* path, void (*f) (const char* path, filemon_flag flags)) {
    return filemon_monitor_all_state(global_state, path, f);
}

int filemon_monitor_all_recursive (const char* path, void (*f) (const char* path, filemon_flag flags)) {
    return filemon_monitor_all_recursive_state(global_state, path, f);
}

int filemon_stop_monitoring (const char* path) {
     return filemon_stop_monitoring_state(global_state, path);
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
    global_state = 0;
}

#if defined(__linux__)
    #define USE_INOTIFY
#endif

typedef struct {
    const char* path;
    filemon_flag flags;
    void (*callback_function) (const char* path);
} filemon_callback;

//inotify implementation

#ifdef USE_INOTIFY
    //requires both inotify and epoll

    //#include <linux/inotify.h>
    #include <sys/inotify.h>
    #include <sys/epoll.h>
    #include <sys/eventfd.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <limits.h>
    #include <assert.h>
    #include <glib.h>
    #include <string.h>

    #define MAX_EVENTS 64 //maximum queue size of epoll queue, not a limit on file watches or anything
    #define MAX_INOTIFY_EVENT_SIZE  ( sizeof (struct inotify_event) + NAME_MAX + 1 ) //maximum size of one inotify event

    struct filemon_state_ {
        int inotify_fd;
        char inotify_buffer[MAX_INOTIFY_EVENT_SIZE];

        int epoll_fd;
        struct epoll_event epoll_ev;
        struct epoll_event events[MAX_EVENTS];

        GHashTable* entry_by_path_lookup;
        GHashTable* entry_by_wfd_lookup;

	    int interrupt_fd;
    };

    struct filemon_entry {
        char* path;
        int wfd; //inotify watch fd
        void (*callback_f) (const char* path);
    };

    filemon_state* filemon_init_state() {
        filemon_state* state = malloc(sizeof(struct filemon_state_));
        if (state == 0) return 0;

        state->inotify_fd = inotify_init();
        if ( state->inotify_fd < 0 ) {
            perror( "inotify fd failed to initialize\n" );
	        return 0;
        }

        //printf("Inotify fd: %d\n", state->inotify_fd);

	    state->interrupt_fd = eventfd(0, EFD_NONBLOCK);
        if ( state->interrupt_fd < 0 ) {
            perror( "interrupt fd failed to initialize\n" );
            close(state->inotify_fd); //cleanup
	        return 0;
        }

        //printf("Interrupt fd: %d\n", state->interrupt_fd);

        memset(&state->epoll_ev, 0, sizeof(state->epoll_ev)); 
        state->epoll_ev.events = EPOLLIN; //type of events we will monitor (read)

        state->epoll_fd = epoll_create(sizeof(state->epoll_fd));
        if (state->epoll_fd < 0) {
            perror("Could not initialized epoll fd\n");
            close(state->interrupt_fd); //cleanup
            close(state->inotify_fd);
	        return 0;
	    }

        int cfg;
        state->epoll_ev.data.fd = state->interrupt_fd;
        cfg = epoll_ctl(state->epoll_fd, EPOLL_CTL_ADD, state->interrupt_fd, &state->epoll_ev);

        if (cfg < 0) {
            perror("Could not configure the epoll interface\n");
            close(state->epoll_fd); //cleanup
            close(state->interrupt_fd);
            close(state->inotify_fd);
	        return 0;
	    }

        state->epoll_ev.data.fd = state->inotify_fd;
        cfg = epoll_ctl(state->epoll_fd, EPOLL_CTL_ADD, state->inotify_fd, &state->epoll_ev);
        if (cfg < 0) {
            perror("Could not configure the epoll interface\n");
            close(state->epoll_fd); //cleanup
	        close(state->interrupt_fd);
	        close(state->inotify_fd);
	        return 0;
	    }

        state->entry_by_path_lookup = g_hash_table_new(g_str_hash, g_str_equal);
        state->entry_by_wfd_lookup = g_hash_table_new(g_int_hash, g_int_equal);

        return state;
    }

    int filemon_monitor_create_state (filemon_state* state, const char* path, void (*f) (const char* path)) {
	    assert(state); //filemon state not initialized
    }

    int filemon_monitor_create_recursive_state (filemon_state* state, const char* path, void (*f) (const char* path)) {
	    assert(state); //filemon state not initialized
    }

    int filemon_monitor_delete_state (filemon_state* state, const char* path, void (*f) (const char* path)) {
	    assert(state); //filemon state not initialized
    }

    int filemon_monitor_delete_recursive_state (filemon_state* state, const char* path, void (*f) (const char* path)) {
	    assert(state); //filemon state not initialized
    }

    int filemon_monitor_modify_state (filemon_state* state, const char* path, void (*f) (const char* path)) {
	    assert(state); //filemon state not initialized

        int wfd = inotify_add_watch(state->inotify_fd, path, IN_CLOSE_WRITE);

        //printf("Monitoring: %s\n", path);
        if (wfd < 0) {
            fprintf(stderr, "Couldn't add watch to %s\n", path);
	        return -1;
        }

        struct filemon_entry* entry = malloc(sizeof(struct filemon_entry));
        entry->path = strdup(path);
        entry->wfd = wfd;
        entry->callback_f = f;

        g_hash_table_insert(state->entry_by_path_lookup, &entry->path, entry);
        g_hash_table_insert(state->entry_by_wfd_lookup, &entry->wfd, entry);

    }

    int filemon_monitor_modify_recursive_state (filemon_state* state, const char* path, void (*f) (const char* path)) {
	    assert(state); //filemon state not initialized
    }

    int filemon_monitor_all_state (filemon_state* state, const char* path, void (*f) (const char* path, filemon_flag flags)) {
	    assert(state); //filemon state not initialized
    }

    int filemon_monitor_all_recursive_state (filemon_state* state, const char* path, void (*f) (const char* path, filemon_flag flags)) {
	    assert(state); //filemon state not initialized
    }

    int filemon_stop_monitoring_state (filemon_state* state, const char* path) {
	    assert(state); //filemon state not initialized

        char* key = NULL;
        char** key_ptr = &key;
        struct filemon_entry* entry = NULL;
        struct filemon_entry** entry_ptr  = &entry;

        gboolean result = g_hash_table_lookup_extended(state->entry_by_path_lookup, path, (void**) key_ptr, (void**) entry_ptr);

        if (result) {
            g_hash_table_remove(state->entry_by_path_lookup, &entry->path);
            g_hash_table_remove(state->entry_by_wfd_lookup, &entry->wfd);
            int errno = inotify_rm_watch(state->inotify_fd, entry->wfd);
            if (errno < 0) {
                fprintf(stderr, "Removing inotify event for \"%s\" failed (%d)\n", path, errno);
                return -1;
            }
            close(entry->wfd);
            free(entry->path);
            free(entry);
        } else {
            fprintf(stderr, "Entry \"%s\" not found (%d)\n", path, result);
            return -1;
        }
        return 0;
    }

    void clear_entry(gpointer key, gpointer value, gpointer state) {
        struct filemon_entry* entry = (struct filemon_entry*) value;
        close(entry->wfd);
        free(entry->path);
        free(entry);
    }

    void filemon_destruct_state (filemon_state* state) {
	    assert(state); //filemon state not initialized
        g_hash_table_foreach(state->entry_by_path_lookup, (GHFunc)clear_entry, NULL);
        g_hash_table_destroy(state->entry_by_path_lookup);
        g_hash_table_destroy(state->entry_by_wfd_lookup);
        close(state->epoll_fd); //cleanup
        close(state->interrupt_fd);
        close(state->inotify_fd);
	    free(state);
	    state = 0;
    }

    void filemon_wait_state (filemon_state* state) {
	    assert(state); //filemon state not initialized

        for (;;) {
            int nr_of_events = epoll_wait(state->epoll_fd, &state->events, MAX_EVENTS, -1);
            if (nr_of_events < 0) {
                fprintf(stderr, "epoll_wait error(%d).\n", nr_of_events);
                continue; //continue because this happens unexpected sometimes and is not really a problem
            }

	        int stop = 0;
            int i;

            for (i = 0; i < nr_of_events; ++i) {
		        if (state->events[i].data.fd == state->interrupt_fd) {
                    uint64_t interrupt_code;
                    read(state->interrupt_fd, &interrupt_code, sizeof(uint64_t)); //read interrupt code
		            stop = 1; //we were interrupted, but still want to cleanly handle the event queue first
		        } else {
                    int length = read(state->inotify_fd, state->inotify_buffer, MAX_INOTIFY_EVENT_SIZE);

                    if ( length < 0 ) {
                        fprintf(stderr, "inotify read error(%d).\n", length);
                        return;
                    }

                    int j = 0;

                    while (j < length) {
                        struct inotify_event* event = (struct inotify_event*) &state->inotify_buffer[j];
                        if (event->mask & IN_IGNORED) { //Watch was removed explicitly (inotify_rm_watch) or automatically (file was deleted, or file system was unmounted). 
                            //TODO cleanup
                        }

                        struct filemon_entry* entry = NULL;
                        struct filemon_entry** entry_ptr  = &entry;
                        entry = g_hash_table_lookup(state->entry_by_wfd_lookup, &event->wd);

                        if (entry != NULL) {
                            entry->callback_f(entry->path);
                        }

                        j += sizeof(struct inotify_event) + event->len; //move to next event
                    }
                }
            }
	        if (stop) break;
        }
    }


    void filemon_wait_nonblock_state (filemon_state* state) {
	    assert(state); //filemon state not initialized
    }

    void filemon_interrupt_state (filemon_state* state) {
        uint64_t data = 1;
        write(state->interrupt_fd, &data, sizeof(data)); //write interrupt code (1)
    }

#endif //USE_INOTIFY

#ifndef FILEMON_H
#define FILEMON_H

//////////////
//INTERFACE://
//////////////

///initialise filemon
int filemon_init();

///pass a function pointer that will be called when a specific file or any file or directory in a given directory is created
int filemon_monitor_create (char* path, void (*f) (char* path));

///pass a function pointer that will be called when a specific file or any file or directory in a given directory or any directory rooted in the given directory is created
int filemon_monitor_create_recursive (char* path, void (*f) (char* path));

///pass a function pointer that will be called when a specific file or any file or directory in a given directory is removed
int filemon_monitor_delete (char* path, void (*f) (char* path));

///pass a function pointer that will be called when a specific file or any file or directory in a given directory or any directory rooted in the given directory is removed
int filemon_monitor_delete_recursive (char* path, void (*f) (char* path));

///pass a function pointer that will be called when a specific file or any fileor directory in a given directory is modified
int filemon_monitor_modify (char* path, void (*f) (char* path));

///pass a function pointer that will be called when a specific file or any file or directory in a given directory or any directory rooted in the given directory is modified
int filemon_monitor_modify_recursive (char* path, void (*f) (char* path));

typedef union {
    char all_flags;
    struct {
        char MODFIY : 1,
             CREATE : 1,
             DETELE : 1;
    };
} filemon_flag;

///pass a function pointer that will be called when a specific file or any file in a given directory is created/removed/modified, filemon_flag variable passed to the function will contain what happened
int filemon_monitor_all (char* path, void (*f) (char* path, filemon_flag flags));

///pass a function pointer that will be called when a specific file or any file or directory in a given directory or any directory rooted in the given directory is modifiedcreated/removed/modified, filemon_flag variable passed to the function will contain what happened
int filemon_monitor_all_recursive (char* path, void (*f) (char* path, filemon_flag flags));

///stop monitoring a file or directory
void filemon_stop_monitoring (char* path);

///wait for file monitor events (blocking)
void filemon_wait ();

///handle file monitor events (blocking)
void filemon_wait_nonblock ();

///interrupt the blobking filemon_wait_state() call, this function can be called from any thread
void filemon_interrupt ();

///teardown filemonitor
void filemon_destruct();

struct filemon_state_;
typedef struct filemon_state_ filemon_state;

///initialise filemon
filemon_state* filemon_init_state();

///pass a function pointer that will be called when a specific file or any file or directory in a given directory is created
int filemon_monitor_create_state (filemon_state* state, char* path, void (*f) (char* path));

///pass a function pointer that will be called when a specific file or any file or directory in a given directory or any directory rooted in the given directory is created
int filemon_monitor_create_recursive_state (filemon_state* state, char* path, void (*f) (char* path));

///pass a function pointer that will be called when a specific file or any file or directory in a given directory is removed
int filemon_monitor_delete_state (filemon_state* state, char* path, void (*f) (char* path));

///pass a function pointer that will be called when a specific file or any file or directory in a given directory or any directory rooted in the given directory is removed
int filemon_monitor_delete_recursive_state (filemon_state* state, char* path, void (*f) (char* path));

///pass a function pointer that will be called when a specific file or any fileor directory in a given directory is modified
int filemon_monitor_modify_state (filemon_state* state, char* path, void (*f) (char* path));

///pass a function pointer that will be called when a specific file or any file or directory in a given directory or any directory rooted in the given directory is modified
int filemon_monitor_modify_recursive_state (filemon_state* state, char* path, void (*f) (char* path));

///pass a function pointer that will be called when a specific file or any file in a given directory is created/removed/modified, filemon_flag variable passed to the function will contain what happened
int filemon_monitor_all_state (filemon_state* state, char* path, void (*f) (char* path, filemon_flag flags));

///pass a function pointer that will be called when a specific file or any file or directory in a given directory or any directory rooted in the given directory is modifiedcreated/removed/modified, filemon_flag variable passed to the function will contain what happened
int filemon_monitor_all_recursive_state (filemon_state* state, char* path, void (*f) (char* path, filemon_flag flags));

///stop monitoring a file or directory
void filemon_stop_monitoring_state (filemon_state* state, char* path);

///teardown filemonitor
void filemon_destruct_state (filemon_state* state);

///wait for file monitor events (blocking)
void filemon_wait_state (filemon_state* state);

///handle file monitor events (blocking)
void filemon_wait_nonblock_state (filemon_state* state);

///interrupt the blobking filemon_wait_state() call, this function can be called from any thread
void filemon_interrupt_state (filemon_state* state);

#endif //FILEMON_H

#ifndef _APPMAND_H_
#define _APPMAND_H_

#define _GNU_SOURCE /* For recursive mutex initializer. */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <dbus/dbus.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include "cJSON.h"

#define NUMBER_OF_THREADS 1
/* Maximum number of applications. */
#define MAX_NUMBER_APPLICATIONS 50
/* Maximum number of running applications. */
#define MAX_NUMBER_LIVE_APPLICATIONS 1
/* Manifest storage. */
#define MANIFEST_DIR "/etc/appmand/"

/* Qt application that controls login. */
#define APPMAN_LOGIN_PATH "/usr/bin/LoginScreen"
#define APPMAN_LOGIN "LoginScreen"
/* Qt application that leads appman daemon. */
#define APPMAN_VIEW_PATH "/usr/bin/DesktopScreen"
#define APPMAN_VIEW "DesktopScreen"

#define APPMAN_DEBUG_PREFIX "D >> "

#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0);

#define handle_error(msg) \
        do { perror(msg); exit(EXIT_FAILURE); } while (0);

typedef enum { false, true } bool;

/*
 * Thread information.
 * Passed as an argument from main process to threads.
 */
typedef struct thread_info {
    pthread_t thread;
    
} thread_info;

/*
 * Application structure.
 * id           : application id.
 * path         : execution path.
 * name         : application binary name.
 * group        : cgroup group name.
 * perms        : tbd.
 * prettyname   : pretty application name.
 * iconpath     : application icon's path.
 * color        : theme color.
 */
typedef struct application {
    unsigned int id;
    char* path;
    char* name;
    char* group;
    /* char* perms; */
    char* prettyname;
    char* iconpath;
    char* color;
    pid_t pid; /* in case of need, -1 until first run. */
} application;

const char *reasonstr(int, int);

/*
 * Forks, execs and returns pid of a child.
 */
pid_t run (const char *, const char *);

/*
 * Convert json data to application structure.
 */
void json_to_application (char *, int);

/*
 * Reads application list from manifest directory.
 * Returns 0 on success.
 */
int get_applist();

/*
 * Runs application with the given id.
 */
int runapp (int);

/*
 * Reply runapp request for dbus messages.
 */
void reply_runapp (DBusMessage*, DBusConnection*);

/*
 * Reply listapps request for dbus messages.
 */
void reply_listapps (DBusMessage*, DBusConnection*);

/*
 * Action that triggered if valid pin supplied.
 */
void login_access (DBusMessage* msg, DBusConnection* conn);

/*
 * Expose a method call and wait for it to be called.
 */
void listen ();

/*
 * Request handling with dbus.
 */
void *request_handler (void *);

/*
 * Handle termination status of child process.
 */
void status_handler (int, int);

/*
 * Signal handling for main process.
 */
void signal_handler (int, siginfo_t *, void *);

/* Application list. */
application APPLIST[MAX_NUMBER_APPLICATIONS];
/* Number of applications in the system. */
unsigned int number_of_applications = 0;

/* Running applications. */
application *LIVEAPPS[MAX_NUMBER_LIVE_APPLICATIONS];
/* Number of running applications. */
unsigned int number_of_live_applications = 0;

/* Process id of a appman login. */
pid_t appman_login_pid = 0;
/* Process id of a appman view. */
pid_t appman_view_pid = 0;

#endif  /* not defined _APPMAND_H_ */

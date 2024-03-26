#include "err.h"
#include "utils.h"
#include <ctype.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAX_N_TASKS 4096
#define BUFFER_SIZE 512
#define LINE_SIZE 1024

pthread_t thread[MAX_N_TASKS];
int n_tasks = 0;

struct Task {
    char* out;
    char* err;
    int pid;
    int pipe_out[2];
    int pipe_err[2];
    sem_t mutex_out;
    sem_t mutex_err;
    pthread_t pthread_out;
    pthread_t pthread_err;
    bool is_running;
    sem_t mutex_is_running;
};

void* reading_out(void* arg)
{
    struct Task* task = (struct Task*)arg;
    FILE* read_stream = fdopen(task->pipe_out[0], "r");
    char buffer[LINE_SIZE];
    while (read_line(buffer, LINE_SIZE, read_stream)) {
        ASSERT_SYS_OK(sem_wait(&task->mutex_is_running));
        if (task->is_running == false) {
            ASSERT_SYS_OK(sem_post(&task->mutex_is_running));
            break;
        }
        ASSERT_SYS_OK(sem_post(&task->mutex_is_running));
        if (buffer[strlen(buffer) - 1] == '\n') {
            buffer[strlen(buffer) - 1] = '\0';
        }
        ASSERT_SYS_OK(sem_wait(&task->mutex_out));
        free(task->out);
        task->out = calloc(LINE_SIZE, sizeof(char));
        memcpy(task->out, buffer, LINE_SIZE);
        ASSERT_SYS_OK(sem_post(&task->mutex_out));
    }
    ASSERT_SYS_OK(close(task->pipe_out[0]));
    fclose(read_stream);
    return NULL;
}

void* reading_err(void* arg)
{
    struct Task* task = (struct Task*)arg;
    FILE* read_stream = fdopen(task->pipe_err[0], "r");
    char buffer[LINE_SIZE];
    while (read_line(buffer, LINE_SIZE, read_stream)) {
        ASSERT_SYS_OK(sem_wait(&task->mutex_is_running));
        if (task->is_running == false) {
            ASSERT_SYS_OK(sem_post(&task->mutex_is_running));
            break;
        }
        ASSERT_SYS_OK(sem_post(&task->mutex_is_running));
        if (buffer[strlen(buffer) - 1] == '\n') {
            buffer[strlen(buffer) - 1] = '\0';
        }
        ASSERT_SYS_OK(sem_wait(&task->mutex_err));
        free(task->err);
        task->err = calloc(LINE_SIZE, sizeof(char));
        memcpy(task->err, buffer, LINE_SIZE);
        ASSERT_SYS_OK(sem_post(&task->mutex_err));
    }
    ASSERT_SYS_OK(close(task->pipe_err[0]));
    fclose(read_stream);
    return NULL;
}

struct Task tasks[MAX_N_TASKS];
pthread_mutex_t mutex;
sem_t mutex_run;

void* run(void* args)
{
    char** argv = (char**)args;

    ASSERT_SYS_OK(pthread_mutex_lock(&mutex));
    int my_task = n_tasks;
    n_tasks++;
    ASSERT_SYS_OK(pthread_mutex_unlock(&mutex));

    ASSERT_SYS_OK(pipe(tasks[my_task].pipe_out));
    ASSERT_SYS_OK(pipe(tasks[my_task].pipe_err));

    int fd_out[2];
    ASSERT_SYS_OK(pipe(fd_out));

    int pid = fork();
    ASSERT_SYS_OK(pid);

    if (pid == 0) {
        ASSERT_SYS_OK(close(tasks[my_task].pipe_out[0]));
        ASSERT_SYS_OK(close(tasks[my_task].pipe_err[0]));
        ASSERT_SYS_OK(close(fd_out[0]));

        ASSERT_SYS_OK(dup2(tasks[my_task].pipe_out[1], STDOUT_FILENO));
        ASSERT_SYS_OK(close(tasks[my_task].pipe_out[1]));

        ASSERT_SYS_OK(dup2(tasks[my_task].pipe_err[1], STDERR_FILENO));
        ASSERT_SYS_OK(close(tasks[my_task].pipe_err[1]));

        write(fd_out[1], "starting", sizeof("zaczynam"));

        ASSERT_SYS_OK(execvp(argv[1], &argv[1]));
    } else {
        ASSERT_SYS_OK(close(fd_out[1]));

        char buffer[LINE_SIZE];
        ASSERT_SYS_OK(read(fd_out[0], buffer, sizeof(buffer)));
        printf("Task %d started: pid %d.\n", my_task, pid);
        ASSERT_SYS_OK(close(fd_out[0]));

        tasks[my_task].pid = pid;

        ASSERT_SYS_OK(sem_wait(&tasks[my_task].mutex_is_running));
        tasks[my_task].is_running = true;
        ASSERT_SYS_OK(sem_post(&tasks[my_task].mutex_is_running));

        ASSERT_SYS_OK(close(tasks[my_task].pipe_out[1]));
        ASSERT_SYS_OK(close(tasks[my_task].pipe_err[1]));
        set_close_on_exec(tasks[my_task].pipe_out[0], 1);
        set_close_on_exec(tasks[my_task].pipe_err[0], 1);

        ASSERT_SYS_OK(sem_post(&mutex_run));

        pthread_t* thread_out = &tasks[my_task].pthread_out;
        pthread_t* thread_err = &tasks[my_task].pthread_err;

        ASSERT_SYS_OK(pthread_mutex_lock(&mutex));
        ASSERT_SYS_OK(pthread_create(thread_out, NULL, reading_out, &tasks[my_task]));
        ASSERT_SYS_OK(pthread_create(thread_err, NULL, reading_err, &tasks[my_task]));
        ASSERT_SYS_OK(pthread_mutex_unlock(&mutex));

        ASSERT_SYS_OK(pthread_join(*thread_out, NULL));
        ASSERT_SYS_OK(pthread_join(*thread_err, NULL));

        int status;
        waitpid(pid, &status, 0);
        ASSERT_SYS_OK(sem_wait(&mutex_run));
        if (WIFEXITED(status)) {
            printf("Task %d ended: status %d.\n", my_task, WEXITSTATUS(status));
        } else {
            printf("Task %d ended: signalled.\n", my_task);
        }
        ASSERT_SYS_OK(sem_post(&mutex_run));
        ASSERT_SYS_OK(sem_wait(&tasks[my_task].mutex_is_running));
        tasks[my_task].is_running = false;
        ASSERT_SYS_OK(sem_post(&tasks[my_task].mutex_is_running));
    }
    return NULL;
}

void out2(char** argv)
{
    int my_task = atoi(argv[1]);
    ASSERT_SYS_OK(sem_wait(&tasks[my_task].mutex_out));
    printf("Task %d stdout: '%s'.\n", my_task, tasks[my_task].out);
    ASSERT_SYS_OK(sem_post(&tasks[my_task].mutex_out));
}

void err2(char** argv)
{
    int my_task = atoi(argv[1]);
    ASSERT_SYS_OK(sem_wait(&tasks[my_task].mutex_err));
    printf("Task %d stderr: '%s'.\n", my_task, tasks[my_task].err);
    ASSERT_SYS_OK(sem_post(&tasks[my_task].mutex_err));
}

void kill2(char** argv)
{
    int my_task = atoi(argv[1]);
    ASSERT_SYS_OK(sem_wait(&tasks[my_task].mutex_is_running));
    if (tasks[my_task].is_running) {
        ASSERT_SYS_OK(kill(tasks[my_task].pid, SIGINT));
    }
    ASSERT_SYS_OK(sem_post(&tasks[my_task].mutex_is_running));
}

void quit2()
{
    for (int i = 0; i < n_tasks; i++) {
        ASSERT_SYS_OK(sem_wait(&tasks[i].mutex_is_running));
        if (tasks[i].is_running) {
            tasks[i].is_running = false;
            ASSERT_SYS_OK(sem_post(&tasks[i].mutex_is_running));
            ASSERT_SYS_OK(kill(tasks[i].pid, SIGKILL));
        } else {
            ASSERT_SYS_OK(sem_post(&tasks[i].mutex_is_running));
        }
    }
}

char** parse_line(char* line)
{
    if (line[strlen(line) - 1] == '\n') {
        line[strlen(line) - 1] = '\0';
    }
    char** args = split_string(line);

    int p = 0;
    for (int i = 0; args[i] != NULL; i++) {
        p = i;
    }
    size_t l = strlen(args[p]);
    if (args[p][l - 1] == '\n') {
        args[p][l - 1] = '\0';
    }

    return args;
}

int main()
{
    char* buffer = calloc(BUFFER_SIZE, sizeof(char));
    size_t size = BUFFER_SIZE;

    for (int i = 0; i < MAX_N_TASKS; i++) {
        tasks[i].is_running = false;
        tasks[i].out = calloc(BUFFER_SIZE, sizeof(char));
        tasks[i].err = calloc(BUFFER_SIZE, sizeof(char));
        ASSERT_SYS_OK(sem_init(&tasks[i].mutex_out, 1, 1));
        ASSERT_SYS_OK(sem_init(&tasks[i].mutex_err, 1, 1));
        ASSERT_SYS_OK(sem_init(&tasks[i].mutex_is_running, 1, 1));
    }

    ASSERT_SYS_OK(pthread_mutex_init(&mutex, NULL));
    ASSERT_SYS_OK(sem_init(&mutex_run, 1, 1));

    int i = 0;
    while (getline(&buffer, &size, stdin) != EOF) {

        if (buffer[0] == '\n') {
            continue;
        }

        char** args = parse_line(buffer);

        ASSERT_SYS_OK(sem_wait(&mutex_run));

        if (strcmp(args[0], "run") == 0) {
            ASSERT_SYS_OK(pthread_mutex_lock(&mutex));
            ASSERT_SYS_OK(pthread_create(&thread[i], NULL, run, args));
            i++;
            ASSERT_SYS_OK(pthread_mutex_unlock(&mutex));
        } else if (strcmp(args[0], "out") == 0) {
            out2(args);
            ASSERT_SYS_OK(sem_post(&mutex_run));
        } else if (strcmp(args[0], "err") == 0) {
            err2(args);
            ASSERT_SYS_OK(sem_post(&mutex_run));
        } else if (strcmp(args[0], "kill") == 0) {
            kill2(args);
            ASSERT_SYS_OK(sem_post(&mutex_run));
        } else if (strcmp(args[0], "sleep") == 0) {
            ASSERT_SYS_OK(sem_post(&mutex_run));
            usleep(atoi(args[1]) * 1000);
        } else if (strcmp(args[0], "quit") == 0) {
            quit2();
            ASSERT_SYS_OK(sem_post(&mutex_run));
        }

        ASSERT_SYS_OK(sem_wait(&mutex_run));
        free_split_string(args);
        ASSERT_SYS_OK(sem_post(&mutex_run));
    }
    ASSERT_SYS_OK(sem_wait(&mutex_run));
    quit2();
    ASSERT_SYS_OK(sem_post(&mutex_run));

    for (int i = 0; i < n_tasks; i++) {
        ASSERT_SYS_OK(pthread_join(thread[i], NULL));
    }

    for (int i = 0; i < MAX_N_TASKS; i++) {
        ASSERT_SYS_OK(sem_destroy(&tasks[i].mutex_out));
        ASSERT_SYS_OK(sem_destroy(&tasks[i].mutex_err));
        ASSERT_SYS_OK(sem_destroy(&tasks[i].mutex_is_running));
    }

    free(buffer);

    ASSERT_SYS_OK(pthread_mutex_destroy(&mutex));
    ASSERT_SYS_OK(sem_destroy(&mutex_run));
    return 0;
}
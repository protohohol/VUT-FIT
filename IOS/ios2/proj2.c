#include "proj2.h"

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#define MUTEX_NAME "/xtiemn00-ios2-mutex"
#define PRINT_Q_NAME "/xtiemn00-ios2-print_q"
#define MUTEX_QUEUE_NAME "/xtiemn00-ios2-mutex_queue"
#define OXYGEN_Q_SEM_NAME "/xtiemn00-ios2-oxygen_q_sem"
#define HYDROGEN_Q_SEM_NAME "/xtiemn00-ios2-hyderogen_q_sem"
#define OXYGEN_CREATING_SEM_NAME "/xtiemn00-ios2-oxygen_creating_sem"
#define HYDROGEN_CREATING_SEM_NAME "/xtiemn00-ios2-hydrogen_creating_sem"
#define MOLECULE_CREATING_SEM "/xtiemn00-ios2-molecule_creating_sem"
#define MOLECULE_CREATED_SEM "/xtiemn00-ios2-molecule_created_sem"
#define OXYGEN_END_SEM "/xtiemn00-ios2-oxygen_end_sem"
#define HYDROGEN_END_SEM "/xtiemn00-ios2-hydrogen_end_sem"

int* end_f;
long molecule_amount;
long oxygen_sum;
long hydrogen_sum;
long* oxygen_queue;
long* hydrogen_queue;
long* oxygen_count;
long* hydrogen_count;
long* line_count;
long* molecule_count;
FILE* f;
sem_t* mutex;
sem_t* print_q;
sem_t* mutex_queue;
sem_t* oxygen_q_sem;
sem_t* hydrogen_q_sem;
sem_t* oxygen_creating_sem;
sem_t* hydrogen_creating_sem;
sem_t* molecule_creating_sem;
sem_t* molecule_created_sem;
sem_t* oxygen_end_sem;
sem_t* hydrogen_end_sem;

// void exitfunc(int sig)
// {
//     fprintf(stderr, "Process timed out!\n");
//     cleanup();
//     _exit(1);
// }

void cleanup() {
    sem_close(mutex);
    sem_unlink(MUTEX_NAME);
    sem_close(print_q);
    sem_unlink(PRINT_Q_NAME);
    sem_close(mutex_queue);
    sem_unlink(MUTEX_QUEUE_NAME);
    sem_close(oxygen_q_sem);
    sem_unlink(OXYGEN_Q_SEM_NAME);
    sem_close(hydrogen_q_sem);
    sem_unlink(HYDROGEN_Q_SEM_NAME);
    sem_close(oxygen_creating_sem);
    sem_unlink(OXYGEN_CREATING_SEM_NAME);
    sem_close(hydrogen_creating_sem);
    sem_unlink(HYDROGEN_CREATING_SEM_NAME);
    sem_close(molecule_creating_sem);
    sem_unlink(MOLECULE_CREATING_SEM);
    sem_close(molecule_created_sem);
    sem_unlink(MOLECULE_CREATED_SEM);
    sem_close(oxygen_end_sem);
    sem_unlink(OXYGEN_END_SEM);
    sem_close(hydrogen_end_sem);
    sem_unlink(HYDROGEN_END_SEM);

    munmap(end_f, sizeof(end_f));
    munmap(oxygen_queue, sizeof(oxygen_queue));
    munmap(hydrogen_queue, sizeof(hydrogen_queue));
    munmap(oxygen_count, sizeof(oxygen_count));
    munmap(hydrogen_count, sizeof(hydrogen_count));
    munmap(line_count, sizeof(line_count));
    munmap(molecule_count, sizeof(molecule_count));
    munmap(mutex, sizeof(mutex));
    munmap(print_q, sizeof(print_q));
    munmap(mutex_queue, sizeof(mutex_queue));
    munmap(oxygen_q_sem, sizeof(oxygen_q_sem));
    munmap(hydrogen_q_sem, sizeof(hydrogen_q_sem));
    munmap(oxygen_creating_sem, sizeof(oxygen_creating_sem));
    munmap(hydrogen_creating_sem, sizeof(hydrogen_creating_sem));
    munmap(molecule_creating_sem, sizeof(molecule_creating_sem));
    munmap(molecule_created_sem, sizeof(molecule_created_sem));
    munmap(oxygen_end_sem, sizeof(oxygen_end_sem));
    munmap(hydrogen_end_sem, sizeof(hydrogen_end_sem));

    if(fclose(f) == EOF)
	{
		fprintf(stderr,"Cannot close file!\n");
		exit(1);
	}
}

int semaphore_init() {
    sem_unlink(MUTEX_NAME);
    sem_unlink(PRINT_Q_NAME);
    sem_unlink(MUTEX_QUEUE_NAME);
    sem_unlink(OXYGEN_Q_SEM_NAME);
    sem_unlink(HYDROGEN_Q_SEM_NAME);
    sem_unlink(OXYGEN_CREATING_SEM_NAME);
    sem_unlink(HYDROGEN_CREATING_SEM_NAME);
    sem_unlink(MOLECULE_CREATING_SEM);
    sem_unlink(MOLECULE_CREATED_SEM);
    sem_unlink(OXYGEN_END_SEM);
    sem_unlink(HYDROGEN_END_SEM);

    if ((end_f = mmap(NULL, sizeof(end_f), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) {
		fprintf(stderr, "Cannot create shared memory!\n");
		cleanup();
        return 1;
	}

    if ((oxygen_queue = mmap(NULL, sizeof(oxygen_queue), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) {
		fprintf(stderr, "Cannot create shared memory!\n");
		cleanup();
        return 1;
	}

    if ((hydrogen_queue = mmap(NULL, sizeof(hydrogen_queue), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) {
		fprintf(stderr, "Cannot create shared memory!\n");
		cleanup();
        return 1;
	}

    if ((oxygen_count = mmap(NULL, sizeof(oxygen_count), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) {
		fprintf(stderr, "Cannot create shared memory!\n");
		cleanup();
        return 1;
	}

    if ((hydrogen_count = mmap(NULL, sizeof(hydrogen_count), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) {
		fprintf(stderr, "Cannot create shared memory!\n");
		cleanup();
        return 1;
	}

    if ((line_count = mmap(NULL, sizeof(line_count), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) {
		fprintf(stderr, "Cannot create shared memory!\n");
		cleanup();
        return 1;
	}

    if ((molecule_count = mmap(NULL, sizeof(molecule_count), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) {
		fprintf(stderr, "Cannot create shared memory!\n");
		cleanup();
        return 1;
	}

    if ((mutex = mmap(NULL, sizeof(mutex), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) {
		fprintf(stderr, "Cannot create shared memory!\n");
		cleanup();
        return 1;
	}

    if ((print_q = mmap(NULL, sizeof(print_q), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) {
		fprintf(stderr, "Cannot create shared memory!\n");
		cleanup();
        return 1;
	}

    if ((mutex_queue = mmap(NULL, sizeof(mutex_queue), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) {
		fprintf(stderr, "Cannot create shared memory!\n");
		cleanup();
        return 1;
	}

    if ((oxygen_q_sem = mmap(NULL, sizeof(oxygen_q_sem), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) {
		fprintf(stderr, "Cannot create shared memory!\n");
		cleanup();
        return 1;
	}

    if ((hydrogen_q_sem = mmap(NULL, sizeof(hydrogen_q_sem), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) {
		fprintf(stderr, "Cannot create shared memory!\n");
		cleanup();
        return 1;
	}

    if ((oxygen_creating_sem = mmap(NULL, sizeof(oxygen_creating_sem), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) {
		fprintf(stderr, "Cannot create shared memory!\n");
		cleanup();
        return 1;
	}

    if ((hydrogen_creating_sem = mmap(NULL, sizeof(hydrogen_creating_sem), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) {
		fprintf(stderr, "Cannot create shared memory!\n");
		cleanup();
        return 1;
	}

    if ((molecule_creating_sem = mmap(NULL, sizeof(molecule_creating_sem), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) {
		fprintf(stderr, "Cannot create shared memory!\n");
		cleanup();
        return 1;
	}

    if ((molecule_created_sem = mmap(NULL, sizeof(molecule_created_sem), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) {
		fprintf(stderr, "Cannot create shared memory!\n");
		cleanup();
        return 1;
	}

    if ((oxygen_end_sem = mmap(NULL, sizeof(oxygen_end_sem), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) {
		fprintf(stderr, "Cannot create shared memory!\n");
		cleanup();
        return 1;
	}

    if ((hydrogen_end_sem = mmap(NULL, sizeof(hydrogen_end_sem), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) {
		fprintf(stderr, "Cannot create shared memory!\n");
		cleanup();
        return 1;
	}

    if ((mutex = sem_open(MUTEX_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
		fprintf(stderr, "Cannot create semaphore!\n");
		cleanup();
        return 1;
    }

    if ((print_q = sem_open(PRINT_Q_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
		fprintf(stderr, "Cannot create semaphore!\n");
		cleanup();
        return 1;
    }

    if ((mutex_queue = sem_open(MUTEX_QUEUE_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
		fprintf(stderr, "Cannot create semaphore!\n");
		cleanup();
        return 1;
    }

    if ((oxygen_q_sem = sem_open(OXYGEN_Q_SEM_NAME, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) {
		fprintf(stderr, "Cannot create semaphore!\n");
		cleanup();
        return 1;
    }

    if ((hydrogen_q_sem = sem_open(HYDROGEN_Q_SEM_NAME, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) {
		fprintf(stderr, "Cannot create semaphore!\n");
		cleanup();
        return 1;
    }

    if ((oxygen_creating_sem = sem_open(OXYGEN_CREATING_SEM_NAME, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) {
		fprintf(stderr, "Cannot create semaphore!\n");
		cleanup();
        return 1;
    }

    if ((hydrogen_creating_sem = sem_open(HYDROGEN_CREATING_SEM_NAME, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) {
		fprintf(stderr, "Cannot create semaphore!\n");
		cleanup();
        return 1;
    }

    if ((molecule_creating_sem = sem_open(MOLECULE_CREATING_SEM, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
		fprintf(stderr, "Cannot create semaphore!\n");
		cleanup();
        return 1;
    }

    if ((molecule_created_sem = sem_open(MOLECULE_CREATED_SEM, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) {
		fprintf(stderr, "Cannot create semaphore!\n");
		cleanup();
        return 1;
    }

    if ((oxygen_end_sem = sem_open(OXYGEN_END_SEM, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) {
		fprintf(stderr, "Cannot create semaphore!\n");
		cleanup();
        return 1;
    }

    if ((hydrogen_end_sem = sem_open(HYDROGEN_END_SEM, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) {
		fprintf(stderr, "Cannot create semaphore!\n");
		cleanup();
        return 1;
    }

    setbuf(f, NULL);

    return 0;
}

void oxygen(long TI, long TB) {
    sem_wait(mutex);
    if (oxygen_sum < 1 || hydrogen_sum < 2) {
        sem_post(oxygen_q_sem);
        (*end_f) = 0;
    }

    (*oxygen_count)++;
    long cur_oxygen = (*oxygen_count);

    sem_wait(print_q);
    (*line_count)++;
    //printf("%ld: O %ld: started\n", *line_count, cur_oxygen);
    fprintf(f, "%ld: O %ld: started\n", *line_count, cur_oxygen);
    sem_post(print_q);

    sem_post(mutex);

    usleep((rand() % (TI + 1)) * 1000);

    sem_wait(mutex_queue);
    (*oxygen_queue)++;

    sem_wait(print_q);
    (*line_count)++;
    //printf("%ld: O %ld: going to queue\n", *line_count, cur_oxygen);
    fprintf(f, "%ld: O %ld: going to queue\n", *line_count, cur_oxygen);
    sem_post(print_q);

    if (*oxygen_queue >= 1 && *hydrogen_queue >= 2) {
        sem_wait(molecule_creating_sem);
        (*oxygen_queue)--;
        (*hydrogen_queue) -= 2;
        (*molecule_count)++;

        sem_wait(print_q);
        (*line_count)++;
        //printf("%ld: O %ld: creating molecule %ld\n", *line_count, cur_oxygen, *molecule_count);
        fprintf(f, "%ld: O %ld: creating molecule %ld\n", *line_count, cur_oxygen, *molecule_count);
        sem_post(print_q);

        if (*molecule_count == molecule_amount) {
            (*end_f)++;
        }

        usleep((rand() % (TB + 1)) * 1000);

        sem_post(hydrogen_q_sem);
        sem_post(hydrogen_q_sem);

        sem_post(mutex_queue);

        sem_wait(hydrogen_creating_sem);
        sem_wait(hydrogen_creating_sem);

        sem_post(molecule_created_sem);
        sem_post(molecule_created_sem);

        sem_wait(print_q);
        (*line_count)++;
        //printf("%ld: O %ld: molecule %ld created\n", *line_count, cur_oxygen, *molecule_count);
        fprintf(f, "%ld: O %ld: molecule %ld created\n", *line_count, cur_oxygen, *molecule_count);
        sem_post(print_q);

        sem_wait(hydrogen_end_sem);
        sem_wait(hydrogen_end_sem);

        if (*molecule_count == molecule_amount) {
            (*end_f)--;

            // //printf("Hi\n");
            for (long i = 0; i < oxygen_sum - molecule_amount; i++) {
                sem_post(oxygen_q_sem);
            }

            for (long i = 0; i < hydrogen_sum - (molecule_amount * 2); i++) {
                sem_post(hydrogen_q_sem);
            }
        }

        sem_post(molecule_creating_sem);

        exit(0);
    }

    sem_post(mutex_queue);

    sem_wait(oxygen_q_sem);

    if (*molecule_count == molecule_amount && (*end_f) == 0) {
        // sem_wait(hydrogen_end_sem);
        // sem_wait(hydrogen_end_sem);

        // sem_wait(mutex);
        // if (*end_f) {
        //     (*end_f) = 0;
        //     sem_close(mutex);

        //     for (long i = 0; i < ((*oxygen_queue) - 1); i++) {
        //         sem_post(hydrogen_end_sem);
        //         sem_post(hydrogen_end_sem);
        //     }

        //     for (long i = 0; i < *hydrogen_queue; i++) {
        //         sem_post(hydrogen_end_sem);
        //         sem_post(oxygen_end_sem);
        //     }
        // }

        sem_wait(print_q);
        (*line_count)++;
        //printf("%ld: O %ld: not enough H\n", *line_count, cur_oxygen);
        fprintf(f, "%ld: O %ld: not enough H\n", *line_count, cur_oxygen);
        sem_post(print_q);

        exit(0);
    } else {
        sem_wait(print_q);
        (*line_count)++;
        //printf("%ld: O %ld: creating molecule %ld\n", *line_count, cur_oxygen, *molecule_count);
        fprintf(f, "%ld: O %ld: creating molecule %ld\n", *line_count, cur_oxygen, *molecule_count);
        sem_post(print_q);

        sem_post(oxygen_creating_sem);

        sem_wait(molecule_created_sem);

        sem_wait(print_q);
        (*line_count)++;
        //printf("%ld: O %ld: molecule %ld created\n", *line_count, cur_oxygen, *molecule_count);
        fprintf(f, "%ld: O %ld: molecule %ld created\n", *line_count, cur_oxygen, *molecule_count);
        sem_post(print_q);

        sem_post(oxygen_end_sem);

        exit(0);
    }
}

void hydrogen(long TI, long TB) {
    sem_wait(mutex);
    if (oxygen_sum < 1 || hydrogen_sum < 2) {
        sem_post(hydrogen_q_sem);
    }
    
    (*hydrogen_count)++;
    long cur_hydrogen = (*hydrogen_count);

    sem_wait(print_q);
    (*line_count)++;
    //printf("%ld: H %ld: started\n", *line_count, cur_hydrogen);
    fprintf(f, "%ld: H %ld: started\n", *line_count, cur_hydrogen);
    sem_post(print_q);

    sem_post(mutex);

    usleep((rand() % (TI + 1)) * 1000);

    sem_wait(mutex_queue);
    (*hydrogen_queue)++;

    sem_wait(print_q);
    (*line_count)++;
    //printf("%ld: H %ld: going to queue\n", *line_count, cur_hydrogen);
    fprintf(f, "%ld: H %ld: going to queue\n", *line_count, cur_hydrogen);
    sem_post(print_q);

    if (*oxygen_queue >= 1 && *hydrogen_queue >= 2) {
        sem_wait(molecule_creating_sem);
        (*oxygen_queue)--;
        (*hydrogen_queue) -= 2;
        (*molecule_count)++;
        

        sem_wait(print_q);
        (*line_count)++;
        //printf("%ld: H %ld: creating molecule %ld\n", *line_count, cur_hydrogen, *molecule_count);
        fprintf(f, "%ld: H %ld: creating molecule %ld\n", *line_count, cur_hydrogen, *molecule_count);
        sem_post(print_q);

        if (*molecule_count == molecule_amount) {
            (*end_f)++;
        }

        usleep((rand() % (TB + 1)) * 1000);

        sem_post(hydrogen_q_sem);
        sem_post(oxygen_q_sem);

        sem_post(mutex_queue);

        sem_wait(oxygen_creating_sem);
        sem_wait(hydrogen_creating_sem);

        sem_wait(print_q);
        (*line_count)++;
        //printf("%ld: H %ld: molecule %ld created\n", *line_count, cur_hydrogen, *molecule_count);
        fprintf(f, "%ld: H %ld: molecule %ld created\n", *line_count, cur_hydrogen, *molecule_count);
        sem_post(print_q);

        sem_post(molecule_created_sem);
        sem_post(molecule_created_sem);

        sem_wait(oxygen_end_sem);
        sem_wait(hydrogen_end_sem);

        if (*molecule_count == molecule_amount) {
            // //printf("Hi\n");
            (*end_f)--;

            for (long i = 0; i < oxygen_sum - molecule_amount; i++) {
                sem_post(oxygen_q_sem);
            }

            for (long i = 0; i < hydrogen_sum - (molecule_amount * 2); i++) {
                sem_post(hydrogen_q_sem);
            }
        }

        sem_post(molecule_creating_sem);

        exit(0);
    }

    sem_post(mutex_queue);

    sem_wait(hydrogen_q_sem);

    if (*molecule_count == molecule_amount && (*end_f) == 0) {
        sem_wait(print_q);
        (*line_count)++;
        //printf("%ld: H %ld: not enough O or H\n", *line_count, cur_hydrogen);
        fprintf(f, "%ld: H %ld: not enough O or H\n", *line_count, cur_hydrogen);
        sem_post(print_q);

        exit(0);
    } else {
        sem_wait(print_q);
        (*line_count)++;
        //printf("%ld: H %ld: creating molecule %ld\n", *line_count, cur_hydrogen, *molecule_count);
        fprintf(f, "%ld: H %ld: creating molecule %ld\n", *line_count, cur_hydrogen, *molecule_count);
        sem_post(print_q);

        sem_post(hydrogen_creating_sem);

        sem_wait(molecule_created_sem);

        sem_wait(print_q);
        (*line_count)++;
        //printf("%ld: H %ld: molecule %ld created\n", *line_count, cur_hydrogen, *molecule_count);
        fprintf(f, "%ld: H %ld: molecule %ld created\n", *line_count, cur_hydrogen, *molecule_count);
        sem_post(print_q);

        sem_post(hydrogen_end_sem);

        // sem_post(hydrogen_end_sem);

        // if (*molecule_count == molecule_amount) {
        //     for (long i = 0; i < *oxygen_queue; i++) {
        //         sem_post(oxygen_q_sem);
        //     }

        //     for (long i = 0; i < *hydrogen_queue; i++) {
        //         sem_post(hydrogen_q_sem);
        //     }
        // }

        exit(0);
    }
}

int main(int argc, char* argv[]) {
    if ((f = fopen("proj2.out","w")) == NULL){
        fprintf(stderr, "Error opening file!\n");
        return 1;
    }

    long NO, NH, TI, TB;

    if (argc != 5) {
        fprintf(stderr, "Wrong amount of arguments!\n");
        return 1;
    } else {
        char* e_f = NULL;
        for (int i = 1; i < argc; i++) {
            long buf; // buffer for number from argv[i]
            if ((buf = strtol(argv[i], &e_f, 0)) >= 0) {
                if (*e_f != '\0') {
                    fprintf(stderr, "Wrong argument!\n");
                    return 1;
                }
                if (buf == 0) {
                    if (strlen(argv[i]) == 0) {
                        fprintf(stderr, "Wrong argument!\n");
                        return 1;
                    }
                }

                switch(i) {
                    case 1:
                        NO = buf;
                        break;

                    case 2:
                        NH = buf;
                        break;

                    case 3:
                        if (buf <= 1000) {
                            TI = buf;
                            break;
                        } else {
                            fprintf(stderr, "Wrong argument!\n");
                            return 1;
                            break;
                        }

                    case 4:
                        if (buf <= 1000) {
                            TB = buf;
                            break;
                        } else {
                            fprintf(stderr, "Wrong argument!\n");
                            return 1;
                            break;
                        }
                }
            } else {
                    fprintf(stderr, "Wrong argument!\n");
                    return 1;
            }
        }
    }

    if (NO == 0 && NH == 0) {
        fprintf(stderr, "No atoms!\n");
        return 1;
    }

    molecule_amount = MIN(NO, (NH / 2));

    oxygen_sum = NO;
    hydrogen_sum = NH;
    //(*end_f) = 1;

    // //printf("Hi\n");

    if (semaphore_init()) {
        return 1;
    }
    (*end_f) = 0;

    // //printf("Hi\n");

    for (int i = 0; i < NO; i++) {
        int id = fork();

        if (id == 0) {
            // //printf("Hi1\n");
            oxygen(TI, TB);
        }
    }

    for (int i = 0; i < NH; i++) {
       int id = fork();

        if (id == 0) {
            // //printf("Hi2\n");
            hydrogen(TI, TB);
        }
    }

    while(wait(NULL) > 0) {
        // signal(SIGALRM, exitfunc);
        // int timeout = 150;//MAX(NO, (NH / 2)) * (MAX(TI, TB) * 2) / 1000;
        // if (!timeout) {
        //     timeout = 1;
        // }

        // alarm(timeout);
    }

    cleanup();

    return 0;
}

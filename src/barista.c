#include "stdio.h"
#include "stdlib.h"
#include "semaphore.h"
#include "string.h"
#include "pthread.h"
#include "unistd.h"
#include "time.h"
int wastage = 0;
int waiting_time = 0;
int b, k, n;
int *time_arr;
int *time_tol;
int *ind;
int *prep_time;
int done = 0;
float avg = 0;
// pthread_mutex_t barista_mutex[MAX_BARISTAS];
// sem_t customer_sem[MAX_CUSTOMERS];
int *customer_order;
char **input_str;
pthread_mutex_t *barista_mutex;
sem_t *customer_sem;

int *barista_bool;
long int c = 0;
// int* cust_check;

sem_t *barista_sem;

sem_t *lock_check;
sem_t *plus_plus;

sem_t **complete;

void *customer_thread(void *arg)
{

    int customer_id = *(int *)arg;
    // int c_arr=c;
    printf("Customer %d arrives at %ld second(s)\n", customer_id + 1, c);
    printf("\033[33mCustomer %d orders %s\033[0m\n", customer_id + 1, input_str[customer_id]);

    sem_post(complete[customer_id]);
    int barista_id = -1;
    int tm = c + time_tol[customer_id];
    int printing = 0;

    // printf("customer id: %d , time :%ld\n",customer_id,tm);
    while (barista_id == -1)
    {        
        
        if (c > tm && !printing)
        {
            waiting_time += time_tol[customer_id];
            printf("\033[31mCustomer %d leaves without their order at %ld second(s)\033[0m\n", customer_id + 1, c);
            // cust_check[customer_id]=1;
            printing = 1;
            done++;
            return NULL;
        }

        sem_wait(lock_check);
        for (int j = 0; j < b; j++)
        {
            if (c > tm && !printing)
            {
                waiting_time += time_tol[customer_id];
                printf("\033[31mCustomer %d leaves without their order at %ld second(s)\033[0m\n", customer_id + 1, c);
                // cust_check[customer_id]=1;
                printing = 1;
                done++;
                sem_post(lock_check);
                return NULL;
            }
            // pthread_mutex_lock(&barista_mutex[j]);
            if (customer_order[customer_id] == -1)
            {
                if (barista_bool[j] == 0)
                {
                    barista_bool[j] = 1;
                    customer_order[customer_id] = j;
                    barista_id = j;
                    // pthread_mutex_unlock(&barista_mutex[j]);
                    break;
                }
            }
            if (c > tm && !printing)
            {
                waiting_time += time_tol[customer_id];
                printf("\033[31mCustomer %d leaves without their order at %ld second(s)\033[0m\n", customer_id + 1, c);
                printing = 1;
                done++;
                // cust_check[customer_id]=1;
                sem_post(lock_check);
                return NULL;
            }
            // pthread_mutex_unlock(&barista_mutex[j]);
        }
        sem_post(lock_check);
    }
    return NULL;
}

void *barista_thread(void *arg)
{

    int barista_id = *(int *)arg;

    while (1)
    {
        int brkr1 = 0;
        for (int i = 0; i < n; i++)
        {
            if (customer_order[i] == barista_id)
            {
                barista_bool[barista_id] = 1;
                //     if((c-time_arr[i])>time_tol[i])
                // {
                //     printf("ACustomer %d leaves without their order at %ld second(s)\n",i + 1, c);
                //     brkr1=1;
                //     customer_order[i] = -1;
                //             // barista_bool[barista_id]=0;
                //             // sem_post(&customer_sem[i]);
                //             // pthread_mutex_unlock(&barista_mutex[barista_id]);
                // }
                int tm = c + 1;
                while (tm != c)
                    ;
                waiting_time += (c - time_arr[i]);
                printf("\033[36mBarista %d begins preparing the order of customer %d at %ld second(s)\033[0m\n", barista_id + 1, i + 1, c);

                tm = c + prep_time[i];
                int brkr = 0;
                while (tm != c)
                {
                    if ((c - time_arr[i]) > time_tol[i] && !brkr)
                    {
                        printf("\033[31mCustomer %d leaves without their order at %ld second(s)\033[0m\n", i + 1, c);
                        customer_order[i] = -1;
                        sem_wait(plus_plus);
                        wastage += 1;
                        sem_post(plus_plus);
                        // barista_bool[barista_id]=0;
                        // sem_post(&customer_sem[i]);
                        // pthread_mutex_unlock(&barista_mutex[barista_id]);
                        brkr = 1;
                        // break;
                    }
                }
                if ((c - time_arr[i]) > time_tol[i] && !brkr)
                {
                    printf("\033[31mCustomer %d leaves without their order at %ld second(s)\033[0m\n", i + 1, c);
                    customer_order[i] = -1;
                    sem_wait(plus_plus);
                    wastage++;
                    sem_post(plus_plus);
                    // barista_bool[barista_id]=0;
                    // sem_post(&customer_sem[i]);
                    // pthread_mutex_unlock(&barista_mutex[barista_id]);
                    brkr = 1;
                    // break;
                }
                printf("\033[34mBarista %d completes the order of customer %d at %ld second(s)\033[0m\n",
                       barista_id + 1, i + 1, c);
                barista_bool[barista_id] = 0;
                customer_order[i] = -1;
                // if(brkr||brkr1)
                // break;
                if (!brkr)
                    printf("\033[32mCustomer %d leaves with their order at %ld second(s)\033[0m\n", i + 1, c);
                sem_post(&customer_sem[i]);
                pthread_mutex_unlock(&barista_mutex[barista_id]);
                done++;

                break;
            }
        }
        // if(done==n)
        // return NULL;
    }

    return NULL;
}

int main()
{
    scanf("%d%d%d", &b, &k, &n);

    char **coffees = (char **)malloc(sizeof(char *) * k);
    for (int i = 0; i < k; i++)
    {
        coffees[i] = (char *)malloc(sizeof(char) * 100);
    }
    int time_coffee[k];
    for (int i = 0; i < k; i++)
    {
        scanf("%s %d", coffees[i], &time_coffee[i]);
    }

    // struct cust* cust_str=(struct cust*)malloc(sizeof(struct cust)*n);
    time_arr = (int *)malloc(sizeof(int) * n);
    time_tol = (int *)malloc(sizeof(int) * n);
    ind = (int *)malloc(sizeof(int) * n);
    input_str = (char **)malloc(sizeof(char *) * n);
    customer_order = (int *)malloc(n * sizeof(int));
    prep_time = (int *)malloc(sizeof(int) * n);
    barista_sem = (sem_t *)malloc(sizeof(sem_t) * b);
    barista_bool = (int *)malloc(sizeof(int) * b);
    // cust_check=(int*)malloc(sizeof(int)*n);
    // for(int i=0;i<n;i++)
    // cust_check[i]=0;
    for (int i = 0; i < b; i++)
        barista_bool[i] = 0;
    for (int i = 0; i < n; i++)
    {
        input_str[i] = (char *)malloc(sizeof(char) * 100);
    }
    for (int i = 0; i < n; i++)
    {
        scanf("%d %s %d %d", &ind[i], input_str[i], &time_arr[i], &time_tol[i]);
        for (int j = 0; j < k; j++)
        {
            if (strcmp(coffees[j], input_str[i]) == 0)
            {
                prep_time[i] = time_coffee[j];
                break;
            }
        }
    }

    pthread_t *baristas = (pthread_t *)malloc(b * sizeof(pthread_t));
    pthread_t *customers = (pthread_t *)malloc(n * sizeof(pthread_t));
    barista_mutex = (pthread_mutex_t *)malloc(b * sizeof(pthread_mutex_t));
    customer_sem = (sem_t *)malloc(n * sizeof(sem_t));
    complete = (sem_t **)malloc(n * sizeof(sem_t *));
    lock_check = (sem_t *)malloc(sizeof(sem_t));
    sem_init(lock_check, 0, 1);
    plus_plus = (sem_t *)malloc(sizeof(sem_t));
    sem_init(plus_plus, 0, 1);
    // pthread_t customers[n];
    // for (int i = 0; i < n; i++)
    // {
    //     pthread_create(&customers[i], NULL, customer_thread, NULL);
    // }

    for (int i = 0; i < n; i++)
    {
        sem_init(&customer_sem[i], 0, 0);
        customer_order[i] = -1;
    }

    for (int i = 0; i < b; i++)
    {
        pthread_mutex_init(&barista_mutex[i], NULL);
        int *barista_id = (int *)malloc(sizeof(int));
        *barista_id = i;
        pthread_create(&baristas[i], NULL, barista_thread, barista_id);
    }
    // sem_init(complete, 0, 0);
    while (1)
    {
        // int count_done=0;

        for (int i = 0; i < n; i++)
        {
            int *customer_id = (int *)malloc(sizeof(int));
            *customer_id = i;
            if (c == time_arr[i])
            {
                complete[i] = (sem_t *)malloc(sizeof(sem_t));
                sem_init(complete[i], 0, 0);
                pthread_create(&customers[i], NULL, customer_thread, customer_id);
                sem_wait(complete[i]);
            }
        }

        if (done == n)
        {
            break;
        }
        sleep(1);
        c++;
    }
    avg = (float)waiting_time / (float)n;
    printf("\n\n%d coffee wasted\n", wastage);
    printf("Average Waiting time of customer: %f\n", avg - 1);
}

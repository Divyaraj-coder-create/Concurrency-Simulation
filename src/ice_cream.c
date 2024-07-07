#include "stdio.h"
#include "stdlib.h"
#include "semaphore.h"
#include "string.h"
#include "pthread.h"
#include "unistd.h"
#include "time.h"
#include "stdbool.h"


int num_machines;
int *t_start;
int *t_stop;
int num_flavours;
int *t_f;
int num_topping;
int max_customers;
// int* topping_put;
int *topping_qt;
int *t_arr;
char **flavours;
char **toppings;
int *cust_ind;
int *num_icecream;
int num_cust = 0;
struct customer
{
    int index;
    int time_arrival;
    int num_icecream;
    char **icecream;
    char ***topping;
    int *num_tooping_ice;
    int num_completed;
    int first_order_id;
};

int num_orders;

struct order
{
    int index;
    int time_arrival;
    char **topping;
    char *icecream;
    int num_topping;
    int time_req;
    int order_id_cust;
    int customer_id;
};
struct order *orders;
int machines_done = 0;
int consumers_done=0;
struct customer *cust_det;

int *machine_available;
int *order_ids;

int *order_completed;
int c = 0;
pthread_t *machines;
pthread_t *customers;
pthread_t *orderss;
int *order_ing_status;
sem_t *machine_semaphore;
pthread_mutex_t *machine_mutex;
pthread_mutex_t *order_lock;
sem_t *order_semaphore;
sem_t *order_wait;
int* machine_assigned;
pthread_mutex_t mach_final,cust_final;

int next = 0;
int *order_there;

void *machine_thread(void *arg)
{
    // printf("dchc\n");
    int machine_id = *(int *)(arg);
    // printf("%d\n", machine_id);
    while (c != t_start[machine_id])
    {
        // printf("dibw\n");
    }
    printf("\033[1;35mMachine %d has started working at %d second(s)\n\033[0m", machine_id + 1, c);
    // machine_available[machine_id]=1;
    while (1)
    {
        // printf("db\n");
        if (c >= t_stop[machine_id])
                        {
                            // pthread_mutex_unlock(&order_lock[i]);
                            // sem_post(&order_semaphore[i]);
                            break;
                        }
            // printf("knewws\n");
        for (int i = 0; i < num_orders; i++)
        {
            // int ans = pthread_mutex_trylock(&order_lock[i]);
            //  if(ans==0)
            {
                if (order_there[i] == 1 && order_completed[i] == -1)
                {

                    int ans = pthread_mutex_trylock(&order_lock[i]);
                    // order_there[i]=false;
                    if (ans == 0)
                    {
                        machine_available[machine_id] = 1;
                        machine_assigned[i]=machine_id;
                        sem_post(&order_wait[i]);
                        sem_wait(&machine_semaphore[machine_id]);

                        // printf("hulala\n");
                        if (order_ids[i] == -1)
                        {
                            pthread_mutex_unlock(&order_lock[i]);
                            continue;
                        }
                        else
                        {

                            machine_available[machine_id] = 0;
                            for (int j = 0; j < orders[i].num_topping; j++)
                            {
                                int ind = -1;
                                if (orders[i].topping[j][strlen(orders[i].topping[j]) - 1] == '\n')
                                    orders[i].topping[j][strlen(orders[i].topping[j]) - 1] = '\0';
                                // orders[i].topping[j]strlen(orders[i].topping[j])
                                for (int k = 0; k < num_topping; k++)
                                {
                                    if (strcmp(toppings[k], orders[i].topping[j]) == 0)
                                    {
                                        ind = k;
                                        break;
                                    }
                                }

                                // printf("IND : %d TOPPING QT: %d\n",ind,topping_qt[ind]);
                                if (topping_qt[ind] == 0)
                                {
                                    order_ing_status[i] = 0;
                                    // return NULL;
                                    machine_available[machine_id] = 1;
                                    // pthread_mutex_unlock(&machine_mutex[machine_id]);
                                    order_completed[i]=0;
                                    sem_post(&order_semaphore[i]);
                                    // next++;
                                    int pm=c+1;
                                    while(pm!=c);
                                    break;
                                    // order not completed
                                }
                                else
                                {
                                    if (topping_qt[ind] != -1)
                                        topping_qt[ind]--;
                                }
                            }

                            // printf("Order_ing_status: %d\n",order_ing_status[i]);
                            if (order_ing_status[i] == -1)
                            {
                                order_ing_status[i] = 1;
                                int tm = c + 1;
                                if (c == orders[i].time_arrival)
                                {
                                    while (tm != c);
                                }
                                printf("\033[1;36mMachine %d starts preparing ice cream %d of customer %d at %d second(s)\n\033[0m", machine_id + 1, orders[i].order_id_cust + 1, orders[i].customer_id + 1, c);
                                tm = c + orders[i].time_req;
                                while (tm != c)
                                    ;
                                printf("\033[1;34mMachine %d completes preparing ice cream %d of customer %d at %d second(s)\n\033[0m", machine_id + 1, orders[i].order_id_cust + 1, orders[i].customer_id + 1, c);
                                machine_available[machine_id] = 1;
                                // next++;
                                order_completed[i] = 1;
                                sem_post(&order_semaphore[i]);
                                // pthread_mutex_unlock(&machine_mutex[machine_id]);
                            }
                            
                        }
                        if (c >= t_stop[machine_id])
                        {
                            pthread_mutex_unlock(&order_lock[i]);
                            // sem_post(&order_semaphore[i]);
                            break;
                        }
                        pthread_mutex_unlock(&order_lock[i]);
                    }
                }

                else
                {
                    if (c >= t_stop[machine_id])
                    {
                        // pthread_mutex_unlock(&order_lock[i]);
                        // sem_post(&order_semaphore[i]);
                        break;
                    }
                    // continue;
                }
                // pthread_mutex_unlock(&order_lock[i]);
            }

            // else
            {
                // if (c >= t_stop[machine_id])
                // {
                //     // pthread_mutex_unlock(&order_lock[i]);
                //     // sem_post(&order_semaphore[i]);
                //     break;
                // }
                // continue;
            }
            // pthread_mutex_unlock()
        }

        // if (c >= t_stop[machine_id])
        // {
        //     // sem_post(&order_semaphore[i]);
        //     break;
        // }
    }
    machine_available[machine_id] = -1;
    pthread_mutex_unlock(&machine_mutex[machine_id]);
    pthread_mutex_lock(&mach_final);
    machines_done++;
    pthread_mutex_unlock(&mach_final);
    // printf("mach: %d\n",machines_done);
    if (machines_done == num_machines)
    {
        for (int i = 0; i < num_orders; i++)
        sem_post(&order_wait[i]);
    }
    printf("\033[1;35mMachine %d has stopped working at %d seconds(s)\n\033[0m", machine_id + 1, c);
    return NULL;
}

void *order_thread(void *arg)
{

    // usleep(1000);
    int order_id = *(int *)arg;
    order_there[order_id] = 1;
    // for(int i=0;i<orders[order_id].num_topping;i++)
    // {
    //     orders[order_id].topping[i]
    // }

    int fl = -1;
    while (fl == -1)
    {

        sem_wait(&order_wait[order_id]);
        if (machines_done == num_machines)
        {
            order_completed[order_id]=-1;
            break;
        }

        int mach=machine_assigned[order_id];
        // int ans=pthread_mutex_trylock(&machine_mutex[machine_assigned[order_id]]);
        // if(ans==0)
        {
                  if ((t_stop[mach] - c) > (orders[order_id].time_req))
                {
                    // printf("chp=== %d\n",i+1);
                    order_there[order_id] = 0;
                    order_ids[order_id] = mach;
                    sem_post(&machine_semaphore[mach]);
                    fl = mach;
                    sem_wait(&order_semaphore[order_id]);
                    // pthread_mutex_unlock(&machine_mutex[mach]);
                    break;
                }
                    else
                {
                    sem_post(&machine_semaphore[mach]);
                }
        // pthread_mutex_unlock(&machine_mutex[mach]);
        }
        // else
        {
            // sem_post(&machine_semaphore[mach]);
            continue;
        }
        // printf("hell\n");
        // for (int i = 0; i < num_machines; i++)
        // {
        //     // if (ans == 0)
        //     {
        //         if (machine_available[i] == 1)
        //         {
        //     int ans = pthread_mutex_trylock(&machine_mutex[i]);
        //             // pthread_mutex_lock(&machine_mutex[i]);
        //             // printf("%d\n",c);
        //             if(ans==0)
        //             {
        //                 if ((t_stop[i] - c) >= (orders[order_id].time_req))
        //             {
        //                 // printf("chp=== %d\n",i+1);
        //                 order_there[order_id] = 0;
        //                 sem_post(&machine_semaphore[i]);
        //                 order_ids[order_id] = i;
        //                 fl = i;
        //                 sem_wait(&order_semaphore[order_id]);
        //                 break;
        //             }
        //             else
        //             {
        //                 sem_post(&machine_semaphore[i]);
        //             }
        //             pthread_mutex_unlock(&machine_mutex[i]);


        //             }

        //             else
        //             {
        //                 continue;
        //             }
        //         }
        //     }
            
        // }
    }
}

void *customer_thread(void *arg)
{
    // usleep(1000);  
    // while(tm!=c);
    int customer_id = *(int *)arg;
    // printf("Customer id : %d\n",customer_id);
    printf("Customer %d enters at %d seconds(s)\n", customer_id + 1, c);

    printf("\033[1;33mCustomer %d orders %d ice creams\n", customer_id + 1, cust_det[customer_id].num_icecream);
    for (int i = 0; i < cust_det[customer_id].num_icecream; i++)
    {

        printf("\033[1;33mIce Cream %d: %s \033[0m", i + 1, cust_det[customer_id].icecream[i]);
        for (int j = 0; j < cust_det[customer_id].num_tooping_ice[i]; j++)
        {
            printf("\033[1;33m%s \033[0m", cust_det[customer_id].topping[i][j]);
        }
        printf("\033[1;33m\n\033[0m");
    }
    // printf("%d\n",cust_det[customer_id].first_order_id);
    for (int i = 0; i < cust_det[customer_id].num_icecream; i++)
    {
        int *o_id = (int *)malloc(sizeof(int));
        *o_id = cust_det[customer_id].first_order_id + i;
        // usleep(1000);
        pthread_create(&orderss[cust_det[customer_id].first_order_id + i], NULL, order_thread, o_id);
        usleep(100);
    }

    for (int i = 0; i < cust_det[customer_id].num_icecream; i++)
    {
        pthread_join(orderss[cust_det[customer_id].first_order_id + i], NULL);
    }
    int brkr = 0;
    for (int i = 0; i < cust_det[customer_id].num_icecream; i++)
    {
        if (order_completed[i + cust_det[customer_id].first_order_id] == 0)
        {
            brkr = 1;
            printf("\033[1;31mCustomer %d was not served due to unavailability of ingredients\n\033[0m", customer_id + 1);
            break;
        }
        else if(order_completed[i + cust_det[customer_id].first_order_id] == -1)
        {
            brkr = 1;
            printf("\033[1;31mCustomer %d was not served due to unavailability of Machine\n\033[0m", customer_id + 1);
            break;
        }
    }

    if (!brkr)
    {
        printf("\033[1;32mCustomer %d has collected their order(s) and left at %d second(s)\n\033[0m", customer_id + 1, c);
    }

    // pthread_t* orders=(pthread_t*)malloc(sizeof(pthread_t)*cust_det[customer_id].num_icecream);

    // for(int i=0;i<cust_det[customer_id].num_icecream;i++)
    // {
    //     // order_ids[order_index++]=
    //     pthread_create(&orders[i], NULL, order_thread,cust_det[customer_id].);
    // }

    pthread_mutex_lock(&cust_final);
    consumers_done++;
    pthread_mutex_unlock(&cust_final);
    return NULL;
}

int main()
{

    scanf("%d%d%d%d", &num_machines, &max_customers, &num_flavours, &num_topping);
    t_start = (int *)malloc(sizeof(int) * (num_machines + 1));
    t_stop = (int *)malloc(sizeof(int) * (num_machines + 1));
    for (int i = 0; i < num_machines; i++)
    {
        scanf("%d%d", &t_start[i], &t_stop[i]);
    }
    flavours = (char **)malloc(sizeof(char *) * (num_flavours + 1));
    for (int i = 0; i < (num_flavours + 1); i++)
        flavours[i] = (char *)malloc(sizeof(char) * 100);

    toppings = (char **)malloc(sizeof(char *) * (num_topping + 1));
    for (int i = 0; i < (num_topping + 1); i++)
        toppings[i] = (char *)malloc(sizeof(char) * 100);
    t_f = (int *)malloc(sizeof(int) * (num_flavours + 1));
    // topping_put=(int*)malloc(sizeof(int)*num_topping);
    topping_qt = (int *)malloc(sizeof(int) * (num_topping + 1));
    for (int i = 0; i < num_flavours; i++)
    {
        scanf("%s %d", flavours[i], &t_f[i]);
    }

    for (int i = 0; i < (num_topping-1); i++)
    {
        scanf("%s %d", toppings[i], &topping_qt[i]);
    }
    scanf("%s %d\n",toppings[num_topping-1],&topping_qt[num_topping-1]);
    pthread_mutex_init(&mach_final,NULL);
    pthread_mutex_init(&cust_final,NULL);
    cust_det = (struct customer *)malloc(sizeof(struct customer) * (max_customers + 1));
    machine_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * (num_machines + 1));
    // for(int i=0;i<max_customers;i++)
    // {
    //     cust_det[i].icecream=(char*)malloc(sizeof(char)*)
    //     for(int j=0;j<num_icecream;j++)
    //     cust_det[i].icecream[j]=(char*)malloc(sizeof(char)*100);
    //     cust_det[i].topping=(char*)malloc(sizeof(char)*num_topping);
    //     for(int j=0;j<num_topping;j++)
    //     {
    //         cust_det[i].topping[j]=(char*)malloc(sizeof(char)*100);
    //     }
    // }
    // printf("Enter number of customers\n");

    // scanf("%d", &num_cust);
    int num_cust=0;
    char* input=(char*)malloc(sizeof(char)*100);


    

    while(1)
    {

        fgets(input,100,stdin);
        if(strlen(input)==1)
        {
            break;
        }
        if(input[strlen(input)-1]=='\n')
        input[strlen(input)-1]='\0';
        char* delim=" \t";
        char* inp=strtok(input,delim);
        if (inp != NULL)
        {
            cust_det[num_cust].index=atoi(inp);
        }
        inp=strtok(NULL,delim);
        cust_det[num_cust].time_arrival=atoi(inp);
        inp=strtok(NULL,delim);
        cust_det[num_cust].num_icecream=atoi(inp);

        cust_det[num_cust].num_tooping_ice = (int *)malloc(sizeof(int) * (cust_det[num_cust].num_icecream + 1));
        cust_det[num_cust].icecream = (char **)malloc(sizeof(char *) * (cust_det[num_cust].num_icecream + 1));
        for (int j = 0; j < (cust_det[num_cust].num_icecream + 1); j++)
            cust_det[num_cust].icecream[j] = (char *)malloc(sizeof(char) * 100);
        cust_det[num_cust].topping = (char ***)malloc(sizeof(char **) * (cust_det[num_cust].num_icecream + 1));
        cust_det[num_cust].num_completed = 0;
        for (int j = 0; j < (cust_det[num_cust].num_icecream + 1); j++)
        {
            cust_det[num_cust].topping[j] = (char **)malloc(sizeof(char *) * (num_topping + 1));
            for (int k = 0; k < (num_topping + 1); k++)
            {
                cust_det[num_cust].topping[j][k] = (char *)malloc(sizeof(char) * 100);
            }
        }


        for (int j = 0; j < cust_det[num_cust].num_icecream; j++)
        {
            char str[100];
            fgets(str, 100, stdin);
            if(str[strlen(str)-1]=='\n')
            str[strlen(str)-1]='\0';
            // printf("fibe %s",str);
            char *token = (char *)malloc(sizeof(char) * 100);
            token = strtok(str, " |\t");
            if (token != NULL)
            {
                strcpy(cust_det[num_cust].icecream[j], token);
            }
            // Process the remaining tokens (n-1 words)
            int ind = 0;
            while (token != NULL)
            {
                token = strtok(NULL, " |\t"); // Get the next token
                if (token != NULL)
                {
                    strcpy(cust_det[num_cust].topping[j][ind++], token);
                }
            }
            cust_det[num_cust].num_tooping_ice[j] = ind;
        }

        num_cust++;

    }




    // for (int i = 0; i < num_cust; i++)
    // {

    //     scanf("%d %d %d\n", &cust_det[i].index, &cust_det[i].time_arrival, &cust_det[i].num_icecream);
    //     cust_det[i].num_tooping_ice = (int *)malloc(sizeof(int) * (cust_det[i].num_icecream + 1));
    //     cust_det[i].icecream = (char **)malloc(sizeof(char *) * (cust_det[i].num_icecream + 1));
    //     for (int j = 0; j < (cust_det[i].num_icecream + 1); j++)
    //         cust_det[i].icecream[j] = (char *)malloc(sizeof(char) * 100);
    //     cust_det[i].topping = (char ***)malloc(sizeof(char **) * (cust_det[i].num_icecream + 1));
    //     cust_det[i].num_completed = 0;
    //     for (int j = 0; j < (cust_det[i].num_icecream + 1); j++)
    //     {
    //         cust_det[i].topping[j] = (char **)malloc(sizeof(char *) * (num_topping + 1));
    //         for (int k = 0; k < (num_topping + 1); k++)
    //         {
    //             cust_det[i].topping[j][k] = (char *)malloc(sizeof(char) * 100);
    //         }
    //     }
    //     for (int j = 0; j < cust_det[i].num_icecream; j++)
    //     {
    //         char str[100];
    //         fgets(str, 100, stdin);
    //         // printf("fibe %s",str);
    //         char *token = (char *)malloc(sizeof(char) * 100);
    //         token = strtok(str, " |\t");
    //         if (token != NULL)
    //         {
    //             strcpy(cust_det[i].icecream[j], token);
    //         }
    //         // Process the remaining tokens (n-1 words)
    //         int ind = 0;
    //         while (token != NULL)
    //         {
    //             token = strtok(NULL, " |\t"); // Get the next token
    //             if (token != NULL)
    //             {
    //                 strcpy(cust_det[i].topping[j][ind++], token);
    //             }
    //         }
    //         cust_det[i].num_tooping_ice[j] = ind;
    //     }
    // }

    int ind = 0;
    orders = (struct order *)malloc(sizeof(struct order) * (num_cust * num_flavours + 1));
    for (int i = 0; i < num_cust; i++)
    {
        for (int j = 0; j < cust_det[i].num_icecream; j++)
        {
            if (j == 0)
            {
                cust_det[i].first_order_id = ind;
            }
            orders[ind].index = ind;
            orders[ind].icecream = (char *)malloc(sizeof(char) * 100);
            strcpy(orders[ind].icecream, cust_det[i].icecream[j]);
            orders[ind].topping = (char **)malloc(sizeof(char *) * (cust_det[i].num_tooping_ice[j] + 1));
            for (int k = 0; k < (cust_det[i].num_tooping_ice[j] + 1); k++)
                orders[ind].topping[k] = (char *)malloc(sizeof(char) * 100);
            for (int k = 0; k < (cust_det[i].num_tooping_ice[j]); k++)
            {
                strcpy(orders[ind].topping[k], cust_det[i].topping[j][k]);
            }
            orders[ind].time_arrival = cust_det[i].time_arrival;
            orders[ind].num_topping = cust_det[i].num_tooping_ice[j];
            orders[ind].order_id_cust = j;
            orders[ind].customer_id = i;
            for (int k = 0; k < num_flavours; k++)
            {
                if (strcmp(orders[ind].icecream, flavours[k]) == 0)
                {
                    orders[ind].time_req = t_f[k];
                    break;
                }
            }
            ind++;
        }
    }
    machine_assigned=(int*)malloc(sizeof(int)*(num_orders+1));
    num_orders = ind;
    order_lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * (num_orders + 1));
    order_semaphore = (sem_t *)malloc(sizeof(sem_t) * (num_orders + 1));
    order_wait = (sem_t *)malloc(sizeof(sem_t) * (num_orders + 1));
    for (int i = 0; i < num_orders; i++)
    {
        sem_init(&order_semaphore[i], 0, 0);
        sem_init(&order_wait[i], 0, 0);
        pthread_mutex_init(&order_lock[i], NULL);
        // customer_order[i] = -1;
    }
    // for(int i=0;i<num_orders;i++)
    // {
    //     printf("Order num: %d\n",orders[i].index);
    //     printf("Ice Cream: %s\n",orders[i].icecream);
    //     printf("Toppings: ");
    //     for(int j=0;j<orders[i].num_topping;j++)
    //     {
    //         printf("%s ",orders[i].topping[j]);
    //     }

    // }

    order_there = (int *)malloc(sizeof(int) * (num_orders + 1));
    order_completed = (int *)malloc(sizeof(int) * (num_orders + 1));
    for (int i = 0; i < (num_orders + 1); i++)
    {
        order_completed[i] = -1;
        order_there[i] = 0;
    }
    // for(int i=0;i<num_cust;i++)
    // {
    //     printf("%d\n",cust_det[i].index);
    //     printf("Icecreams:\n");
    //     for(int j=0;j<cust_det[i].num_icecream;j++)
    //     {
    //         printf("%s\n",cust_det[i].icecream[j]);
    //         for(int k=0;k < cust_det[i].num_tooping_ice[j];k++)
    //         printf("%s ",cust_det[i].topping[j][k]);
    //         printf("\n");
    //     }

    // }

    machines = (pthread_t *)malloc(sizeof(pthread_t) * (num_machines + 1));
    customers = (pthread_t *)malloc(sizeof(pthread_t) * (num_cust + 1));
    machine_available = (int *)malloc(sizeof(int) * (num_machines + 1));
    machine_semaphore = (sem_t *)malloc(sizeof(sem_t) * (num_machines + 1));
    for (int i = 0; i < (num_machines + 1); i++)
        machine_available[i] = 0;
    for (int i = 0; i < num_machines; i++)
    {
        sem_init(&machine_semaphore[i], 0, 0);
        pthread_mutex_init(&machine_mutex[i], NULL);
    }
    for (int i = 0; i < num_machines; i++)
    {
        // printf("bjebc\n");
        int *machine_id = (int *)malloc(sizeof(int));
        *machine_id = i;
        pthread_create(&machines[i], NULL, machine_thread, machine_id);
    }

    order_ids = (int *)malloc(sizeof(int) * (num_orders + 1));
    for (int i = 0; i < num_orders; i++)
        order_ids[i] = -1;
    orderss = (pthread_t *)malloc(sizeof(pthread_t) * (num_orders + 1));
    order_ing_status = (int *)malloc(sizeof(int) * (num_orders + 1));
    // for(int i=0;i<)
    for (int i = 0; i < num_orders; i++)
    {
        order_ing_status[i] = -1;
        order_there[i] = 0;
    } // -1 for order ingredients sufficiency

    // machine_semaphore=(sem_t*)malloc(sizeof(sem_t)*num_machines);
    // for(int i=0;i<num_machines;i++)
    // printf("mach: %d\n",num_machines);
    while (1)
    {
        if(consumers_done==num_cust&&machines_done==num_machines)
        {
            break;
        }
        // int count_done=0;
        // printf("eifbeibdf\n");
        int inde = 0;
        for (int i = 0; i < num_cust; i++)
        {
            int *customer_id = (int *)malloc(sizeof(int));
            *customer_id = i;
            if (c == cust_det[i].time_arrival)
            {
                pthread_create(&customers[i], NULL, customer_thread, customer_id);
                usleep(1000);
                // for(int j=0;j<cust_det[i].num_icecream;j++)
                // {
                //      int* order_id = (int*)malloc(sizeof(int));
                //     *order_id = inde;
                //     pthread_create(&orderss[inde++],NULL,order_thread,order_id);
                // }
            }
        }

        // if(done==n)
        // {
        //     break;
        // }
        sleep(1);
        c++;
    }
    printf("Parlour Closed\n");
}
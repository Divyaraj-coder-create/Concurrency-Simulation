# Concurrency-Simulation


## Cafe Sim

### Implementation details:-
1. **Thread Creation:**
    - Threads are created for both baristas and customers.
    - Barista threads are initialized at the start, while customer threads are dynamically created based on their arrival time.

2. **Barista and Customer Threads:**
    - Barista threads continuously check for pending orders and prepare them.
    - Customer threads check the global timer (`c`) to determine if it's time for their arrival. If so, they place an order and interact with baristas.
    - There are total n+b threads.

3. **Order Handling:**
    - Baristas look for the lowest-indexed customer present in the cafe to take up their order.
    - A while loop with appropriate conditions ensures that orders are assigned accurately, and customers who leave before receiving their order are handled.
    - semaphores and locks are used to do same.


## Assumptions

1. **Customer Departure Time:**
    - Customers are assumed to leave the cafe if they haven't received their order by `t_tol + 1`.
    - If a customer's order is not ready at (t_arr+t_tol) , it will leave at (t_arr+t_tol+1) no matter if its order gets completed at (t_arr+t_tol+1). 

2. **Print Statements:**
    - The order of printing independent statements may not be indexed due to the parallel execution of threads.



### Question 1 : 

``` Calculate the average time a customer spends waiting for their coffee. Determine if this is more than (and how much) if the cafe had infinite baristas. Coffee prep time is not part of waiting time.```
</br>
**Solution :**</br>
 The average time a customer spends waiting for their coffee turns out to be :

 ![average_time](/avg_runtime_sc.png)
</br>
The average time we consider is the time that a customer has to wait before it's his/her turn to execute order. This can be compared to the scenario where there are infinite baristas. This scinerio is equivalent to the case where there are more baristas than the total number of ordered coffees. Say we have the following test case : </br>
```
>>  100 2 5
    Espresso 3
    Cappuccino 10
    1 Espresso 2 8
    2 Espresso 3 12
    3 Cappuccino 3 7
    4 Espresso 3 9
    5 Cappuccino 4 8
```
Th Output is :</br>
 ![average_time](/personal_test.png)
</br>
In the above case, the average time is 0. This is because when the number of baristas are more, as soon as a customer arrives, it can uniquely choose an *guaranteed available* barista. This results in the customer not having to wait at all. This is the best case scenario. </br>
Thus, the scenerio of have less baristas than the total number of customers, will lead to a higher average wait time.</br> This difference would be equal to the average time of the simulated scenerio.</br>
</br>

### Question 2 : 

``` Determine how many coffees are wasted. Print the number of coffees wasted after the simulation ends.```

**Solution :**</br>
The number of coffees wasted will be the number of customers who left the cafe without getting their coffee due to exceeding of their tolerance time. </br>
The number of coffees wasted in the given test case is 1. </br>
This will be because of the limited number of baristas. If there were more baristas, the customer would have been served before his/her tolerance time. </br>

 ![average_time](/output.png)




## Ice Cream Parlour Simulation

This program simulates the operations of an ice cream parlour, where customers place orders for various types of ice creams, and machines prepare these orders. The simulation involves multiple threads representing customers, orders, and ice cream machines, incorporating synchronization mechanisms for efficient coordination.

### Assumptions

1. **Random Choice:**
    - When multiple machines and consumers are available simultaneously, the choice between them is random.

2. **Print Statement Order:**
    - Independent statements at the same time may be printed in any order due to the parallel execution of threads.

3. **Ingredient Reservation:**
    - Ingredients are not reserved, allowing multiple orders to be processed simultaneously.

4. **Order Fulfillment:**
    - If a customer places multiple orders (e.g., order 1, 2, 3, 4), and the ingredient requirements for order 4 are not fulfilled, orders 1, 2, 3 will still be made.

### Implementation Details

1. **Thread Organization:**
    - The program manages threads for customers, orders, and ice cream machines.

2. **Order Creation:**
    - Customer threads are responsible for creating order threads within them.

3. **Customer-Order Interaction:**
    - Customers use semaphores to wait until they receive a signal from a machine that it has chosen them.
    - If a machine doesn't choose the customer, they will wait until the parlour is closed.

4. **Parlour Closure:**
    - If all machines exit, the parlour is considered closed.

#### Answer to Questions(Report)

1. We can take the following action as the parlor's reputation won't be harmed if orders are refused right away: We start by making sure that all of the customers' orders with the shortest arrival times have the ingredients available. We reserve the right to reject a customer if ingredients are not available for even a single order. We are also bookeeping the machine status for the future, so if all the ingredients are present, we will check if the machine will be available for the customer's order to be created. If the machines are unavailable, we will decline the customer; otherwise, we will take the orders and begin preparing them after updating the bookeeping we were doing.

2. Since ingrediants can be replenished we can say that we will have abundance of ingrediants at any point fo time. Hence our task is just to check if the machines are available for order prepartion.This we can do in the same way as mentioned in answer 1.

3. We can minimise the number of unserviced order by using a similar approach as mentioned in answer 1 where we are doing a book keeping of all the ingrediants and machines.


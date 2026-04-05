/*
 * Lab 4 - Task 5
 * ADT Queue Application: Fast Food Restaurant Order Management
 *
 * UNDERSTANDING THE TASK:
 * Simulate a fast food restaurant:
 * - Customer places order to waiter -> waiter adds to Queue.
 * - Cook takes order from Queue (dequeue from front), prepares, gives to waiter.
 * - Waiter serves customer in FIFO order (same sequence as ordered).
 * - Track time of order placement and fulfillment.
 * - Calculate total money earned.
 * Items: Burger(200,20min), Sandwich(250,30min), French Fries(100,10min),
 *        Fried Chicken(400,40min), Pizza(350,40min)
 *
 * HOW I WILL SOLVE IT:
 * Create a class Order with: name, price, prepTime, orderTime, servedTime.
 * Since Queue.h only supports int, we build an OrderQueue (similar to StringStack
 * in Lab2 Task4) using a plain array of Orders as backing store.
 * The ADT logic (enqueue/dequeue/isEmpty/isFull/FIFO) is fully respected.
 * Queue notation: ( Burger -> Sandwich -> Pizza -> ) front=Burger, rear=Pizza
 *
 * Simulation:
 * 1. Place orders -> enqueue into orderQueue.
 * 2. Process orders -> dequeue from front, mark servedTime = orderTime + prepTime.
 * 3. Display each order with price and timing.
 * 4. Total revenue = sum of all prices.
 *
 * FUNCTIONS REUSED: display() adapted for Order type.
 *
 * LEARNING: Queue perfectly models FIFO service systems. Class Order encapsulates
 *           order data cleanly without needing a generic container.
 *
 * SKILLS DEVELOPED: ADT application design, simulation of real-world FIFO systems.
 */

#include <iostream>
#include <string>
using namespace std;

const int MAX_ORDERS = 20;

// ---- Menu Data ----
const int MENU_SIZE = 5;
const string MENU_NAMES[]  = {"Burger", "Sandwich", "French Fries", "Fried Chicken", "Pizza"};
const int    MENU_PRICES[] = {200, 250, 100, 400, 350};
const int    MENU_TIMES[]  = {20, 30, 10, 40, 40};

// ---- Order Class ----
class Order
{
public:
    string name;
    int    price;
    int    prepTime;    // minutes to prepare
    int    orderTime;   // minute at which order was placed
    int    servedTime;  // minute at which order was served

    Order() : name(""), price(0), prepTime(0), orderTime(0), servedTime(0) {}

    Order(const string & n, int p, int t, int ot)
        : name(n), price(p), prepTime(t), orderTime(ot), servedTime(0) {}

    void display() const
    {
        cout << "  Order: " << name
             << " | Price: " << price << " PKR"
             << " | Prep Time: " << prepTime << " min"
             << " | Ordered at: minute " << orderTime
             << " | Served at:  minute " << servedTime << "\n";
    }

    bool isEmpty() const { return name == ""; }
};

// ---- Order Queue (Queue ADT for Orders) ----
struct OrderQueue
{
    Order data[MAX_ORDERS];
    int   front;
    int   rear;
    int   count;
    int   capacity;

    OrderQueue(int cap = MAX_ORDERS) : front(0), rear(-1), count(0), capacity(cap) {}

    bool isEmpty() const { return count == 0; }
    bool isFull()  const { return count == capacity; }

    void enqueue(const Order & o)
    {
        if(isFull()) { cout << "Order queue full!\n"; return; }
        rear++;
        data[rear] = o;
        count++;
    }

    Order dequeue()
    {
        if(isEmpty()) { cout << "No orders!\n"; return Order(); }
        Order o = data[front];
        front++;
        count--;
        return o;
    }

    // Display all orders in queue without removing (for listing)
    void display() const
    {
        cout << "( ";
        for(int i = front; i <= rear; i++)
            cout << data[i].name << " -> ";
        cout << ")\n";
    }
};

// ---- Lookup menu item ----
bool findMenuItem(const string & name, int & price, int & prepTime)
{
    for(int i = 0; i < MENU_SIZE; i++)
    {
        if(MENU_NAMES[i] == name)
        {
            price    = MENU_PRICES[i];
            prepTime = MENU_TIMES[i];
            return true;
        }
    }
    return false;
}

// ---- Place orders interactively ----
void placeOrders(OrderQueue * waitingQ, int & currentTime)
{
    cout << "\n=== Place Orders ===\n";
    cout << "Menu:\n";
    for(int i = 0; i < MENU_SIZE; i++)
        cout << "  " << (i+1) << ". " << MENU_NAMES[i]
             << " - " << MENU_PRICES[i] << " PKR, "
             << MENU_TIMES[i] << " min\n";

    cout << "Enter order item names (type 'done' to finish):\n";
    string item;
    while(!waitingQ->isFull())
    {
        cout << "Order: ";
        cin >> item;
        if(item == "done") break;
        int price, prepTime;
        if(findMenuItem(item, price, prepTime))
        {
            currentTime++;
            Order o(item, price, prepTime, currentTime);
            waitingQ->enqueue(o);
            cout << "  -> " << item << " added to queue at minute " << currentTime << "\n";
        }
        else
        {
            cout << "  Item not found on menu. Try: Burger, Sandwich, French Fries, Fried Chicken, Pizza\n";
        }
    }
}

// ---- Process orders (cook prepares, waiter serves) ----
void processOrders(OrderQueue * waitingQ, OrderQueue * servedQ, int & currentTime)
{
    cout << "\n=== Processing Orders ===\n";
    while(!waitingQ->isEmpty())
    {
        Order o = waitingQ->dequeue();
        currentTime += o.prepTime;
        o.servedTime = currentTime;
        cout << "  Prepared & Served: " << o.name << " at minute " << currentTime << "\n";
        servedQ->enqueue(o);
    }
}

// ---- Display served orders and calculate revenue ----
void displayReport(OrderQueue * servedQ)
{
    cout << "\n=== Order Report ===\n";
    int total = 0;
    OrderQueue temp(MAX_ORDERS);
    while(!servedQ->isEmpty())
    {
        Order o = servedQ->dequeue();
        o.display();
        total += o.price;
        temp.enqueue(o);
    }
    // Restore
    while(!temp.isEmpty()) servedQ->enqueue(temp.dequeue());
    cout << "Total Revenue: " << total << " PKR\n";
}

// ---- Demo with predefined sequence ----
void runDemo(const string names[], int n)
{
    OrderQueue * waitingQ = new OrderQueue(MAX_ORDERS);
    OrderQueue * servedQ  = new OrderQueue(MAX_ORDERS);
    int currentTime = 0;

    cout << "\n--- Demo Order Sequence ---\n";
    for(int i = 0; i < n; i++)
    {
        int price, prepTime;
        if(findMenuItem(names[i], price, prepTime))
        {
            currentTime++;
            Order o(names[i], price, prepTime, currentTime);
            waitingQ->enqueue(o);
            cout << "Customer ordered: " << names[i] << " at minute " << currentTime << "\n";
        }
    }

    cout << "\nWaiting Queue: ";
    waitingQ->display();

    processOrders(waitingQ, servedQ, currentTime);
    displayReport(servedQ);

    delete waitingQ;
    delete servedQ;
}

int main()
{
    cout << "======================================\n";
    cout << " Fast Food Restaurant Order System\n";
    cout << "======================================\n";

    // Demo 1: Burger, Sandwich, French Fries
    string seq1[] = {"Burger", "Sandwich", "French Fries"};
    runDemo(seq1, 3);

    // Demo 2: Fried Chicken, Pizza, Burger, Sandwich
    cout << "\n";
    string seq2[] = {"Fried Chicken", "Pizza", "Burger", "Sandwich"};
    runDemo(seq2, 4);

    // Interactive mode
    cout << "\n==============================\n";
    cout << " Interactive Mode\n";
    cout << "==============================\n";
    char choice;
    cout << "Run interactive mode? (y/n): ";
    cin >> choice;
    if(choice == 'y' || choice == 'Y')
    {
        OrderQueue * waitingQ = new OrderQueue(MAX_ORDERS);
        OrderQueue * servedQ  = new OrderQueue(MAX_ORDERS);
        int currentTime = 0;
        placeOrders(waitingQ, currentTime);
        cout << "\nWaiting Queue: ";
        waitingQ->display();
        processOrders(waitingQ, servedQ, currentTime);
        displayReport(servedQ);
        delete waitingQ;
        delete servedQ;
    }

    return 0;
}

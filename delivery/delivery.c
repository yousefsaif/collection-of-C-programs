#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// orders above £20 from these cities qualify for express delivery (otherwise standard)
char *ExpressDelivery[9] = {"Birmingham","Bristol","Cardiff","Edinburgh","Glasgow","Liverpool","London","Manchester","Newcastle"};

// orders from these cities qualify for standard delivery
char *StandardDelivery[27] = {"Aberdeen","Bath","Blackpool","Bradford","Brighton","Cambridge","Chester","Coventry","Derby","Durham",
                              "Exeter","Hull","Leeds","Leicester","Norwich","Nottingham","Oxford","Peterborough","Plymouth","Portsmouth",
                              "Reading","Sheffield","Southampton","Stoke","Sunderland","Swansea","York"};

// Binary search check if city qualifies for express delivery
bool checkexpress(const char *city){
    int bottom= 0;
    int mid;
    int top = 8;

    while(bottom <= top){
        mid = (bottom + top)/2;
        if (strcmp(ExpressDelivery[mid], city) == 0){
            return true ;
        } else if (strcmp(ExpressDelivery[mid], city) > 0){
            top    = mid - 1;
        } else if (strcmp(ExpressDelivery[mid], city) < 0){
            bottom = mid + 1;
        }
    }
    return false;
}

// Binary search to check if city qualifies for standard delivery
bool checkstandard(const char *city){
    int bottom= 0;
    int mid;
    int top = 26;

    while(bottom <= top){
        mid = (bottom + top)/2;
        if (strcmp(StandardDelivery[mid], city) == 0){
            return true ;
        } else if (strcmp(StandardDelivery[mid], city) > 0){
            top    = mid - 1;
        } else if (strcmp(StandardDelivery[mid], city) < 0){
            bottom = mid + 1;
        }
    }
    return false;
}

// find what type of delivery you qualify for
char *DeliveryType(const char *city, char *ordervalue) { 
   char *reply = "?";
   int n;
   n = atoi(ordervalue);
   if (n <= 0)                                 reply = "Invalid Value";          // checks if order value is invalid
   else if (checkexpress(city) && n >= 20)     reply = "Express Delivery";       // checks if order quailfies for express delivery
   else if (checkexpress(city) && n < 20)      reply = "Standard Delivery";      // checks if order quailfies for standard delivery (for express cities)
   else if (checkstandard(city))               reply = "Standard Delivery";      // checks if order quailfies for standard delivery (for standard cities)
   else                                        reply = "Delivery Unavailable";   // cities on neither list ( express or standard)
   return reply;

 }

// Increment the test number and check that two strings are equal.
int eq(int n, const char *actual, const char *expected) {
    n++;
    if (strcmp(actual, expected) != 0) {
        fprintf(stderr, "Test %d fails\n", n);
        fprintf(stderr, "result: %s\n", actual);
        fprintf(stderr, "instead of: %s\n", expected);
        exit(1);
    }
    return n;
}

void test() {
    int n = 0;

    // Tests 1 to 5 : checks Express Delivery cities
    n = eq(n, DeliveryType("London", "20"),      "Express Delivery");
    n = eq(n, DeliveryType("Bristol", "19.99"),  "Standard Delivery");
    n = eq(n, DeliveryType("Glasgow", "18"),     "Standard Delivery");
    n = eq(n, DeliveryType("Cardiff", "50"),     "Express Delivery");
    n = eq(n, DeliveryType("Birmingham", "-10"), "Invalid Value");

    // Tests 6 to 10 : checks Express Delivery cities
    n = eq(n, DeliveryType("Bath", "20"),         "Standard Delivery");
    n = eq(n, DeliveryType("Reading", "18"),      "Standard Delivery");
    n = eq(n, DeliveryType("Stoke", "36"),        "Standard Delivery");
    n = eq(n, DeliveryType("Swansea", "200"),     "Standard Delivery");
    n = eq(n, DeliveryType("York", "-15"),        "Invalid Value");

    // Tests 11 to 15 : checks cities dont dont qualify for either
    n = eq(n, DeliveryType("Dublin", "10"),       "Delivery Unavailable");
    n = eq(n, DeliveryType("Moscow", "47"),       "Delivery Unavailable");
    n = eq(n, DeliveryType("Madrid", "63"),       "Delivery Unavailable");
    n = eq(n, DeliveryType("New York", "500"),    "Delivery Unavailable");
    n = eq(n, DeliveryType("Rio", "-5"),          "Invalid Value");

    // Tests 16 to 20 : Checks for Invalid order values
    n = eq(n, DeliveryType("Dublin", "a"),        "Invalid Value");
    n = eq(n, DeliveryType("Moscow", "two"),      "Invalid Value");
    n = eq(n, DeliveryType("Madrid", "abc"),      "Invalid Value");
    n = eq(n, DeliveryType("New York", "x"),      "Invalid Value");
    n = eq(n, DeliveryType("Rio", "-"),           "Invalid Value");
    printf("Tests passed: %d\n", n);
}



int main(int n, char *args[n]) {
    setbuf(stdout, NULL);
    if (n == 1) test();
    else if (n == 3) printf("%s\n", DeliveryType(args[1], args[2]));
    else {
        fprintf(stderr, "Use: e.g  ./delivery London 20 (city total_order_value) \n");
        exit(1);
    }
    return 0;
}

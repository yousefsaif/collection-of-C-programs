//The lists provided by this module are not thread safe.
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include <stdio.h>

// structure for storing a nodes
struct node{
  struct node *previous ;
  struct node *next ;
  char item[];

};

typedef struct node node;

// The list type is opaque (declared here, and defined in list.c).
struct list{
  struct node *first, * current, *last;
  int b;
  int length; //how many nodes i've got (+1 creat new node ,-1 delete node etc.)

};
typedef struct list List;

// Create a new empty list.  The argument is the size of an item in bytes.
list *newList(int b){
  list *l = malloc(sizeof(list));
  l->b = b;
  l->first = malloc(sizeof(node));
  l->last = malloc(sizeof(node));
  l->current = l->last;
  l->first->next = l->last;
  l->last->previous = l->first;
  return l;
}

// Set the current position before the first item or after the last item, e.g.
// to begin a forward or backward traversal.
void start(list *l){
  l->current = l->last; //- 1;
}
void end(list *l){
  l->current = l->last; //+ 1;
}

// Check whether the current position is at the start or end, e.g. to test
// whether a traversal has finished.
bool atStart(list *l){
  return l->current == l->first->next;
}
bool atEnd(list *l){
  return l->current == l->last; //+ 1;
}

// Move the current position one place forwards or backwards.  It is an error
// to call forward when at the end of the list, or backward when at the start.
void forward(list *l){
  if(l->current != l->last ){
    l->current = l->current->next; // +1
  }
  else{
    printf("error in forward\n");
    exit(1);
  }
}

void backward(list *l){
  if(l->current != l->first ){
    l->current = l->current->previous; // -1
  }
  else{
    printf("error in  backward\n");
    exit(1);
  }
}

// Insert an item before or after the current position (i.e. at the current
// position, but with the current position ending up after or before the new
// item).  The second argument is a pointer to the item to be copied.
void insertBefore(list *l, void *p){
  node *new = malloc(sizeof(node) + l->b);
  node *before = l->current->previous;
  memcpy(new->item,p, l->b);
  new->next = l->current;
  new->previous = before;
  before->next = new;
  l->current->previous = new;
  l->length ++;

}
void insertAfter(list *l, void *p){
  node *new = malloc(sizeof(node) + l->b);
  node *before = l->current->previous;

  memcpy(new->item,p, l->b);
  new->next = l->current;
  new->previous = before;
  before->next = new;
  l->current->previous = new;
  l->current = new;
  l->length ++;
}

// Get the item before the current position or after the current position.
// The second argument is a pointer to a place to copy the item into. It is an
// error to call getBefore when at the start, or getAfter when at the end.
void getBefore(list *l, void *p){
  memcpy(p, l->current->previous->item, l->b);
}
void getAfter(list *l, void *p){
  //memcpy(l->current->next,p,l->b);
  memcpy(p,l->current->item, l->b);
}

// Set the item before the current position or after the current position.
// The second argument is a pointer to the new item value. It is an error to
// call setBefore when at the start, or setAfter when at the end.
void setBefore(list *l, void *p){
  if(!atStart(l)){
    deleteBefore(l);
    insertBefore(l, p);
  }
  else{
    printf("error: setBefore at start\n");
    exit(1);
  }
}
void setAfter(list *l, void *p){
  if(!atEnd(l)){
    deleteAfter(l);
    insertAfter(l, p);
  }
  else{
    printf("error: setBefore at start\n");
    exit(1);
  }
}

// Delete the item before or after the current position. It is an error to call
// deleteBefore when at the start, or deleteAfter when at the end.
void deleteBefore(list *l){
  if(!atStart(l)){
  node *before = l->current->previous;

  l->current->previous = before->previous;
  before->previous->next = l->current;
  l->length --;
  free(before);
}
  else{
    printf("error in deleteBefore: your at the start of the list\n");
    exit(1);
 }
}
void deleteAfter(list *l){
  if(!atEnd(l)){
  forward(l);
  deleteBefore(l);
 }
  else{
    printf("error in deleteAfter: your at the end of the list \n");
    exit(1);
 }
}

//------------------------------------------------------------------------------
// Testing below
//------------------------------------------------------------------------------

// Constants representing types.
enum type { CHAR, INT, BOOL };

// Check that two ints, chars or bools are equal
int eq(enum type t, int x, int y) {
    static int n = 0;
    n++;
    if (x != y) {
        if (t==CHAR) fprintf(stderr, "Test %d gives %c not %c", n, x, y);
        if (t==INT) fprintf(stderr, "Test %d gives %d not %d", n, x, y);
        if (t==BOOL && x) fprintf(stderr, "Test %d gives true not false", n);
        if (t==BOOL && y) fprintf(stderr, "Test %d gives false not true", n);
        exit(1);
    }
    return n;
}

// More specific versions of the eq function
int eqc(char x, char y) { return eq(CHAR, x, y); }
int eqi(int x, int y) { return eq(INT, x, y); }
int eqb(bool x, bool y) { return eq(BOOL, x, y); }


// Unit testing.  Test the module, returning the number of tests passed.  If a
// test fails, print out a message and stop the program.

int testLists(list *l){
  //Test 1 to 4     -- testing start
  List *new = newList(sizeof(int));
  start(new);
  eqb(atStart(new),true);
  int x = 4;
  getBefore(new, &x);
  eqi(x, 0);
  x = 4;
  getAfter(new, & x);
  eqi(x, 0);
  eqi(new->length, 0);

  //Test 5 to 8     -- testing insertBefore and getBefore
  int a = 42;
  insertBefore(new, &a);
  eqi(new->length, 1);
  a=3;
  getBefore(new, &a);
  eqi(a, 42);

  int b = 15;
  insertBefore(new, &b);
  eqi(new->length, 2);
  b=3;
  getBefore(new, &a);
  eqi(a, 15);

  //Test 9 & 10     -- testing insertAfter and getAfter
  int c = 74;
  insertAfter(new, &c);
  eqi(new->length, 3);
  c = 6;
  getAfter(new, &c);
  eqi(c, 74);

  //Tests 11 & 12   -- testing backward
  int d = 3;
  backward(new);
  getAfter(new, &d);
  eqi(d, 15);
  backward(new);
  getAfter(new, &d);
  eqi(d, 42);

  //Tests 13 to 16  -- testing forward and atEnd
  forward(new);
  getBefore(new, &d);
  eqi(d, 42);
  forward(new);
  getBefore(new, &d);
  eqi(d, 15);
  forward(new);
  getBefore(new, &d);
  eqi(d, 74);
  eqb(atEnd(new), true);

  //Tests 17 to 21  -- preparing to test deleteBefore and deleteAfter
  backward(new);
  getAfter(new, &d);
  eqi(d, 74);
  getBefore(new, &d);
  eqi(d, 15);

  int e = 11;
  insertAfter(new, &e);
  eqi(new->length, 4);
  e = 0;
  getAfter(new, &e);
  eqi(e, 11);
  getBefore(new, &e);
  eqi(e, 15);          // 42 15 11 74

  //Tests 22 to 27  -- testing deleteBefore and deleteAfter
  deleteBefore(new);
  eqi(new->length, 3);
  getBefore(new, &e);  // 42 11 74
  eqi(e,42);
  getAfter(new, &e);
  eqi(e, 11);

  deleteAfter(new);
  eqi(new->length, 2);
  getAfter(new, &e);
  eqi(e, 74);
  getBefore(new, &e);
  eqi(e, 42);          // 42 74

  //Tests 28 to 34  -- testing setBefore and setAfter
  int f = 23;
  setBefore(new, &f);
  eqi(new->length, 2);
  f = 14;
  getBefore(new, &f);
  eqi(f, 23);
  backward(new);
  getBefore(new, &f);
  eqi(f, 0);           // 23 74
  forward(new);
  getAfter(new, &f);
  eqi(f, 74);

  int g = 7;
  setAfter(new, &g);   // 23 7
  eqi(new->length, 2);
  g = 2;
  getAfter(new, &g);
  eqi(g, 7);
  forward(new);
  getAfter(new, &g);
  eqi(g, 0);

  return eqi(0, 0) -1;
}

/* Name: Hailemichael Molla Gashaw
Student Number: 20190771
Assignment Number: 3*/
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"


/*-------------------------------------------------------------------
Uncomment and use the following code if you want */
#define UNIT_ARRAY_SIZE 1024
int iBucketCount = 1024;
enum {HASH_MULTIPLIER = 65599};

static int hash_function(const char *pcKey, int iBucketCount) {

/* Return a hash code for pcKey that is between 0 and iBucketCount-1,
  inclusive. Adapted from the EE209 lecture notes. */

  int i;
  unsigned int uiHash = 0U;
  for (i = 0; pcKey[i] != '\0'; i++) {
    uiHash = uiHash * (unsigned int)HASH_MULTIPLIER
          + (unsigned int)pcKey[i];
  }
  return (int)(uiHash % (unsigned int)iBucketCount);

}

struct UserInfo {
  char *name;                // customer name
  char *id;                  // customer id
  int purchase;              // purchase amount (> 0)
  struct UserInfo *next_id;  // for the hash table by id
  struct UserInfo *next_name;  // for the hash table by name
};

struct DB {
  struct UserInfo **hasht_id;   // id hash table
  struct UserInfo **hasht_name;  //name hash table
  struct UserInfo **new_hasht_id; //temporary hash tables for expansion
  struct UserInfo **new_hasht_name; // of hasht_id and hasht_name 
  int numItems;              // # of stored items, needed to determine
			     // # whether the array should be expanded
			     // # or not
};

/*---------------------------------------------------------------*/
/*This function creates a customer database by allocating memory for
two hash tables which are name(hasht_name) and id(hasht_id) hash tables. 
It returns pointer to the database created. If it fails to do so, 
it returns NULL.*/
DB_T
CreateCustomerDB(void)
{
  DB_T d;
  
  d = (DB_T) calloc(1, sizeof(struct DB));
  if (d == NULL) {
    fprintf(stderr, "Can't allocate a memory for DB_T\n");
    return NULL;
  }
  d->hasht_id = (struct UserInfo **)calloc(UNIT_ARRAY_SIZE, 
                sizeof(struct UserInfo *)) ;  
  d->hasht_name = (struct UserInfo **)calloc(UNIT_ARRAY_SIZE,
               sizeof(struct UserInfo *));
  if (d->hasht_id == NULL) {
    fprintf(stderr, "Can't allocate memory for id hash table\n");   
    free(d->hasht_id);
    free(d);
    return NULL;
  }
  if (d->hasht_name == NULL) {
    fprintf(stderr, "Can't allocate memory for name hash table\n");   
    free(d->hasht_name);
    free(d);
    return NULL;
  }
  return d;
  
}
/*--------------------------------------------------------------*/
/*This function removes the database by deallocating it's memory.
It takes the pointer to the database we want to destroy and it doesn't
return anything on success. It iteratively frees the memory occupied by
the user items untill all the memory is freed. 
If it fails, it just returns.*/
void
DestroyCustomerDB(DB_T d)
{
  /* fill out this function */
  //assert(0); 
  int b;
  if (d == NULL){
    return;
  }
  struct UserInfo *p;
  for (b = 0; b < iBucketCount; b++){
      for (p = d->hasht_id[b]; p != NULL;  p = d->hasht_id[b]){
          d->hasht_id[b] = p->next_id;
          free(p->name);
          free(p->id);
          free(p);
      }
  }
 free(d->hasht_id);
 free(d->hasht_name);
 free(d);
}
/*-----------------------------------------------------------------*/
/*This function registers new user items to the database. It takes 
pointer to the database, pointer to user id, pointer to name and 
integer value for the amount of purchase. It registers the name to 
the name hash table (hasht_name) and the id to id hash table(hasht_id)
and returns 0 for success and -1 when it fails. When the number of
users registered reaches 75% of the iBucketCount, it doubles the size of 
the hash table.*/
int
RegisterCustomer(DB_T d, const char *id,
		 const char *name, const int purchase)
{
  /* fill out this function */
  //assert(0);
  struct UserInfo *p; //for iteration through hasht_id
  struct UserInfo *r; //for iteration through hasht_name
  if ((d == NULL) || (id == NULL) || (name == NULL)){
    return (-1);
  }
  if(purchase <= 0){
    return (-1);
  }
  int id_key; //hash key of of id
  int name_key; //hash id of name

  id_key = hash_function(id, iBucketCount);

  for (p = d->hasht_id[id_key]; p != NULL; p = p->next_id) {
                                          //check if an id exists
    if((strcmp(p->id, id) == 0)){
      return (-1);
    }
  }
  name_key = hash_function(name, iBucketCount);
  for (p = d->hasht_name[name_key]; p != NULL; p = p->next_id) { 
                                   //check if a name already exists
    if((strcmp(p->name, name) == 0)){
      return (-1);
    }
  }

struct UserInfo *q = (struct UserInfo *)malloc(sizeof
                            (struct UserInfo));
  if (q == NULL){
    return -1;
  } 
  
  /*Expansion*/ 
  if((d->numItems >= (iBucketCount*0.75)) && (iBucketCount < 1048576)){
    d->new_hasht_id= (struct UserInfo **)calloc(2*iBucketCount, 
                  sizeof(struct UserInfo *));  //new hash table for id
    d->new_hasht_name = (struct UserInfo **)calloc(2*iBucketCount, 
                sizeof(struct UserInfo *)); //new hash table for name
    if((d->new_hasht_id == NULL) || (d->new_hasht_name == NULL)){
       return -1;
  }
  //putting the items to the new hash tables using new key of the
  //expanded iBucketCount
  int i;
  int new_id_key;
  int new_name_key;
  for(i=0; i < iBucketCount; i++){
   for (p = d->hasht_id[i]; p != NULL; p = r){
     new_id_key = hash_function(p->id, 2*iBucketCount);
     new_name_key = hash_function(p->name, 2*iBucketCount);
        r = p->next_id;
        d->hasht_id[i] = r;
        p->next_id = d->new_hasht_id[new_id_key];
        p->next_name = d->new_hasht_name[new_name_key];
        d->new_hasht_id[new_id_key] = p;
        d->new_hasht_name[new_name_key] = p;
       }
   }
  free(d->hasht_id);
  free(d->hasht_name);
  d->hasht_id = d->new_hasht_id;
  d->hasht_name = d->new_hasht_name;
  iBucketCount *=2;

  id_key = hash_function(id, iBucketCount);
  name_key = hash_function(name, iBucketCount);

  q->id = (char *)strdup(id);
  q->name = (char *)strdup(name);
  q->purchase = purchase;
  q->next_id = d->hasht_id[id_key];
  d->hasht_id[id_key] = q;
  q->next_name = d->hasht_name[name_key];
  d->hasht_name[name_key] = q;

 d->numItems++;
 return 0;
}

  q->id = (char *)strdup(id);
  q->name = (char *)strdup(name);
  q->purchase = purchase;
  q->next_id = d->hasht_id[id_key];
  d->hasht_id[id_key] = q;
  q->next_name = d->hasht_name[name_key];
  d->hasht_name[name_key] = q;

 d->numItems++;
 return 0;
}
/*--------------------------------------------------------------------*/
/*This function removes the registered items by using their id. It 
takes the pointer to database and pointer to id of the user we want
to unregister as arguments and returns 0 on success and -1 on failure.
It searches for the items with the given id in a given database 
and deallocates the memory occupied by the item.*/
int
UnregisterCustomerByID(DB_T d, const char *id)
{
  /* fill out this function */
  //assert(0); 
struct UserInfo *p; //for iteration through hasht_id
struct UserInfo *r; //for iteration through hasht_name
struct UserInfo *prev_id = NULL;
struct UserInfo *prev_name = NULL;
  if ((d == NULL) || (id == NULL)){ 
    return (-1);
  }
  
  int id_key;
  int name_key;

  id_key = hash_function(id, iBucketCount);
  int found = 0;

for (p = d->hasht_id[id_key]; p != NULL; prev_id = p, p = p->next_id){ 
                               //check if an id already exists
    if((strcmp(p->id, id) == 0)){
     found = 1;
     break;
    }
  }
  if (found == 0){   //id doesnot exist
    return -1;
  }

name_key = hash_function(p->name, iBucketCount);
r = d->hasht_name[name_key];

while ( r != p){ //to make r = p
 prev_name = r;
 r = r->next_name;
}
if(prev_id == NULL){ //case where p is the first node
  p = d->hasht_id[id_key];
  d->hasht_id[id_key] = p->next_id;

}
else {
  prev_id->next_id = p->next_id;

}
if(prev_name == NULL){ //case where r is the first node
  r = d->hasht_name[name_key];
  d->hasht_name[name_key] = r->next_name;

}
else {
  prev_name->next_name = r->next_name;
 
}
 free(p->id);
 free(p->name);
 free(p);
 d->numItems--;
 return 0;
}
  

/*--------------------------------------------------------------------*/
/*This function removes the registered items by using their name. It 
takes the pointer to database and pointer to the name of the user we 
want to unregister as arguments and returns 0 on success and -1 on 
failure. It searches for the items with the given id in a given 
database and deallocates the memory occupied by the item.*/
int
UnregisterCustomerByName(DB_T d, const char *name)
{
  /* fill out this function */
  //assert(0); 
struct UserInfo *p;
struct UserInfo *r;
struct UserInfo *prev_id = NULL;
struct UserInfo *prev_name = NULL;
  if ((d == NULL) || (name == NULL)){ 
    return (-1);
  }
  
  int id_key;
  int name_key;

  name_key = hash_function(name, iBucketCount);
  int found = 0;

for (p = d->hasht_name[name_key]; p != NULL; prev_name = p, 
                p = p->next_name){ //check if a name already exists                                 
    if((strcmp(p->name, name) == 0)){
     found = 1;
     break;
    }
  }
  if (found == 0){   //name doesnot exist
    return -1;
  }

id_key = hash_function(p->id, iBucketCount);
r = d->hasht_id[id_key];
while (r != p){ //to make r = p
    prev_id = r;
    r = r->next_id;
}
if(prev_name == NULL){ //case where p is the first node
  p = d->hasht_name[name_key];
  d->hasht_name[name_key] = p->next_name;

}
else {
  prev_name->next_name = p->next_name;

}
if(prev_id == NULL){ //case where r is the first node
  r = d->hasht_id[id_key];
  d->hasht_id[id_key] = r->next_id;

}
else {
  prev_id->next_id = r->next_id;
 
}
 free(p->name);
 free(p->id);
 free(p);
 d->numItems--;
 return 0;
}

/*-----------------------------------------------------------------*/
/*This function takes pointer to database and pointer to id as 
arguments and returns the amount of purchase on success and 
-1 on failure. It searches for the item with the given id and 
returns the amount of purchase by the respective user.*/
int
GetPurchaseByID(DB_T d, const char* id)
{
  /* fill out this function */
  //assert(0); /*
  struct UserInfo *p;
  if ((d == NULL) || (id == NULL)){ 
    return (-1);
  }
  
  int id_key; //hash key for id
  int pur; //the ourchase amount

  id_key = hash_function(id, iBucketCount);

  for (p = d->hasht_id[id_key]; p != NULL; p = p->next_id){ 
                            //check if an id already exists
    if((strcmp(p->id, id) == 0)){
     pur = p->purchase;
     return pur;
    }
  }
 return -1;
}
  

/*------------------------------------------------------------------*/
/*This function takes pointer to database and pointer to name as 
arguments and returns the amount of purchase on success and 
-1 on failure. It searches for the item with the given name and 
returns the amount of purchase by the respective user.*/
int
GetPurchaseByName(DB_T d, const char* name)
{
  /* fill out this function */
  //assert(0); 
  struct UserInfo *p;
  if ((d == NULL) || (name == NULL)){ 
    return (-1);
  }
  
  int name_key; //hash key for name
  int pur;  // the purchase amount

  name_key = hash_function(name, iBucketCount);

  for (p = d->hasht_name[name_key]; p != NULL; p = p->next_name) { 
                                  //check if a name already exists
    if((strcmp(p->name, name) == 0)){
     pur = p->purchase;
     return pur;
    }
  }
 return -1;
}
/*----------------------------------------------------------------*/
/*This function takes pointer to database and a function pointer
fp that takes user id, name and purchase as parametres. On success, 
the function returns the sum of the numbers returned by fp. 
If it fails to do so, it returns -1.*/
int
GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)
{
  /* fill out this function */
  //assert(0); 
  int total_purchase = 0;
  struct UserInfo *p;
  int i = 0;
  if ((d == NULL) || (fp == NULL)){
    return (-1);
  }
 while(i < iBucketCount){
   for(p = d->hasht_id[i]; p !=NULL; p=p->next_id){
     total_purchase +=fp(p->id, p->name, p->purchase);
   }
   i++;
 }
  return total_purchase;
}

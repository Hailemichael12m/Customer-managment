/*Name: Hailemichael Molla Gashaw
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

/*--------------------------------------------------------------------
Uncomment and use the following code if you want */

#define UNIT_ARRAY_SIZE 1024

struct UserInfo {
  char *name;                // customer name
  char *id;                  // customer id
  int purchase;              // purchase amount (> 0)
};

struct DB {
  struct UserInfo *pArray;   // pointer to the array
  int curArrSize;            // current array size (max # of elements)
  int numItems;              // # of stored items, needed to determine
			     // # whether the array should be expanded
			     // # or not
};



/*------------------------------------------------------------------*/
DB_T
CreateCustomerDB(void)
{
  /* Uncomment and use the following implementation if you want */
  DB_T d;
  
  d = (DB_T) calloc(1, sizeof(struct DB));
  if (d == NULL) {
    fprintf(stderr, "Can't allocate a memory for DB_T\n");
    return NULL;
  }
  d->curArrSize = UNIT_ARRAY_SIZE; // start with 1024 elements
  d->pArray = (struct UserInfo *)calloc(d->curArrSize, 
                  sizeof(struct UserInfo));
  if (d->pArray == NULL) {
    fprintf(stderr, "Can't allocate a memory for array of size %d\n", 
              d->curArrSize);   
    free(d);
    return NULL;
  }
  return d;
  //return NULL;
}
/*-----------------------------------------------------------------*/
/*This function takes pointer to a struct database and frees all the 
memory occupied by the database. It doesn't have return value. If the 
database pointer is NULL, it just returns */

void
DestroyCustomerDB(DB_T d)
{
  /* fill out this function */
  int i = 0;
  if (d == NULL){
    return;
  }
  while (i < d->curArrSize){
    free(d->pArray[i].name);
    free(d->pArray[i].id);
    i++;
  }
  free(d->pArray);
  free(d);
}
/*-------------------------------------------------------------*/
/*This function stores a new user item with customer information
with d. It takes pointer to struct database, pointer to id, pointer
to mame and integer data type purchase amount. It checks whether the
user information already exists or not before registering. It returns 
0 on success and -1 on failure. It increments the currArrsize by 
UNIT_ARRAY_SIZE everytime it expands the array. */
int
RegisterCustomer(DB_T d, const char *id,
		 const char *name, const int purchase)
{
  /* fill out this function */
  //assert(0);
  if ((d == NULL) || (id == NULL) || (name == NULL)){ 
    return (-1);
  }
  if(purchase <= 0){
    return (-1);
  }
  int i=0;
  while(i < d->curArrSize){ //check if an item already exists
    if((d->pArray[i].id != NULL) || (d->pArray[i].name != NULL)){
      if((strcmp(d->pArray[i].name, name) == 0) || 
                (strcmp(d->pArray[i].id, id) == 0)){
       return (-1);
    }
    }
    i++;
  }
  int prev_arrsize = d->curArrSize;
  if (d->curArrSize <= d->numItems){
    d->pArray = (struct UserInfo *)realloc(d->pArray, 
        (d->curArrSize + UNIT_ARRAY_SIZE)*sizeof(struct UserInfo));
    if (d->pArray == NULL){
      return -1;
    }
    d->curArrSize +=UNIT_ARRAY_SIZE;
  }
  int k;
  for (k = prev_arrsize; k < d->curArrSize; k++){
    /*Initializing the expanded array to NULL*/
    d->pArray[k].name = NULL;
    d->pArray[k].id = NULL;
  }
                        

  int j;
  for (j = 0; j < d->curArrSize; j++){
    if((d->pArray[j].name == NULL) || (d->pArray[j].id == NULL)){
      d->pArray[j].name = (char *)strdup(name); 
      d->pArray[j].id = (char *)strdup(id);
      d->pArray[j].purchase = purchase;
      d->numItems++;
      break;   
    }
  }
  return 0;       
}
/*----------------------------------------------------------------*/
/*This function takes pointer to database and pointer to id to remove
a user item from the database. It frees the respective memory and 
returns 0 on success. If it fails to do so, it returns -1.*/
int
UnregisterCustomerByID(DB_T d, const char *id)
{
  /* fill out this function */
  //assert(0);
  if ((d == NULL) || (id == NULL)){
    return (-1);
  }
  int i = 0;
  for(i = 0; i < d->curArrSize; i++){
    if((d->pArray[i].id != NULL) || (d->pArray[i].name != NULL)){
    if((strcmp(d->pArray[i].id, id) == 0)){
      free(d->pArray[i].id);
      free(d->pArray[i].name);
      d->pArray[i].id = NULL;
      d->pArray[i].name = NULL;
      d->numItems--;
      return 0;
    }
  }
  } 
 return (-1);
}

/*--------------------------------------------------------------*/
/*This function takes pointer to database and pointer to name to remove
a user item from the database. It frees the respective memory and 
returns 0 on success. If it fails to do so, it returns -1.*/
int
UnregisterCustomerByName(DB_T d, const char *name)
{
  /* fill out this function */
  //assert(0);
  if ((d == NULL) || (name == NULL)){
    return (-1);
  }
  int i = 0;
  for(i = 0; i < d->curArrSize; i++){
    if((d->pArray[i].id != NULL) || (d->pArray[i].name != NULL)){
    if((strcmp(d->pArray[i].name, name) == 0)){
      free(d->pArray[i].id);
      free(d->pArray[i].name);
      d->pArray[i].id = NULL;
      d->pArray[i].name = NULL;
      d->numItems--;
      return 0;
      }
    }
  }
 return (-1);
}
/*----------------------------------------------------------------*/
/*This function takes pointer to database and pointer to id so that
it searches the user item and returns the purchase amount. If the 
user doesnot exist or any of the pointers is NULL, it returns -1.*/
int
GetPurchaseByID(DB_T d, const char* id)
{
  /* fill out this function */
  //assert(0);
  if ((d == NULL) || (id == NULL)){
    return (-1);
  }
  int i = 0;
  int pur;
  for(i = 0; i< d->curArrSize; i++){
    if((d->pArray[i].id != NULL) || (d->pArray[i].name != NULL)){
      if((strcmp(d->pArray[i].id, id) == 0)){
        pur = d->pArray[i].purchase;
        return pur;
      }
  }
  }
  return (-1);
}
/*---------------------------------------------------------------*/
/*This function takes pointer to database and pointer to name so that
it searches the user item and returns the purchase amount. If the 
user doesnot exist or any of the two pointers is NULL, it returns -1.*/
int
GetPurchaseByName(DB_T d, const char* name)
{
  /* fill out this function */
  //assert(0);
   if ((d == NULL) || (name == NULL)){
    return (-1);
  }
  int i = 0;
  int pur;
  for(i = 0; i < d->curArrSize; i++){
    if((d->pArray[i].id != NULL) || (d->pArray[i].name != NULL)){
    if((strcmp(d->pArray[i].name, name) == 0)){
      pur = d->pArray[i].purchase;
      return pur;
      }
  }
  }
  return (-1);
}
/*----------------------------------------------------------------*/
int
GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)
/*This function takes pointer to database and a function pointer
fp that takes user id, name and purchase as parametres. The function
returns the sum of the numbers returned by fp on success. If it fails
to do so, it returns -1.*/

{
  /* fill out this function */
  
  int total = 0;
  int i = 0;
  if ((d == NULL) || (fp == NULL)){
    return (-1);
  }
 while(i < d->curArrSize){
  if((d->pArray[i].id != NULL) || (d->pArray[i].name != NULL)){
  total +=fp(d->pArray[i].id,d->pArray[i].name, d->pArray[i].purchase);
   }
   i++;
 }
  return total;
}

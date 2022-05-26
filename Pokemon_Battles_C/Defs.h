#ifndef _DEFS_H_
#define _DEFS_H_

typedef enum e_status { success, failure, exist, not_exist } status;
typedef enum equals_e { bigger, smaller, equal} equals;

typedef enum e_bool { false,true } bool;


typedef void * element;
typedef element(*copyFunction) (element);
typedef status(*freeFunction) (element);
typedef status(*printFunction) (element);
typedef int(*equalFunction) (element, element);//1 first bigger, 0 - same, -1 second bigger
typedef char*(*getCategoryFunction)(element);
typedef int(*getAttackFunction)(element firstElem ,element secondElem ,int* attackFirst,int* attackSecond);// return the first attack - second attack . also produce the attack of each element, The function transmits the information through the pointers received in the function.



#endif // _DEFS_H_
/**
* @file hash.h
* @brief Simple hashing algorithm library
*
* @date 2020 Apr 11
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#ifndef HASH_H_
#define HASH_H_

#include "main.h"

/*******************************Definitions***********************************/
#define MAX_TABLE_SIZE                  31   // Should choose prime numbe for better hashing

/** Hash data type, support 8-bit constant array */
typedef struct
{
    const uint8_t* table[MAX_TABLE_SIZE];       /** Hash table */
    uint32_t load;                              /** Current hash load */
    uint32_t current_index;                     /** Current traversing hash index */
} Hash_t;

/****************************Function prototypes******************************/
uint32_t Hash_function(const uint8_t* pdata);
bool Hash_Add(Hash_t* hash, const uint8_t* pdata);
bool Hash_Remove(Hash_t* hash, const uint8_t* pdata);
int Hash_Search(Hash_t* hash, const uint8_t* pdata);

/*****************************************************************************/
#endif /* HASH_H_ */

/* End of hash.h */

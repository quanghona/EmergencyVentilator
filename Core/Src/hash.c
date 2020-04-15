/**
* @file hash.c
* @brief Simple hashing algorithm library
*
* @date 2020 Apr 11
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#include "hash.h"

/*************************Private function prototypes*************************/

static uint32_t Hash_function(const uint8_t* pdata);
static uint32_t Hash_CollisionResolution(uint32_t hash_val);

/****************************Function definitions*****************************/

/******************************************************************************
 * @brief Default hash function
 * 
 * @param pdata pointer to data
 * @return uint32_t hash value
*****************************************************************************/
static inline uint32_t Hash_function(const uint8_t* pdata)
{
    return (uint32_t)pdata % MAX_TABLE_SIZE;    // Modulo hashing
}

/******************************************************************************
 * @brief Collision resolution function
 * 
 * @param hash_val hash value
 * @return uint32_t new value
*****************************************************************************/
static inline uint32_t Hash_CollisionResolution(uint32_t hash_val)
{
    return (hash_val + 10) % MAX_TABLE_SIZE;    // Use linear probing
}

/******************************************************************************
 * @brief Add element to hash table
 * 
 * @param hash Hash object
 * @param pdata data to be added
 * @return true if data sucessfully added to hash table
 * @return false if load has been full
*****************************************************************************/
bool Hash_Add(Hash_t* hash, const uint8_t* pdata)
{
    uint32_t hash_value;

    if (hash->load >= MAX_TABLE_SIZE) return false;

    /* Perform hashing */
    hash_value = Hash_function(pdata);

    /* Collisions resolution */
    while (hash->table[hash_value] != NULL)
    {
        if (hash->table[hash_value] != pdata)
        {
            hash_value = Hash_CollisionResolution(hash_value);
        }
        else
        {
            return true;
        }
    }
    hash->table[hash_value] = pdata;
    hash->load++;

    return true;
}

/******************************************************************************
 * @brief Remove element from hash table
 * 
 * @param hash hash object
 * @param pdata pointer to data
 * @return true if data was removed
 * @return false if data not exist in the hash table
*****************************************************************************/
bool Hash_Remove(Hash_t* hash, const uint8_t* pdata)
{
    int index = Hash_Search(hash, pdata);
    
    if (index >= 0)
    {
        hash->table[index] = NULL;
        hash->load--;
        return true;
    }
    else
    {
        return false;
    }
}

/******************************************************************************
 * @brief Search for an element in the hash table
 * 
 * @param hash hash object
 * @param pdata data that need to look up
 * @return int index of the element in the hash table
*****************************************************************************/
int Hash_Search(Hash_t* hash, const uint8_t* pdata)
{
    uint32_t hash_value;
    int traversal_index;

    if (hash->load == 0 || pdata == NULL) return -1;

    /* Perform hashing */
    hash_value = Hash_function(pdata);
    traversal_index = hash_value;

    do
    {
        if (hash->table[traversal_index] == pdata)
        {
            return traversal_index;
        }
        else
        {
            /* Resolve collisions */
            traversal_index = Hash_CollisionResolution(traversal_index);
        }
    } while (traversal_index != hash_value);

    return -1;
}

/* End of hash.c */

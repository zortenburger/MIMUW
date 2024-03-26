#ifdef __cplusplus
#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>
#include <string>
#include <unordered_map>

namespace jnp1{
extern "C" {
#else
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#endif
// Pointer to hash function.
typedef uint64_t (*hash_function_t)(uint64_t const *, size_t);

/**
 * @brief 
 * Creates hashing table with custom hash and returns it's id.
 * @param hash_function - pointer to hash_function.
 * @return unsigned long - set's id.
 */
unsigned long hash_create(hash_function_t hash_function);

/**
 * @brief 
 * Deletes hashing table of id - @param id.
 */
void hash_delete(unsigned long id);

/**
 * @brief 
 * Returns size of hash table of id - @param id. 
 */
size_t hash_size(unsigned long id);

/**
 * @brief 
 * Inserts an array @param seq of length @param size
 * to set that matches id - @param id.
 * @return true - successfully added.
 * @return false - did not add.
 */
bool hash_insert(unsigned long id, uint64_t const * seq, size_t size);

/**
 * @brief 
 * Removes an array @param seq of length @param size
 * from set that matches id - @param id.
 * @return true - successfully removed.
 * @return false - did not remove.
 */
bool hash_remove(unsigned long id, uint64_t const * seq, size_t size);

/**
 * @brief 
 *  Purges data from @param id's set.
 */
void hash_clear(unsigned long id);

/**
 * @return true - In @param id's set exists @param seq of length @param size.
 * @return false - In @param id's set does not exist @param seq of length @param size.
 */
bool hash_test(unsigned long id, uint64_t const * seq, size_t size);

#ifdef __cplusplus
}
}
#endif

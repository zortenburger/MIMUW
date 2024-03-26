/* Opis testów:
 * Group 1 - hash_test1.c, hash_test2.cc ze specyfikacji zadania;
 * Group 2 - zwykłe użycie tablic haszujących;
 * Group 3 - „niestandardowe” użycie tablic haszujących;
 * Group 4 - niepoprawne użycia tablic haszujących;
 * Group 5 - długie ciągi, dużo tablic haszujących, duże tablice haszujące;
 * Group 6 - inne sprawdzenia; wystarczy, że przejdą dwa z trzech testów.
 */

/* Włączamy najpierw "hash.h", aby sprawdzić, czy włącza wszystko,
 * co mu jest potrzebne, czyli czy poprawność programu nie zależy od
 * kolejności włączania plików nagłówkowych.
 */
#include "hash.h"

#ifdef NDEBUG
  #undef NDEBUG
#endif

#include <assert.h>

#if TEST_NUM == 501 || TEST_NUM == 502
  #include <stdlib.h>
#endif

#if TEST_NUM == 101 || TEST_NUM == 201 || TEST_NUM == 202 || \
    TEST_NUM == 203 || TEST_NUM == 301 || TEST_NUM == 302 || \
    TEST_NUM == 303 || TEST_NUM == 304 || TEST_NUM == 305 || \
    TEST_NUM == 307 || TEST_NUM == 401 || TEST_NUM == 402 || \
    TEST_NUM == 502 || TEST_NUM == 503
  #define SIZE_SEQ(seq) (sizeof seq / sizeof seq[0])
#endif

#if TEST_NUM == 101 || TEST_NUM == 307
  static uint64_t hash_function_1(uint64_t const *v, size_t n) {
    uint64_t hash = 0;
    for (size_t k = 0; k < n; ++k)
      hash ^= v[k];
    return hash;
  }

  static uint64_t hash_function_2(uint64_t const *v, size_t n) {
    uint64_t hash = 0;
    for (size_t k = 0; k < n; ++k)
      hash += v[k];
    return hash;
  }
#endif

#if TEST_NUM == 201 || TEST_NUM == 202 || TEST_NUM == 301 || TEST_NUM == 302 || \
    TEST_NUM == 305 || TEST_NUM == 402 || TEST_NUM == 501 || TEST_NUM == 502 || \
    TEST_NUM == 503
  static uint64_t hash_function_3(uint64_t const *v, size_t n) {
    uint64_t hash = n;
    for (size_t k = 0; k < n; ++k)
      hash ^= v[k] + (hash << 6) + (hash >> 2) + 0x9c7b39b79e3bcdc9;
    return hash;
  }
#endif

#if TEST_NUM == 203 || TEST_NUM == 301 || TEST_NUM == 303 || TEST_NUM == 304 || \
    TEST_NUM == 306 || TEST_NUM == 403 || TEST_NUM == 501 || TEST_NUM == 502 || \
    TEST_NUM == 503
  static uint64_t hash_function_4(uint64_t const *v, size_t n) {
    uint64_t hash = n;
    for (size_t k = 0; k < n; ++k)
      hash += v[k] + (hash << 6) + (hash >> 2) + 0x9e3bcdc99c7b39b7;
    return hash;
  }
#endif

#if TEST_NUM ==  303
  static void copy_seq(uint64_t const *src, uint64_t *dst, size_t size) {
    for (size_t k = 0; k < size; ++k)
      dst[k] = src[k];
  }
#endif

int main() {

/**
 ** GRUPA 1
 **/

// Przykładowy test 1 ze specyfikacji zadania
#if TEST_NUM == 101
  unsigned long id_1, id_2;
  uint64_t const seq_1[] = {0, 1, 3000, 150, 25};
  uint64_t const seq_2[] = {25, 3000, 150, 1, 0};
  uint64_t const seq_3[] = {-1, 0, 250};
  id_1 = hash_create(hash_function_1);
  assert(hash_size(id_1) == 0);
  assert(hash_insert(id_1, seq_1, SIZE_SEQ(seq_1)));
  assert(hash_size(id_1) == 1);
  assert(hash_insert(id_1, seq_2, SIZE_SEQ(seq_2)));
  assert(hash_size(id_1) == 2);
  assert(!hash_insert(id_1, seq_1, SIZE_SEQ(seq_1)));
  assert(hash_size(id_1) == 2);
  assert(!hash_insert(id_1, NULL, 0));
  assert(!hash_insert(id_1, NULL, 1));
  assert(!hash_insert(id_1, seq_3, 0));
  assert(hash_test(id_1, seq_2, SIZE_SEQ(seq_2)));
  assert(!hash_test(id_1, seq_3, SIZE_SEQ(seq_3)));
  assert(!hash_test(id_1, NULL, 0));
  assert(!hash_test(id_1, NULL, 1));
  assert(!hash_test(id_1, seq_1, 0));
  assert(!hash_test(id_1, seq_1, SIZE_SEQ(seq_1) - 1));
  assert(hash_remove(id_1, seq_1, SIZE_SEQ(seq_1)));
  assert(hash_size(id_1) == 1);
  assert(hash_insert(id_1, seq_3, SIZE_SEQ(seq_3)));
  assert(hash_size(id_1) == 2);
  assert(!hash_remove(id_1, seq_1, SIZE_SEQ(seq_1)));
  assert(!hash_remove(id_1, NULL, 0));
  assert(!hash_remove(id_1, NULL, 1));
  assert(!hash_remove(id_1, seq_2, 0));
  id_2 = hash_create(hash_function_2);
  assert(hash_insert(id_2, seq_2, SIZE_SEQ(seq_2)));
  assert(hash_insert(id_2, seq_3, SIZE_SEQ(seq_3)));
  hash_clear(id_1);
  assert(hash_size(id_1) == 0);
  assert(hash_remove(id_2, seq_2, SIZE_SEQ(seq_2)));
  assert(hash_size(id_2) == 1);
  assert(hash_remove(id_2, seq_3, SIZE_SEQ(seq_3)));
  assert(hash_size(id_2) == 0);
  hash_clear(id_2);
  assert(hash_size(id_2) == 0);
  hash_delete(id_1);
  assert(!hash_insert(id_1, seq_1, SIZE_SEQ(seq_1)));
  assert(!hash_remove(id_1, seq_1, SIZE_SEQ(seq_1)));
  assert(!hash_test(id_1, seq_1, SIZE_SEQ(seq_1)));
  hash_clear(id_1);
  assert(hash_size(id_1) == 0);
  hash_delete(id_1);
  hash_delete(id_2);
#endif /* TEST_NUM == 101 */

/**
 ** GRUPA 2
 **/

// Zwykłe użycie tablicy haszującej z liczbami 32-bitowymi
#if TEST_NUM == 201
  unsigned long id = hash_create(hash_function_3);
  uint64_t const seq_1[] = {0xFF25A09D, 0xFF25A09E, 0x87EF4531, 0x0017AB99};
  uint64_t const seq_2[] = {0xBF73A1BC, 0x78FE5431, 0x001099AA};
  uint64_t const seq_3[] = {0xA77EBBC3};
  assert(hash_insert(id, seq_1, SIZE_SEQ(seq_1)));
  assert(hash_insert(id, seq_2, SIZE_SEQ(seq_2)));
  assert(hash_insert(id, seq_3, SIZE_SEQ(seq_3)));
  assert(hash_size(id) == 3);
  assert(hash_test(id, seq_1, SIZE_SEQ(seq_1)));
  assert(hash_test(id, seq_2, SIZE_SEQ(seq_2)));
  assert(hash_test(id, seq_3, SIZE_SEQ(seq_3)));
  assert(hash_remove(id, seq_2, SIZE_SEQ(seq_2)));
  assert(hash_size(id) == 2);
  assert(!hash_test(id, seq_2, SIZE_SEQ(seq_2)));
  assert(!hash_remove(id, seq_2, SIZE_SEQ(seq_2)));
  assert(hash_insert(id, seq_2, SIZE_SEQ(seq_2)));
  assert(!hash_insert(id, seq_2, SIZE_SEQ(seq_2)));
  assert(hash_size(id) == 3);
  hash_clear(id);
  assert(hash_size(id) == 0);
  assert(hash_insert(id, seq_3, SIZE_SEQ(seq_3)));
  assert(hash_insert(id, seq_1, SIZE_SEQ(seq_1)));
  assert(hash_insert(id, seq_2, SIZE_SEQ(seq_2)));
  assert(hash_size(id) == 3);
  assert(hash_remove(id, seq_1, SIZE_SEQ(seq_1)));
  assert(hash_remove(id, seq_3, SIZE_SEQ(seq_3)));
  assert(hash_remove(id, seq_2, SIZE_SEQ(seq_2)));
  assert(hash_size(id) == 0);
  hash_delete(id);
#endif /* TEST_NUM == 201 */

// Zwykłe użycie tablicy haszującej z liczbami 64-bitowymi
#if TEST_NUM == 202
  unsigned long id = hash_create(hash_function_3);
  uint64_t const seq_1[] = {0xFF25A09D87EF4531, 0x0017AB99FF25A09E, 0xFF25A09D87EF4530};
  uint64_t const seq_2[] = {0xBF73A1BC001099AA, 0x001099AA78FE5431};
  assert(hash_insert(id, seq_1, SIZE_SEQ(seq_1)));
  assert(hash_insert(id, seq_2, SIZE_SEQ(seq_2)));
  assert(hash_size(id) == 2);
  assert(hash_test(id, seq_1, SIZE_SEQ(seq_1)));
  assert(hash_test(id, seq_2, SIZE_SEQ(seq_2)));
  assert(hash_remove(id, seq_1, SIZE_SEQ(seq_1)));
  assert(hash_size(id) == 1);
  assert(!hash_test(id, seq_1, SIZE_SEQ(seq_1)));
  assert(!hash_remove(id, seq_1, SIZE_SEQ(seq_1)));
  assert(hash_insert(id, seq_1, SIZE_SEQ(seq_1)));
  assert(!hash_insert(id, seq_1, SIZE_SEQ(seq_1)));
  assert(hash_size(id) == 2);
  hash_clear(id);
  assert(hash_size(id) == 0);
  assert(hash_insert(id, seq_2, SIZE_SEQ(seq_2)));
  assert(hash_insert(id, seq_1, SIZE_SEQ(seq_1)));
  assert(hash_size(id) == 2);
  assert(hash_remove(id, seq_1, SIZE_SEQ(seq_1)));
  assert(hash_remove(id, seq_2, SIZE_SEQ(seq_2)));
  assert(hash_size(id) == 0);
  hash_delete(id);
#endif /* TEST_NUM == 202 */

// Tablica ciągów z powtórzeniami liczb
#if TEST_NUM == 203
  unsigned long id = hash_create(hash_function_4);
  uint64_t const seq_1[] = {0, 125, 0, -125, 69, 0, 0, -69, 125};
  uint64_t const seq_2[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  uint64_t const seq_3[] = {111111, 11111, 111111, 11111};
  assert(hash_insert(id, seq_1, SIZE_SEQ(seq_1)));
  assert(hash_test(id, seq_1, SIZE_SEQ(seq_1)));
  assert(hash_size(id) == 1);
  assert(hash_insert(id, seq_2, SIZE_SEQ(seq_2)));
  assert(hash_test(id, seq_2, SIZE_SEQ(seq_2)));
  assert(hash_size(id) == 2);
  assert(hash_remove(id, seq_1, SIZE_SEQ(seq_1)));
  assert(!hash_test(id, seq_1, SIZE_SEQ(seq_1)));
  assert(hash_size(id) == 1);
  assert(!hash_remove(id, seq_1, SIZE_SEQ(seq_1)));
  assert(!hash_test(id, seq_1, SIZE_SEQ(seq_1)));
  assert(hash_size(id) == 1);
  assert(hash_insert(id, seq_3, SIZE_SEQ(seq_3)));
  assert(hash_size(id) == 2);
  assert(hash_test(id, seq_3, SIZE_SEQ(seq_3)));
  assert(!hash_insert(id, seq_2, SIZE_SEQ(seq_2)));
  assert(hash_test(id, seq_2, SIZE_SEQ(seq_2)));
  assert(hash_size(id) == 2);
  assert(hash_insert(id, seq_1, SIZE_SEQ(seq_1)));
  assert(hash_size(id) == 3);
  assert(hash_test(id, seq_1, SIZE_SEQ(seq_1)));
  hash_clear(id);
  assert(hash_size(id) == 0);
  assert(hash_insert(id, seq_2, SIZE_SEQ(seq_2)));
  assert(hash_insert(id, seq_1, SIZE_SEQ(seq_1)));
  assert(hash_insert(id, seq_3, SIZE_SEQ(seq_3)));
  assert(hash_size(id) == 3);
  assert(hash_remove(id, seq_1, SIZE_SEQ(seq_1)));
  assert(hash_remove(id, seq_3, SIZE_SEQ(seq_3)));
  assert(hash_remove(id, seq_2, SIZE_SEQ(seq_2)));
  assert(hash_size(id) == 0);
  hash_delete(id);
#endif /* TEST_NUM == 203 */

/**
 ** GRUPA 3
 **/

// Dwukrotne tworzenie tablicy na tej samej zmiennej
#if TEST_NUM == 301
  unsigned long id;
  uint64_t const seq_1[] = {20000, 45000, 89000, 12000, 45000};
  uint64_t const seq_2[] = {1000, 5000, 7000};
  for (size_t k = 0; k < 2; ++k) {
    if (k == 0)
      id = hash_create(hash_function_3);
    else
      id = hash_create(hash_function_4);
    assert(hash_insert(id, seq_1, SIZE_SEQ(seq_1)));
    assert(hash_insert(id, seq_2, SIZE_SEQ(seq_2)));
    assert(hash_test(id, seq_1, SIZE_SEQ(seq_1)));
    assert(hash_test(id, seq_2, SIZE_SEQ(seq_2)));
    assert(hash_size(id) == 2);
    assert(!hash_insert(id, seq_1, SIZE_SEQ(seq_1)));
    assert(!hash_insert(id, seq_2, SIZE_SEQ(seq_2)));
    assert(hash_size(id) == 2);
    hash_clear(id);
    assert(hash_size(id) == 0);
    assert(!hash_remove(id, seq_1, SIZE_SEQ(seq_1)));
    assert(!hash_remove(id, seq_2, SIZE_SEQ(seq_2)));
    assert(hash_size(id) == 0);
    hash_delete(id);
  }
#endif /* TEST_NUM == 301 */

// Fizycznie różne ciągi, ale liczby te same (uwzględniając różne długości ciagów)
#if TEST_NUM == 302
  unsigned long id = hash_create(hash_function_3);
  uint64_t const seq_1[] = {0, 255, 160, 200};
  uint64_t const seq_2[] = {0x00, 0xFF, 0xA0, 0xC8};
  assert(hash_insert(id, seq_1, SIZE_SEQ(seq_1)));
  assert(!hash_insert(id, seq_2, SIZE_SEQ(seq_2)));
  assert(hash_insert(id, seq_2, SIZE_SEQ(seq_2) - 1));
  assert(hash_insert(id, seq_1, SIZE_SEQ(seq_1) - 2));
  assert(hash_size(id) == 3);
  assert(hash_test(id, seq_2, SIZE_SEQ(seq_2)));
  assert(hash_test(id, seq_1, SIZE_SEQ(seq_1) - 1));
  assert(hash_test(id, seq_2, SIZE_SEQ(seq_2) - 2));
  assert(hash_remove(id, seq_1, SIZE_SEQ(seq_1)));
  assert(hash_remove(id, seq_2, SIZE_SEQ(seq_2) - 1));
  assert(hash_remove(id, seq_1, SIZE_SEQ(seq_1) - 2));
  assert(hash_size(id) == 0);
  hash_delete(id);
#endif /* TEST_NUM == 302 */

// Jeden ciąg o zmiennej zawartości
#if TEST_NUM == 303
  unsigned long id = hash_create(hash_function_4);
  uint64_t const seq_1[] = {100, 10000, 1, 1000, 1000000, 1000000000};
  uint64_t const seq_2[] = {0x100, 0x10000, 0x1, 0x1000};
  uint64_t const seq_3[] = {0100, 010000, 01, 01000, 01000000};
  uint64_t seq[6];
  copy_seq(seq_1, seq, SIZE_SEQ(seq_1));
  assert(hash_insert(id, seq, SIZE_SEQ(seq_1)));
  copy_seq(seq_2, seq, SIZE_SEQ(seq_2));
  assert(hash_insert(id, seq, SIZE_SEQ(seq_2)));
  copy_seq(seq_3, seq, SIZE_SEQ(seq_3));
  assert(hash_insert(id, seq, SIZE_SEQ(seq_3)));
  assert(hash_size(id) == 3);
  copy_seq(seq_1, seq, SIZE_SEQ(seq_1));
  assert(hash_test(id, seq, SIZE_SEQ(seq_1)));
  assert(!hash_insert(id, seq, SIZE_SEQ(seq_1)));
  copy_seq(seq_2, seq, SIZE_SEQ(seq_2));
  assert(hash_test(id, seq, SIZE_SEQ(seq_2)));
  assert(!hash_insert(id, seq, SIZE_SEQ(seq_2)));
  copy_seq(seq_3, seq, SIZE_SEQ(seq_3));
  assert(hash_test(id, seq, SIZE_SEQ(seq_3)));
  assert(!hash_insert(id, seq, SIZE_SEQ(seq_3)));
  copy_seq(seq_1, seq, SIZE_SEQ(seq_1));
  assert(hash_remove(id, seq, SIZE_SEQ(seq_1)));
  assert(!hash_remove(id, seq, SIZE_SEQ(seq_1)));
  copy_seq(seq_2, seq, SIZE_SEQ(seq_2));
  assert(hash_remove(id, seq, SIZE_SEQ(seq_2)));
  assert(!hash_remove(id, seq, SIZE_SEQ(seq_2)));
  copy_seq(seq_3, seq, SIZE_SEQ(seq_3));
  assert(hash_remove(id, seq, SIZE_SEQ(seq_3)));
  assert(!hash_remove(id, seq, SIZE_SEQ(seq_3)));
  assert(hash_size(id) == 0);
  hash_delete(id);
#endif /* TEST_NUM == 303 */

// Usuwanie tablicy bez jej czyszczenia
#if TEST_NUM == 304
  unsigned long id = hash_create(hash_function_4);
  uint64_t const seq_1[] = {123456789, 192347568, 642597318};
  uint64_t const seq_2[] = {123456789, 642597318, 192347568};
  assert(hash_insert(id, seq_1, SIZE_SEQ(seq_1)));
  assert(hash_insert(id, seq_2, SIZE_SEQ(seq_2)));
  assert(hash_size(id) == 2);
  hash_delete(id);
#endif /* TEST_NUM == 304 */

// Brak usuwania tablicy
#if TEST_NUM == 305
  unsigned long id = hash_create(hash_function_3);
  uint64_t const seq_1[] = {129456783, 692347518, 842597316};
  uint64_t const seq_2[] = {143256798, 743596218, 792541368};
  assert(hash_insert(id, seq_1, SIZE_SEQ(seq_1)));
  assert(hash_insert(id, seq_2, SIZE_SEQ(seq_2)));
  assert(hash_size(id) == 2);
#endif /* TEST_NUM == 305 */

// Usuwanie pustej tablicy
#if TEST_NUM == 306
  unsigned long id = hash_create(hash_function_4);
  assert(hash_size(id) == 0);
  hash_delete(id);
#endif /* TEST_NUM == 306 */

// Działanie wskaźnika hash_function_t
#if TEST_NUM == 307
  hash_function_t func;
  unsigned long id_1, id_2;
  uint64_t const seq[] = {1, 2, 3};
  func = hash_function_1;
  assert(func(seq, SIZE_SEQ(seq)) == 0);
  id_1 = hash_create(func);
  func = hash_function_2;
  assert(func(seq, SIZE_SEQ(seq)) == 6);
  id_2 = hash_create(func);
  hash_delete(id_1);
  hash_delete(id_2);
#endif /* TEST_NUM == 307 */

/**
 ** GRUPA 4
 **/

// Błędny parametr id
#if TEST_NUM == 401
  unsigned long id = 0;
  uint64_t const seq[] = {0};
  assert(!hash_insert(id, seq, SIZE_SEQ(seq)));
  assert(!hash_test(id, seq, SIZE_SEQ(seq)));
  assert(!hash_remove(id, seq, SIZE_SEQ(seq)));
  assert(hash_size(id) == 0);
  hash_clear(id);
  hash_delete(id);
#endif /* TEST_NUM == 401 */

// Błędny parametr seq
#if TEST_NUM == 402
  unsigned long id = 0;
  uint64_t const seq[] = {1000, 2000};
  assert(!hash_insert(id, NULL, SIZE_SEQ(seq)));
  assert(!hash_test(id, NULL, SIZE_SEQ(seq)));
  assert(!hash_remove(id, NULL, SIZE_SEQ(seq)));
  id = hash_create(hash_function_3);
  assert(!hash_insert(id, NULL, SIZE_SEQ(seq)));
  assert(!hash_test(id, NULL, SIZE_SEQ(seq)));
  assert(!hash_remove(id, NULL, SIZE_SEQ(seq)));
  hash_delete(id);
#endif /* TEST_NUM == 402 */

// Błędny parametr size
#if TEST_NUM == 403
  unsigned long id = 0;
  uint64_t const seq[] = {2022, 2023};
  assert(!hash_insert(id, NULL, 0));
  assert(!hash_test(id, NULL, 0));
  assert(!hash_remove(id, NULL, 0));
  assert(!hash_insert(id, seq, 0));
  assert(!hash_test(id, seq, 0));
  assert(!hash_remove(id, seq, 0));
  id = hash_create(hash_function_4);
  assert(!hash_insert(id, NULL, 0));
  assert(!hash_test(id, NULL, 0));
  assert(!hash_remove(id, NULL, 0));
  assert(!hash_insert(id, seq, 0));
  assert(!hash_test(id, seq, 0));
  assert(!hash_remove(id, seq, 0));
  hash_delete(id);
#endif /* TEST_NUM == 403 */

/**
 ** GRUPA 5
 **/

// Długie ciągi liczb, test bez wydruków diagnostycznych
#if TEST_NUM == 501
  unsigned long id_1 = hash_create(hash_function_3);
  unsigned long id_2 = hash_create(hash_function_4);
  size_t seq_size = 15000000;
  uint64_t *seq = calloc(seq_size, sizeof(uint64_t));
  assert(hash_size(id_1) == 0);
  assert(hash_size(id_2) == 0);
  assert(hash_insert(id_1, seq, seq_size));
  assert(hash_size(id_1) == 1);
  assert(!hash_insert(id_1, seq, seq_size));
  assert(hash_size(id_1) == 1);
  for (size_t k = 0; k < seq_size; ++k)
    seq[k] = 1;
  assert(hash_insert(id_1, seq, seq_size));
  assert(hash_size(id_1) == 2);
  for (size_t k = 0; k < seq_size; ++k)
    seq[k] = k * 100;
  assert(hash_insert(id_1, seq, seq_size));
  assert(hash_size(id_1) == 3);
  assert(hash_insert(id_2, seq, seq_size));
  assert(hash_size(id_2) == 1);
  for (size_t k = 0; k < seq_size; ++k)
    seq[k] = 1;
  assert(hash_test(id_1, seq, seq_size));
  assert(hash_remove(id_1, seq, seq_size));
  assert(hash_size(id_1) == 2);
  assert(!hash_test(id_1, seq, seq_size));
  free(seq);
  hash_clear(id_1);
  hash_delete(id_1);
  hash_delete(id_2);
#endif /* TEST_NUM == 501 */

// Wiele małych tablic haszujących, test bez wydruków diagnostycznych
#if TEST_NUM == 502
  size_t num_ids = 1300000;
  unsigned long *ids = calloc(num_ids, sizeof(unsigned long));
  uint64_t const seq_1[] = {0, 1};
  uint64_t const seq_2[] = {1, 0};
  for (size_t k = 0; k < num_ids; ++k) {
    if (k % 2 == 0)
      ids[k] = hash_create(hash_function_3);
    else
      ids[k] = hash_create(hash_function_4);
    assert(hash_size(ids[k]) == 0);
    assert(hash_insert(ids[k], seq_1, SIZE_SEQ(seq_1)));
    assert(hash_size(ids[k]) == 1);
    assert(hash_insert(ids[k], seq_2, SIZE_SEQ(seq_2)));
    assert(hash_size(ids[k]) == 2);
  }
  for (size_t k = 0; k < num_ids; ++k) {
    assert(hash_test(ids[k], seq_1, SIZE_SEQ(seq_1)));
    assert(hash_test(ids[k], seq_2, SIZE_SEQ(seq_2)));
  }
  for (size_t k = 0; k < num_ids; ++k)
    assert(hash_remove(ids[k], seq_1, SIZE_SEQ(seq_1)));
  for (size_t k = 0; k < num_ids; ++k) {
    assert(!hash_test(ids[k], seq_1, SIZE_SEQ(seq_1)));
    assert(hash_test(ids[k], seq_2, SIZE_SEQ(seq_2)));
  }
  for (size_t k = 0; k < num_ids; ++k)
    hash_clear(ids[k]);
  for (size_t k = 0; k < num_ids; ++k)
    hash_delete(ids[k]);
  free(ids);
#endif /* TEST_NUM == 502 */

// Tablice haszujące z wieloma ciągami, test bez wydruków diagnostycznych
#if TEST_NUM == 503
  unsigned long id_1 = hash_create(hash_function_3);
  unsigned long id_2 = hash_create(hash_function_4);
  size_t tab_size = 700000;
  uint64_t seq[] = {0, 0};
  for (size_t k = 0; k < tab_size; ++k) {
    seq[0] = k;
    seq[1] = k + 1;
    assert(hash_insert(id_1, seq, SIZE_SEQ(seq)));
    assert(hash_insert(id_2, seq, SIZE_SEQ(seq)));
  }
  for (size_t k = 0; k < tab_size; ++k) {
    seq[0] = k;
    seq[1] = k + 1;
    assert(hash_test(id_1, seq, SIZE_SEQ(seq)));
    assert(hash_test(id_2, seq, SIZE_SEQ(seq)));
  }
  for (size_t k = 0; k < tab_size; ++k) {
    seq[0] = k;
    seq[1] = k + 1;
    assert(hash_remove(id_1, seq, SIZE_SEQ(seq)));
  }
  hash_clear(id_2);
  hash_delete(id_1);
  hash_delete(id_2);
#endif /* TEST_NUM == 503 */

/**
 ** GRUPA 6
 **/

// Błędny parametr funkcji hash_create, powinna być asercja.
#if TEST_NUM == 602
  hash_create(NULL);
#endif /* TEST_NUM == 602 */

  return 0;
}

// Sprawdzamy, czy w wersji NDEBUG dołączane jest iostream w hash.h.
// Jeśli nie jest, to program się skompiluje. W przeciwnym razie wystapi
// błąd kompilacji.
#if TEST_NUM == 603
  #define NDEBUG
  #define cerr
#endif /* TEST_NUM == 603 */

/* Włączamy najpierw "hash.h", aby sprawdzić, czy włącza wszystko,
 * co mu jest potrzebne, czyli czy poprawność programu nie zależy od
 * kolejności włączania plików nagłówkowych.
 */
#include "hash.h"

#ifdef NDEBUG
  #undef NDEBUG
#endif

#if TEST_NUM == 102 || TEST_NUM == 601
 #include <cassert>
 #include <vector>
#endif

namespace {

#if TEST_NUM == 102
  using ::std::data;
  uint64_t hash_function(uint64_t const *v, size_t n) {
    uint64_t hash = 0;
    for (size_t k = 0; k < n; ++k)
      hash ^= v[k];
    return hash;
  }
  unsigned long test() {
    unsigned long id = ::jnp1::hash_create(hash_function);
    assert(::jnp1::hash_insert(id, data<uint64_t>({1, 3, 5, 9}), 4));
    return id;
  }
  unsigned long id = test();
#endif /* TEST_NUM == 102 */

#if TEST_NUM == 601
  using ::std::vector;
  using ::std::data;
  uint64_t hash_function(uint64_t const *v, size_t n) {
    uint64_t hash = n;
    for (size_t k = 0; k < n; ++k)
      hash ^= v[k] + (hash << 6) + (hash >> 2) + 0x9e3bcdc99c7b39b7;
    return hash;
  }
#endif /* TEST_NUM == 601 */

} /* anonymous namespace */

int main() {
// Drugi test przykładowy ze specyfikacji zadania
#if TEST_NUM == 102
  assert(!::jnp1::hash_test(id, data<uint64_t>({9, 5, 3, 1}), 4));
  assert(::jnp1::hash_insert(id, data<uint64_t>({9, 5, 3, 1}), 4));
  assert(!::jnp1::hash_test(id, data<uint64_t>({0, 1, 3, 5, 9}), 5));
  assert(::jnp1::hash_size(id) == 2);
  ::jnp1::hash_delete(id);
  assert(!::jnp1::hash_test(id, nullptr, 0));
#endif /* TEST_NUM == 102 */

// Czy wszystkie funkcje są w przestrzeni ::jnp1? Ma się kompilować.
#if TEST_NUM == 601
  unsigned long id = ::jnp1::hash_create(hash_function);
  vector<uint64_t> seq({1000, 5000, 2000, 5000});
  assert(::jnp1::hash_insert(id, seq.data(), seq.size()));
  assert(::jnp1::hash_test(id, seq.data(), seq.size()));
  assert(::jnp1::hash_remove(id, seq.data(), seq.size()));
  assert(::jnp1::hash_insert(id, seq.data(), seq.size()));
  assert(::jnp1::hash_size(id) == 1);
  ::jnp1::hash_clear(id);
  ::jnp1::hash_delete(id);
#endif /* TEST_NUM == 601 */
}

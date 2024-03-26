#include "hash.h"


using namespace std;

namespace {
    // Custom hash function that overrides operator().
    struct CustomHash {
        jnp1::hash_function_t hash_function;

        explicit CustomHash(jnp1::hash_function_t h) : hash_function(h) {}

        uint64_t operator()(const vector <uint64_t> &x) const {
            return hash_function(x.data(), x.size());
        }
    };
    // Map that matches id with it's set that has custom hash function.
    unordered_map<unsigned long, unordered_set<vector < uint64_t>, CustomHash>> &mapOfSets(){
        static unordered_map<unsigned long, unordered_set<vector < uint64_t>, CustomHash>> m;
        return m;
    }
    // Queue that storages deleted sets' id.
    queue<unsigned long> &deletedId(){
        static queue<unsigned long > q;
        return q;
    }
    unsigned long &getID(){
        static unsigned long maxId = 0;
        return maxId;
    }

#ifndef NDEBUG
    const bool debug = true;
#else
    const bool debug = false;
#endif

    /**
     * @brief: Function that copies uint64_t array @param seq
     *  of length @param size to a vector.
     */
    vector <uint64_t> copy_seq(uint64_t const *seq, size_t size) {
        vector <uint64_t> res;
        for (size_t i = 0; i < size; i++) {
            res.push_back(seq[i]);
        }
        return res;
    }

    /**
     * @brief: function that converts uint64_t array @param seq
     * of length @param size to a string.
     */
    string toString(uint64_t const *seq, size_t size) {
        if (seq == NULL) {
            return "NULL";
        }
        string res = "\"";
        for (size_t i = 0; i < size; i++) {
            res += to_string(seq[i]);
            res += (i != size - 1) ? " " : "";
        }
        res += "\"";
        return res;
    }
}

/**
 * @brief: In this namespace are official functions that are
 * described in file hash.h.
 */
namespace jnp1 {
    unsigned long hash_create(hash_function_t hash_function) {
        if (debug) {
            cerr << "hash_create(" << hash_function << ")\n";
        }

        // Creating a new set with custom hash.
        unordered_set <vector<uint64_t>, CustomHash>
                uset(10, CustomHash(hash_function));
        // Matching new set with an id.
        unsigned long id = getID();
        if (!deletedId().empty()) {
            id = deletedId().front();
            deletedId().pop();
        } else {
            getID()++;
        }
        // Adding new set to the map.
        mapOfSets().insert({id, uset});

        if (debug) {
            cerr << "hash_create: hash table #" << id << " created\n";
        }

        return id;
    }

    void hash_delete(unsigned long id) {
        if (debug) {
            cerr << "hash_delete(" << id << ")\n";
        }

        if (mapOfSets().find(id) == mapOfSets().end()) {
            if (debug) {
                cerr << "hash_delete: hash table #" << id
                     << " does not exist\n";
            }
            return;
        }
        // Set exists, so we delete it and free its id.
        mapOfSets().erase(id);
        deletedId().push(id);

        if (debug) {
            cerr << "hash_delete: hash table #" << id << " deleted\n";
        }
    }


    size_t hash_size(unsigned long id) {
        if (debug) {
            cerr << "hash_size(" << id << ")\n";
        }

        if (mapOfSets().find(id) == mapOfSets().end()) {
            if (debug) {
                cerr << "hash_size: hash table #" << id << " does not exist\n";
            }
            return 0;
        }
        size_t res = mapOfSets().at(id).size();

        if (debug) {
            cerr << "hash_size: hash table #" << id;
            cerr << " contains " << res << " element(s)\n";
        }
        return res;
    }

    bool hash_insert(unsigned long id, uint64_t const *seq, size_t size) {
        string s = toString(seq, size);
        if (debug) {
            cerr << "hash_insert(" << id << ", ";
            cerr << s << ", " << size << ")\n";
        }

        bool flag = false;

        if (seq == NULL) {
            if (debug) {
                cerr << "hash_insert: invalid pointer (NULL)\n";
            }
            flag = true;
        }

        if (size == 0) {
            if (debug) {
                cerr << "hash_insert: invalid size (0)\n";
            }
            flag = true;
        }

        if(flag){
            return false;
        }

        if (mapOfSets().find(id) == mapOfSets().end()) {
            if (debug) {
                cerr << "hash_insert: hash table #" << id
                     << " does not exist\n";
            }
            return false;
        }

        vector <uint64_t> copied = copy_seq(seq, size);
        if (mapOfSets().at(id).count(copied) >= 1) {
            // Vector was already present.
            if (debug) {
                cerr << "hash_insert: hash table #" << id
                     << ", sequence " << s << " was present\n";
            }
            return false;
        }
        // Adding new vector to id's set.
        mapOfSets().at(id).insert(copied);
        if (debug) {
            cerr << "hash_insert: hash table #" << id
                 << ", sequence " << s << " inserted\n";
        }
        return true;
    }

    bool hash_remove(unsigned long id, uint64_t const *seq, size_t size) {
        string s = toString(seq, size);
        if (debug) {
            cerr << "hash_remove(" << id << ", ";
            cerr << s << ", " << size << ")\n";

        }

        bool flag = false;

        if (seq == NULL) {
            if (debug) {
                cerr << "hash_remove: invalid pointer (NULL)\n";
            }
            flag = true;
        }

        if (size == 0) {
            if (debug) {
                cerr << "hash_remove: invalid size (0)\n";
            }
            flag = true;
        }

        if(flag){
            return false;
        }

        if (mapOfSets().find(id) == mapOfSets().end()) {
            if (debug) {
                cerr << "hash_remove: hash table #" << id
                     << " does not exist\n";
            }
            return false;
        }

        vector <uint64_t> copied = copy_seq(seq, size);
        if (mapOfSets().at(id).count(copied) == 0) {
            if (debug) {
                cerr << "hash_remove: hash table #" << id
                     << ", sequence " << s << " was not present\n";
            }
            return false;
        } else {
            // Sequence was present.
            mapOfSets().at(id).erase(copied);
            if (debug) {
                cerr << "hash_remove: hash table #" << id
                     << ", sequence " << s << " removed\n";
            }
            return true;
        }
    }

    void hash_clear(unsigned long id) {
        if (debug) {
            cerr << "hash_clear(" << id << ")\n";
        }

        if (mapOfSets().find(id) == mapOfSets().end()) {
            if (debug) {
                cerr << "hash_clear: hash table #" << id
                     << " does not exist\n";
            }
            return;
        }
        if (mapOfSets().at(id).empty()) {
            if (debug) {
                cerr << "hash_clear: hash table #" << id << " was empty\n";
            }
            return;
        }
        mapOfSets().at(id).clear();
        deletedId().push(id);
        if (debug) {
            cerr << "hash_clear: hash table #" << id << " cleared\n";
        }
    }

    bool hash_test(unsigned long id, uint64_t const *seq, size_t size) {
        string s = toString(seq, size);
        if (debug) {
            cerr << "hash_test(" << id << ", ";
            cerr << s << ", " << size << ")\n";

        }

        bool flag = false;

        if (seq == NULL) {
            if (debug) {
                cerr << "hash_test: invalid pointer (NULL)\n";
            }
            flag = true;
        }

        if (size == 0) {
            if (debug) {
                cerr << "hash_test: invalid size (0)\n";
            }
            flag = true;
        }

        if(flag){
            return false;
        }

        if (mapOfSets().find(id) == mapOfSets().end()) {
            if (debug) {
                cerr << "hash_test: hash table #" << id
                     << " does not exist\n";
            }
            return false;
        }

        // Checking if a sequence is in our set.
        if (mapOfSets().at(id).find(copy_seq(seq, size)) !=
            mapOfSets().at(id).end()) {
            if (debug) {
                cerr << "hash_test: hash table #" << id
                     << ", sequence " << s << " is present\n";
            }
            return true;

        } else {
            if (debug) {
                cerr << "hash_test: hash table #" << id
                     << ", sequence " << s << " is not present\n";
            }
            return false;
        }
    }
}
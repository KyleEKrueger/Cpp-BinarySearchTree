// San Diego State University
// CS210/CS496: Data Structures
//
// File: Map.h
//
// Defines the basic required methods for a dictionary of keys and values.
//
// Additional Reading:
// https://en.wikipedia.org/wiki/Set_(abstract_data_type)

#ifndef PROG2_MAP_H
#define PROG2_MAP_H

#define VERSION_MAP 1.1

#include <memory>

#include "Set.h"

namespace sdsu {
    template<typename KEY, typename VALUE>
class Map : public sdsu::Set<KEY> {

    public:

        virtual ~Map(){};

        // Accesses the value associated with the referenced key
        //
        // return: a reference to the value in the map if present.
        // undefined behavior if the key is missing (ask my why!)
        virtual VALUE &get(const KEY &key) = 0;

        // Adds the key/value pair to the data structure.
        //
        // If the value is already present, it returns the previous value
        // If the value was missing, it returns nullptr
        virtual VALUE insert(const KEY &key, const VALUE &val) = 0;

        // Gives the caller an in-order array of keys.
        //
        // return: two items grouped together in a pair. The first is an array
        // filled with keys, and the second is the size of that array.
        virtual std::pair<std::shared_ptr<KEY*>,int> keys()=0;

        // Gives the caller a display of values that correspond to
        // the in-order key traversal. That is, someone could request
        // the keyset and the values, and keyset[1] corresponds to values[1].
        //
        // return: two items grouped together in a pair. The first is an array
        // filled with values, and the second is the size of that array.
        virtual std::pair<std::shared_ptr<VALUE*>,int> values()=0;

    };
}
#endif //PROG2_MAP_H

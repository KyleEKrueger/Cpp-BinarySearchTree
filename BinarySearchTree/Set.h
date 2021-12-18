// San Diego State University
// CS210/CS496: Data Structures
//
// File: set.h
//
// Defines the basic required methods for a unique collection or items.
// Sets mimic their mathematical counterparts. Sets have no order.
//
// Additional Reading:
// https://en.wikipedia.org/wiki/Set_(abstract_data_type)


#ifndef PROG2_SET_H
#define PROG2_SET_H

namespace sdsu {
    template<typename T>
    class Set {

    public:

        // A virtual destructor to help with polymorphism
        virtual ~Set(){};

        // Reports true if the item exists in the container
        //
        // Return: true if an entry associated with the parameter is present
        // false if missing.
        virtual bool contains(const T &) const = 0;

        // Resets the structure to an empty state.
        //
        // This is not a crypto-clear, data should not be overwritten.
        virtual void clear() = 0;

        // Reports true if the item is present in the container
        // Syntactic sugar for contains.
        virtual bool exists( const T&t) const {return contains(t);}

        // Adds the specified item to the structure.
        //
        // Returns: true after adding to the structure
        virtual bool insert(const T &) = 0;

        virtual bool isEmpty() const { return !size(); }

        // Deletes the identified item from the data structure.
        //
        // returns false if it was not present or true if it was
        virtual bool remove(const T &) = 0;

        virtual int size() const = 0;

    };
}
#endif //PROG2_SET_H

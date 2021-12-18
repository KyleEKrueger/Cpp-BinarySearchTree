// San Diego State University
// CS210/CS496: Data Structures
//
// File: main.cpp
// Exercises and tests a set
//

#include <iostream>
#include <iomanip>
#include <random>
#include <set>
#include <memory>

#include "BinarySearchTree.h"
#include "Map.h"
#include "Set.h"

#define VERSION         1.1

#ifndef VERSION_SET
#define VERSION_SET 1.0
#endif

#define TEST_SIZE 1024

void versionReport(){
    std::cout << std::string(4,'-') << " Version Report " << std::string(4,'-');
    std::cout << std::endl;
    std::cout << "|" << std::setw(16) << "Main Version: " <<
              std::setw(4) << VERSION << "  |" << std::endl;

    std::cout << "|" << std::setw(16) << "SET ADT: " <<
              std::setw(4) << VERSION_SET <<  "  |" << std::endl;
    std::cout << "|" << std::setw(16) << "MAP ADT: " <<
              std::setw(4) << VERSION_MAP << "  |" << std::endl;
    std::cout << std::string(24,'-') << std::endl;
}

#define RUN_AND_REPORT(NUM, NAME, __VAL)  {std::cout << std::setw(2) << \
NUM << std::setw(70)  << std::setfill('.') << NAME << \
 std::setw(8) << (__VAL == true ? "PASS\n":"FAIL\n") << std::setfill(' ');}

template<int SIZE>
std::shared_ptr<sdsu::Set<int>> getRandomSet() {
    std::shared_ptr<sdsu::Set<int>> toRet(new sdsu::BinarySearchTree<int, void *>());

    // The name says random, so we need a RNG
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1, SIZE);

    std::set<int> observed;

    for (int i = 0; i < SIZE; i++) {
        int key = distribution(generator);
        while (observed.find(key) != observed.end())
            key = distribution(generator);
        observed.insert(key);
        toRet->insert(key); // insert a random key
    }

    return toRet;
}

bool testIsEmpty_afterConstruct_isEmpty() {

    sdsu::BinarySearchTree<char, void *> uut;

    return uut.isEmpty();
}

// Version 1.1: i-1 became i+1, did not preload a set with random
template<int SIZE>
bool testSize_afterInsertingRandomOrder_correctAlongTheWay() {
    // LOOK: Smarter pointers so you don't manage memory!!!!!
    std::shared_ptr<sdsu::Set<int>> uut(new sdsu::BinarySearchTree<int,int>);

    // The name says random, so we need a RNG
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1, SIZE);

    for (int i = 0; i < SIZE; i++) {
        uut->insert(distribution(generator)); // insert a random key

        // --> TEST <--
        int datum = uut->size();
        if (datum != i + 1) return false;
    }

    return true;
}

// Version 1.1 tied to observed.size vs. SIZE
template<int SIZE>
bool testSize_afterRemovingAllSIZEItems_correctAlongTheWay() {

    std::unique_ptr<sdsu::Set<int>> uut(new sdsu::BinarySearchTree<int, void *>());

    // The name says random, so we need a RNG
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1, SIZE);

    std::set<int> observed;

    for (int i = 0; i < SIZE; i++) {
        int key = distribution(generator);
        observed.insert(key);
        uut->insert(key); // insert a random key
    }

    std::set<int>::iterator it = observed.begin();
    for (int i = 0; i < observed.size(); i++) {
        if (uut->size() != observed.size() - i) return false;
        uut->remove(*it);
        it++;
    }
    return true;
}

// Version 1.1 uut->insert
template<int SIZE>
bool testInsert_SIZERandomItems_containsAll() {
    std::unique_ptr<sdsu::Set<int>> uut(new sdsu::BinarySearchTree<int, void *>());

    // The name says random, so we need a RNG
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1, SIZE);
    std::set<int> obs;
    int n = SIZE;
    while (n) {
        int val = distribution(generator);
        if (obs.find(val) != obs.end()) continue;
        obs.insert(val);
        uut->insert(val);
        n--;
    }
    for (auto val: obs) {
        if (!uut->contains(val)) return false;
    }

    return true;
}

template<int SIZE>
bool testInsert_SIZEIdenticalItems_sizeEqualsOne() {
    std::shared_ptr<sdsu::Set<char>> uut(new sdsu::BinarySearchTree<char, bool>());

    for (int i = 0; i < SIZE; i++) {
        uut->insert('@');
    }

    // Test point
    return uut->size() == 1;
}

template<int SIZE>
bool testClear_insertSIZERandomitems_isEmpty() {

    std::shared_ptr<sdsu::Set<int>> uut = getRandomSet<TEST_SIZE>();

    if (!uut->isEmpty()) {
        uut->clear();
        return uut->isEmpty();
    }
    return false;
}

bool testRemove_itemMissing_returnsFalse() {
    std::shared_ptr<sdsu::Set<char>> uut(new sdsu::BinarySearchTree<char,char>());
    uut->insert('.');
    if (!uut->exists('.')) return false;
    return !uut->remove('@');
}

bool testRemove_itemPresent_returnsTrue() {
    std::shared_ptr<sdsu::Set<int>> uut = getRandomSet<TEST_SIZE>();
    uut->insert('@');
    return uut->remove('@');
}

// Version 1.1 correct Return val
template<int SIZE>
bool testRemove_sameProbeValuesSIZETimesWhileBuilding_presentNotPresent() {

    std::shared_ptr<sdsu::Set<int>> uut(new sdsu::BinarySearchTree<int, void *>());

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1, SIZE);
    std::set<int> obs;
    int n = SIZE;
    while (n) {
        int val = distribution(generator);
        if (obs.find(val) != obs.end()) continue;
        obs.insert(val);

        uut->insert(val);
        uut->insert(0);
        uut->insert(SIZE + 1);
        if (!uut->contains(0) || !uut->contains(SIZE + 1)) return false;

        // ACTION
        uut->remove(0);
        uut->remove(SIZE + 1);

        // Test
        if (uut->contains(0) || uut->contains(SIZE + 1)) return false;
        n--;
    }

    return true;
}

template<int SIZE>
void initBalancedSequence(int arr[], int start = 0, int stop = SIZE) {
    // assume size power of 2
    arr[0] = stop >> 1;
    arr[1] = arr[0] >> 1;
    arr[2] = arr[0] + (arr[0] >> 1);

    // abs(parent-gp)
    // half that distance
    // subtract from this for left
    // add to this to left

    for (int i = 1; i < arr[0] - 1; i++) {
        int dist = std::abs(arr[i] - (arr[(i - 1) >> 1]));
        arr[2 * i + 1] = arr[i] - (dist >> 1);
        arr[2 * i + 2] = arr[i] + (dist >> 1);
    }
}

// Version 1.1: SIZE-1
template<int SIZE>
bool testRemove_withChildren_presentNotPresent() {
    // BUILD
    sdsu::BinarySearchTree<int, void *> uut;
    int sequence[SIZE - 1];
    initBalancedSequence<SIZE>(sequence);
    for (int key: sequence) {
        uut.insert(key);
    }

    // ACTION: remove the first half of the sequence we inserted. This should be the odd numbers
    for (int i = 0; i < (SIZE >> 1); i++) {
        if (!uut.remove(sequence[i])) return false;
    }

    // VERIFY: Not the first part of the sequence is not present
    for (int i = 0; i < (SIZE >> 1); i++) {
        if (uut.contains(sequence[i])) return false;
    }

    // VERIFY: Present
    for (int i = (SIZE >> 1); i < SIZE-1; i++) {
        if (!uut.contains(sequence[i])) return false;
    }

}

// Version 1.1: SIZE-1
template<int SIZE>
bool testRemove_leaves_presentNotPresent() {

    // BUILD
    sdsu::BinarySearchTree<int, void *> uut;
    int sequence[SIZE - 1];
    initBalancedSequence<SIZE>(sequence);
    for (int key: sequence) {
        uut.insert(key);
    }

    // ACTION: Leaves are odd due to how balanced sequence created, and in the last half
    // of the sequence
    for (int i = (SIZE >> 1) - 1; i < (SIZE-1); i++) {
        if (!uut.remove(sequence[i])) return false;
    }

    // VERIFY: Not present
    for (int i = 1; i < (SIZE-1); i += 2) {
        if (uut.contains(i)) return false;
    }

    // VERIFY: Present
    for (int i = 2; i < (SIZE-1); i += 2) {
        if (!uut.contains(i)) return false;
    }

    return true;
}

template<int SIZE>
bool testGet_changedValue_valueUpdatedInMap() {
    std::shared_ptr<sdsu::Map<int, int>> uut(new sdsu::BinarySearchTree<int, int>);

    for (int i = 0; i < SIZE; i++) {
        uut->insert(i, 496);
    }

    // Action
    for (int i = 0; i < SIZE; i++) {
        if( !uut->exists(i) ) return false;
        uut->get(i) = 210;
    }

    // Verify
    for (int i = 0; i < SIZE; i++)
        if (uut->get(i) != 210) return false;

    return true;

};


template<int SIZE>
bool testInsertKeyValue_uniqueKeysValues_allPresent(){

    int seq[SIZE-1];
    initBalancedSequence<SIZE>(seq);

    std::shared_ptr<sdsu::BinarySearchTree<int,int>> uut(new sdsu::BinarySearchTree<int,int>());
    for(int i = 0; i<SIZE-1; i++)
        uut->insert(seq[i], i);

    // Verify:
    for(int i = 0; i<(SIZE-1); i++)
        if( !uut->exists(seq[i]) || uut->get(seq[i]) != i)
            return false;

    return true;
}

void testSetMethods() {

    int testNum = 1;

    std::cout << "\nTesting Set Methods . . .\n";

    // Size seems like a simple method, so let us start here:
    RUN_AND_REPORT(testNum++, "testIsEmpty_afterConstruct_isEmpty",
                   testIsEmpty_afterConstruct_isEmpty());

    RUN_AND_REPORT(testNum++, "testSize_afterInsertingRandomOrder_correctAlongTheWay",
                   testSize_afterInsertingRandomOrder_correctAlongTheWay<TEST_SIZE>());

    RUN_AND_REPORT(testNum++, "testSize_afterRemovingAllSIZEItems_correctAlongTheWay",
                   testSize_afterRemovingAllSIZEItems_correctAlongTheWay<TEST_SIZE>());

    RUN_AND_REPORT(testNum++, "testInsert_SIZERandomItems_containsAll",
                   testInsert_SIZERandomItems_containsAll<TEST_SIZE>());

    RUN_AND_REPORT(testNum++, "testInsert_SIZEIdenticalItems_sizeEqualsOne",
                   testInsert_SIZEIdenticalItems_sizeEqualsOne<TEST_SIZE>());

    RUN_AND_REPORT(testNum++, "testClear_insertSIZERandomitems_isEmpty",
                   testClear_insertSIZERandomitems_isEmpty<TEST_SIZE>());

    RUN_AND_REPORT(testNum++, "testRemove_itemMissing_returnsFalse",
                   testRemove_itemMissing_returnsFalse());

    RUN_AND_REPORT(testNum++, "testRemove_itemPresent_returnsTrue",
                   testRemove_itemPresent_returnsTrue());

    RUN_AND_REPORT(testNum++, "testRemove_sameProbeValuesSIZETimesWhileBuilding_presentNotPresent",
                   testRemove_sameProbeValuesSIZETimesWhileBuilding_presentNotPresent<TEST_SIZE>());

    RUN_AND_REPORT(testNum++, "testRemove_leaves_presentNotPresent",
                   testRemove_leaves_presentNotPresent<TEST_SIZE>());

    RUN_AND_REPORT(testNum++, "testRemove_withChildren_presentNotPresent",
                   testRemove_withChildren_presentNotPresent<TEST_SIZE>());
}

void testMapMethods() {

    std::cout << "\nTesting Map Methods . . .\n";

    int testNum = 1;


    RUN_AND_REPORT(testNum++, "testGet_changedValue_valueUpdatedInMap",
                   testGet_changedValue_valueUpdatedInMap<16>());


    RUN_AND_REPORT(testNum++, "testInsertKeyValue_uniqueKeysValues_allPresent",
                   testInsertKeyValue_uniqueKeysValues_allPresent<TEST_SIZE>());

}

int main() {

    versionReport();

    testSetMethods();

    testMapMethods();

    std::cout << "Complete." << std::endl;
    return 0;
}

// STUDENT CODE by Kyle Krueger
// San Diego State University
// CS210/CS496: Data Structures
//
// File: BinarySearchTree.h
//
// A concrete implementation of a basic binary search tree. This structure is
// useful only for education and illustration of the BAD things about growth.
//
// Additional Reading:
// https://en.wikipedia.org/wiki/Binary_search_tree


#ifndef PROG2_BINARYSEARCHTREE_H
#define PROG2_BINARYSEARCHTREE_H

#include <memory>


#include "Map.h"

namespace sdsu {
    template<typename KEY, typename VALUE>
    class BinarySearchTree : public Map<KEY, VALUE> {

        // The BST links together BSTNode objects. The outside world never
        // needs to use these, but this data structure will use them to build
        // the search tree.
        int BSTsize; // this is the size of our tree

        struct BSTNode {

            // The BST structures itself around the KEY's ranking. The key is
            // so important, it must be unique in the BST.
            KEY key;


            // The value will almost certainly not be a void*, but this allows
            // us to store ANYTHING, for we can cast a void* into something else
            // void* is an address to anything . . . not nothingness. Values
            // in a map may be repeated, and values don't appear in a Set.
            VALUE value;

            std::shared_ptr<BSTNode> childL;
            std::shared_ptr<BSTNode> childR;

            BSTNode() {};

            BSTNode(KEY item) : key(item), childL(nullptr), childR(nullptr) {};

            std::pair<std::shared_ptr<KEY *> &, int> keyset(std::shared_ptr<KEY *> &arr) {
                std::pair<std::shared_ptr<KEY *> &, int> toRet(arr);
            };

            // This is something like Java's toString method.
            // This is an IN-ORDER traversal.
            friend std::ostream &operator<<(std::ostream &os, BSTNode const &node) {
                if (node.childL != nullptr) os << *node.childL;
                os << std::setw(8) << node.key << " ";
                if (node.childR != nullptr) os << *node.childR;
                return os;
            }
        };


        std::shared_ptr<BSTNode> root;

        // this function may help when you write other functions. Sometimes you
        // wnat to literally work with the node holding the key, and not
        // just the keys and values themselves. Your design will decide if you
        // need something like this or not.
        // BSTNode &getNode(const KEY &key){}

        // This is a PRIVATE version of teh insert function that no one on
        // the outside can call. I find it useful for something like
        // the public version to kick things off, and this does the real
        // work.
        bool
        insert(const KEY &key, std::shared_ptr<BSTNode> &start, const VALUE &value) { // Insert function is recursive
            //Check if key already exists
            if (start->key == key) {
                return false;
            }
            //Check if we need to traverse downward
            if (start->childL != NULL && start->key > key) {
                insert(key, start->childL, value);
            }
            if (start->childR != NULL && start->key < key) {
                insert(key, start->childR, value);
            }
            //Now we've hit the bottom of the tree, and we are looking to insert
            if (start->key < key && start->childR == NULL) {
                BSTsize++;
                start->childR = std::make_shared<BSTNode>(key);
                start->childR->value = value;
                return true;
            }
            if (start->key > key && start->childL == NULL) {
                BSTsize++;
                start->childL = std::make_shared<BSTNode>(key);
                start->childL->value = value;
                return true;
            }
            return true;
        }

    public:
        BinarySearchTree() {
            BSTsize = 0;
            root = nullptr;

        }

        BinarySearchTree(const BinarySearchTree<KEY, VALUE> &other) {

        }

        ~BinarySearchTree() override {

        }

        bool contains(
                const KEY &key) const override { //calls our search function and checks if we get a nullptr back, if we do then the tree does not contain the key
            if (search(key, false).operator bool()) {
                return true;
            } else {
                return false;
            }
        }

        KEY largestKeyInTree(){
            std::shared_ptr<BSTNode> explorerNode = root;
            if (explorerNode == nullptr){
                //root doesn't exist, so tree doesn't exist
                return NULL;
            }
            //while loop that will traverse the explorer node down the right children of the tree
            //until there are no more right children, and since values to the right are larger
            //there will be no value greater than the rightmost node.
            while (explorerNode->childR != nullptr){
                explorerNode = explorerNode->childR;

            }
            //explorer node is now pointing at the right most child, return it's key
            return explorerNode->key;
        }

       

        std::shared_ptr<BSTNode> search(const KEY &key,
                                        bool returnParent) const { // This search function will return either the key we are looking for, or its parent.
            bool canExit = false;
            std::shared_ptr<BSTNode> explorerPointer = root; // This is our pointer that will be searching for the key
            std::shared_ptr<BSTNode> explorerParent = nullptr; // This is the parent that will follow along behind it
            if (explorerPointer ==
                nullptr) { // Check if the explorer pointer is null, meaning there is no tree there since root is null
                return nullptr;
            }
            do { // since there is a tree, we must traverse it
                if (explorerPointer->key ==
                    key) { // If the explorer pointer's key is the key we are looking for, then explorer pointer is pointing at our target
                    if (returnParent) { // if we want to return the parent
                        return explorerParent;
                    }
                    return explorerPointer; // if we want to return the explorer
                    canExit = true;
                } else if (explorerPointer->key <
                           key) { // if the explorer pointer's key is less than our target's, then we must go right
                    explorerParent = explorerPointer;
                    explorerPointer = explorerPointer->childR;
                } else if (explorerPointer->key >
                           key) { // if the explorer pointer's key is greater than our target's then we must go left
                    explorerParent = explorerPointer;
                    explorerPointer = explorerPointer->childL;
                }
                if (explorerPointer ==
                    nullptr) { // Check if the explorer pointer is null, meaning there is no tree there since root is null
                    return nullptr;
                }
                if (explorerPointer->childL == nullptr && explorerPointer->key >
                                                          key) { // if the explorer needs to go left, but there is a null pointer, then we have hit the end of our tree and cannot proceed
                    return nullptr; // return a null pointer
                    canExit = true;
                }
                if (explorerPointer->childR == nullptr && explorerPointer->key <
                                                          key) { // if the explorer needs to go right, but there is a null pointer, then we have hit the end of our tree and cannot proceed
                    return nullptr;
                    canExit = true;
                }
            } while (!canExit); // tbh this do while loop can probably be rewritten, but it works so why fix it if it ain't breaking things?
        }

        void
        clear() override { // Shared pointers are dope, because once we de-reference the root, the entire tree is deleted
            root = nullptr;
            BSTsize = 0;
        }

        virtual VALUE &
        get(const KEY &key) override { // uses the search function to return the node that we are looking for, and grabs that value
            return (search(key, false)->value);
        }

        bool insert(const KEY &key) override {
            //I take care of the case where there is no value, by calling the other insert function but with the value = NULL
            return insert(key, NULL);
        }

        //Check for special cases, if not then pass the values into the private insert function
        VALUE insert(const KEY &key, const VALUE &val) override {
            if (root == nullptr) { //checks if there's a tree, and creates one if there isn't
                root = std::make_shared<BSTNode>(key);
                root->value = val;
                root->childL = nullptr;
                root->childR = nullptr;
                BSTsize++;
            } else {
                insert(key, root, val); // pass to the private insert function
            }
            return val;
        }

        std::pair<std::shared_ptr<KEY *>, int> keys() override {
            KEY *raw = new KEY[size()];
            std::shared_ptr<KEY *> arr = std::make_shared<KEY *>(raw);

            // Todo: Extra Credit Students fill up the arr[] with the keys in-order

            std::pair<std::shared_ptr<KEY *>, int> toRet(arr, size());
            return toRet;
        };

        virtual std::pair<std::shared_ptr<VALUE *>, int> values() override {
            VALUE *raw = new VALUE[size()];
            std::shared_ptr<VALUE *> arr = std::make_shared<VALUE *>(raw);

            // Todo: Students fill up the arr[] with the values in-order
            // Todo: with respect to their keys (not each other). Extra Credit

            std::pair<std::shared_ptr<VALUE *>, int> vals(arr, size());
            return vals;
        };

        bool hasRightChild(
                std::shared_ptr<BSTNode> bstNode) { // This function is used in the remove function to avoid de-referencing a nullptr
            if (bstNode == nullptr) {
                return false;
            }
            if (bstNode->childR != nullptr) {
                return true;
            }
            return false;
        }

        bool hasLeftChild(
                std::shared_ptr<BSTNode> bstNode) {// This function is used in the remove function to avoid de-referencing a nullptr
            if (bstNode == nullptr) {
                return false;
            }
            if (bstNode->childL != nullptr) {
                return true;
            }
            return false;
        }

        bool remove(const KEY &key) override {
            //Using the search function, we first find the node that we are trying to remove and its parent
            std::shared_ptr<BSTNode> searchedNode = root; // <- This is the node that contains the key we're looking for
            std::shared_ptr<BSTNode> searchedParent = search(key, true); //<- This is the parent to the node above
            if (hasLeftChild(
                    searchedParent)) { // Since we found the parent, we can check which child is the key, and set the Searched Node to that child
                if (searchedParent->childL->key == key) {
                    searchedNode = searchedParent->childL;
                }
            }
            if (hasRightChild(searchedParent)) {
                if (searchedParent->childR->key == key) {
                    searchedNode = searchedParent->childR;
                }
            }
            //Now we are to check if there is even a node there, for if there isn't there will be a nullptr in our searchedNode
            if (searchedNode == nullptr) {
                return false;
            }
            //So we've got a node, nice... There are 3 possible cases to consider.
            //The first case to deal with is if we have no children present on our node we are trying to remove. Simply remove the node.
            if (!hasLeftChild(searchedNode) && !hasRightChild(searchedNode) && searchedNode != root) {
                if (searchedNode->key < searchedParent->key) {
                    searchedParent->childL = nullptr;
                    BSTsize--;
                    return true;
                }
                searchedParent->childR = nullptr;
                BSTsize--;
                return true;
            }
            //The second case will be tougher, and will deal with the case of us having one child on the node
            if (hasLeftChild(searchedNode) != hasRightChild(
                    searchedNode)) { // using these methods, I can use logic gates without directly referencing the pointers
                if (searchedParent == nullptr) { // The root case
                    if (searchedNode->childL != nullptr) { // If the root has a left child...
                        root = searchedNode->childL; // set the root to the left child of the searched node
                        BSTsize--;
                        return true; // returning will erase the searched node because of shared pointers
                    }
                    if (searchedNode->childR != nullptr) { // If the root has a right child...
                        root = searchedNode->childR; // set the root to the right child of the searched node
                        BSTsize--;
                        return true; // returning will erase the searched node because of shared pointers
                    }
                }
                if (searchedParent->childL != nullptr) {
                    if (searchedParent->childL->key ==
                        searchedNode->key) {// The searched node is a left child of the parent
                        if (searchedNode->childR != nullptr) { // if the searched node has a right child
                            searchedParent->childL = searchedNode->childR;
                            BSTsize--;
                            return true;
                        } else { // if the searched node has a left child
                            searchedParent->childL = searchedNode->childL;
                            BSTsize--;
                            return true;
                        }
                    }
                }
                if (searchedParent->childR != nullptr) {
                    if (searchedParent->childR->key ==
                        searchedNode->key) {// The searched node is a right child of the parent
                        if (searchedNode->childR != nullptr) { // if the searched node has a right child
                            searchedParent->childR = searchedNode->childR;
                            BSTsize--;
                            return true;
                        } else { // if the searched node has a left child
                            searchedParent->childR = searchedNode->childL;
                            BSTsize--;
                            return true;
                        }
                    }
                }
            }

            //The final case consists of us removing a node with 2 children
            if (hasRightChild(searchedNode) && hasLeftChild(searchedNode)) {
                //This one is tricky, We will attempt to replace the node we are trying to remove with the furthest left child of the right node
                std::shared_ptr<BSTNode> tempNode = searchedNode->childR;// create a new temp node, and set it to the right child of the node to be deleted
                std::shared_ptr<BSTNode> tempParent = tempNode;
                while (tempNode->childL != nullptr) {
                    tempParent = tempNode;
                    tempNode = tempNode->childL; // This will end once the temp node is pointing at the left most leaf
                }
                searchedNode->key = tempNode->key; // swap the contents of the two on the tree
                searchedNode->value = tempNode->value;
                tempParent->childL = nullptr;
                BSTsize--;
                return true;
            }
            return false;
        }

        int size() const override {
            return BSTsize;
        }

        VALUE &operator[](std::size_t idx) {
            return get(idx);
        }

        friend std::ostream &operator<<(std::ostream &os, sdsu::BinarySearchTree<KEY, VALUE> const &bst) {
            if (bst.root != nullptr) return os << "[" << (*bst.root) << "]";
            return os;
        }
    };
}
#endif //PROG2_BINARYSEARCHTREE_H

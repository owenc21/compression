#ifndef LZW_DLB
#define LZW_DLB

#include <string>
#include <memory>

class DLB{
    private:
        struct DLB_Node{
            /**
             * Private struct for the nodes in each
             * linked list in the DLB
            */

            char c; // Character of node
            bool key_valid; // Flag to check if key is valid (is a valid inserted string)
            int key; // Key of string, if node is last representation
            std::shared_ptr<DLB_Node> right; // Pointer to right list node
            std::shared_ptr<DLB_Node> down; // Pointer to down list node
        };
        std::shared_ptr<DLB_Node> head; // Start of trie

    public:
        DLB();
        void put(std::string s, int key); // Put s into trie with key
        std::string longest_prefix_of(std::string s); // Prefix match with string s
        int get(std::string s); // Get key for string s
};

#endif
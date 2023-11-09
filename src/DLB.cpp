/**
 * Implementation of DLB Trie
 * 
 * Stores strings implicitly in the trie as a path
 * Maps strings to given keys
 * Supports prefix mapping of given string 
 * Supports key retrevial of stored strings
*/
#include <memory>
#include <string>
#include <stdexcept>
#include "DLB.hh"

DLB::DLB(){
    /**
     * Initialize fields in DLB
     * Head will always have char value 0 for first ASCII symbol
    */
    head = std::make_shared<DLB_Node>();
    head->c = static_cast<char>(0);
    head->key = 0;
    head->key_valid = false;
    head->down = nullptr;
    head->right = nullptr;
}

void DLB::put(std::string s, int key){
    /**
     * Inserts given string into the trie and
     * maps string to the given key
     * 
     * @param s String to insert into trie
     * @param key   Key to map string to in trie
    */

    /* Iterate over all characters in string, inserting one by one */
    std::shared_ptr<DLB_Node> traverse(head); // Pointer to node for list traversal
    for(auto ch=s.begin(); ch!=s.end(); ++ch){
        while((traverse->c != *ch) && (traverse->right != nullptr)){
            traverse = traverse->right;
        }

        /* Case where node for character exists */
        if(traverse->c == *ch){
           if(ch == s.end()-1){
                traverse->key = key;
                traverse->key_valid = true;
                continue;
           }
           else{
                if(traverse->down == nullptr){
                    traverse->down = std::make_shared<DLB_Node>();
                    traverse = traverse->down;
                    traverse->c = static_cast<char>(0);
                    traverse->key = 0;
                    traverse->key_valid = false;
                    traverse->down = nullptr;
                    traverse->right = nullptr;
                }
           }
        }
        /* Case where node for character must be created */
        else{
            traverse->right = std::make_shared<DLB_Node>();
            traverse = traverse->right;
            traverse->c = *ch;
            traverse->key_valid = (ch == s.end()-1) ? true : false;
            traverse->key = traverse->key_valid ? key : 0;
            traverse->right=nullptr;
            traverse->down = nullptr;

            /* If not at end, need to initialize down node */    
            if(traverse->key_valid) continue;

            traverse->down = std::make_shared<DLB_Node>();
            traverse = traverse->down;
            traverse->c = static_cast<char>(0);
            traverse->key = 0;
            traverse->key_valid = false;
            traverse->down = nullptr;
            traverse->right = nullptr;
            continue;
        }
    }
}

void DLB::put(char c, int key){
    /**
     * Overloaded member for inserting a single
     * char into the trie with a given key
     * 
     * @param c char to insert into trie
     * @param key   Key (int) to map char to in trie
    */

    std::string s(1, c);
    put(s, key);
}

std::string DLB::longest_prefix_of(std::string s){
    /**
     * Given a string, returns longest string in trie
     * that is a prefix of the given string
     * 
     * @param s String to prefix match to
     * @returns Longest string in trie that is a prefix of s
    */

    std::string prefix;
    std::shared_ptr<DLB_Node> traverse(head); // Node for traversal
    /* Iterate over characters in s one by one, building prefix string */ 
    for(auto &ch : s){
        while((traverse->c != ch) && (traverse->right != nullptr)) traverse = traverse->right;
        /* Check if traverse is at proper character */
        if(traverse->c != ch) break;

        prefix += ch;
        traverse = traverse->down;
    }

    return prefix;
}

int DLB::get(std::string s){
    /**
     * Fetches key of given string in trie
     * Throws invalid_argument exception if s not
     * in trie
     * 
     * @param s String to retrieve key for
     * @returns Key of given string
     * @throws invalid_argument exception if s not in trie 
    */

    /* Iterate over the characters in s one by one */
    std::shared_ptr<DLB_Node> traverse(head); // Node for traversal
    std::shared_ptr<DLB_Node> holds_final; // Node that will be used to return key
    for(auto &ch : s){
        while((traverse->c != ch) && (traverse->right != nullptr)) traverse = traverse->right;
        /* Check if traverse is at proper character */
        if(traverse->c != ch) throw std::invalid_argument("String not in trie");
        holds_final = traverse;
        traverse = traverse->down;
    }

    return holds_final->key;
}
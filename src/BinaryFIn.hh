#ifndef BINARY_F_IN
#define BINARY_F_IN

#include<iostream>
#include<fstream>
#include<string>

class BinaryFIn{
    private:
        std::ifstream file; // file input stream
        unsigned char buffer{}; // unsigned buffer to maintain
        int n; // number of bits remaining in buffer
        bool is_initialized; // flag to keep track of initialization
        bool at_eof; // flag to check if at eof
        void fill_buffer();
        char read_bit();

    public:
       BinaryFIn();
       void initialize(std::string file_name);
       void close();
       char read_char();
       short read_short();
       int read_int();
       long read_long();
       int read_r(const int r); 
       std::string read_string();
         
};

#endif
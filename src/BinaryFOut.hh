#ifndef BINARY_F_OUT
#define BINARY_F_OUT

#include <iostream>
#include <fstream>
#include <string>

class BinaryFOut{
    private:
        std::ofstream file; // file to write to
        unsigned char buffer; // 1-byte buffer to maintain
        int n;  // number of bits remaining in buffer
        bool is_initialzied; // flag to check initialization
        void write_bit(bool bit);
        void write_byte(char byte);
        void clear_buffer();
    
    public:
        BinaryFOut();
        void initialize(std::string file_name);
        void flush();
        void close();
        void write(bool bit); // write single bit
        void write(char byte); // write single byte
        void write(short dbyte); // write 16 bits (2 bytes or "d"ouble byte)
        void write(int w, int r); // write w which has r bits
        void write(int qbyte); // write 32 bits (4 bytes or "q"uad byte)
        void write(long obyte); // write 64 bits (8 bytes or "o"cto byte)

};

#endif
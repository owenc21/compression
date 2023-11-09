#ifndef LZW_COMP
#define LZW_COMP

#include <string>

class LZW{
    private:
        const int R = 256; // Number of input characters
        const int L = 4096; // Number of codewords (2^W)
        const int W = 12; // Codeword width
        std::string file;

    public:
        LZW() = delete; // Prevent default constructor
        LZW(std::string file_name); // Constructor with file to compress specified
        void compress();
        void expand();
};

#endif
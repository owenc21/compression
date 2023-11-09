#include <iostream>
#include <string>

#include "src/LZW.hh"

int main(int argc, char** argv){
    // if(argc < 2) return -1;

    // LZW lzw(argv[1]);

    // if(argv[2] == "compress"){
    //    lzw.compress(); 
    // }
    // else if(argv[2] == "expand"){
    //     lzw.expand();
    // }
    // else if(argv[2] == "b"){
    //     lzw.compress();
    //     lzw.expand();
    // }

    LZW lzw("big.txt");

    lzw.compress();
    lzw.expand();
}
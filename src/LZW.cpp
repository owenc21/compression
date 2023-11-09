/**
 * Implementation of LZW compression algorithm
 * wrapped in LZW class
 * Compresses with fixed-length codewords
 * Supports loss-less compression and expansion of
 * any file
 * 
 * Based off of LZW.java
 * https://algs4.cs.princeton.edu/55compression/LZW.java.html
 * 
 * DEPENDENCIES:
 *  DLB
 *  BinaryFIn
 *  BinaryFOut
*/

#include <stdexcept>
#include <string>

#include "DLB.hh"
#include "BinaryFIn.hh"
#include "BinaryFOut.hh"

#include "LZW.hh"

LZW::LZW(std::string file_name){
    /**
     *  Only accepted constructor
     *  Takes name of file to compress and initializes fields 
    */

    file = file_name;
}

void LZW::compress(){
    /**
     * Compresses the given file using LZW
     * compression algorithm
     * Outputs the compressed file as "compress.lzw"
    */

    /* Initialize file I/O objects */
    BinaryFIn file_in;
    file_in.initialize(file);
    BinaryFOut file_out;
    file_out.initialize("compress.lzw");

    DLB st; // Symbol table

    /* Initialize symbol table */
    for(int i=0; i<R; ++i){
        st.put(static_cast<char>(i), i);
    }
    int code = R+1;

    std::string input(file_in.read_string());
    while(input.length() > 0){
        std::string s = st.longest_prefix_of(input); // prefix match s
        file_out.write(st.get(s), W); // output s's encoding
        int t = s.length();
        if(t < input.length() && code < L){
            st.put(input.substr(0, t+1), code);
            code++;
        }
        input = input.substr(t, input.length()-t);
    }

    file_out.write(R, W);
    file_out.close();
}

void LZW::expand(){
    /**
     * Expands a compressed file using lossless
     * decompression algorithm
     * Assumes compressed file exists in directory as
     * "compress.lzw"
     * Outputs expanded file as "expanded.txt"
    */

    /* Initialize file I/O objects */
    BinaryFIn file_in;
    file_in.initialize("compress.lzw");
    BinaryFOut file_out;
    file_out.initialize("expanded.txt");

    std::string st [L]; // Symbol table
    int i; // Next available codeword value

    /* Initialize symbol table */
    for(i=0; i<R; ++i){
        st[i] = std::string(1, static_cast<char>(i));
    }
    st[i] = ""; // (unused) lookahead for EOF
    i++;

    int codeword = file_in.read_r(W);
    if(codeword == R) return; // Empty message

    std::string val = st[codeword]; 

    while(true){
        if(file_in.get_eof()) break; // Break if at EOF
        file_out.write(val);
        codeword = file_in.read_r(W);
        std::string s = st[codeword];
        if (i == codeword) s = val + val.at(0); // Special case
        if(i < L) st[i] = val + s.at(0);
        i++;
        val = s;
    }

    file_out.close();
}
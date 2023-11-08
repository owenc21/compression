/**
 * Implementation of binary file I/O
 * Able to read next n bits of a file where n is some multiple of 8 (up to 64)
 * Implementation based on BinaryStdIn.java
 * https://introcs.cs.princeton.edu/java/stdlib/BinaryStdIn.java.html
 * 
*/

#include "BinaryFIn.hh"
#include <string>
#include <iostream>
#include <fstream>


BinaryFIn::BinaryFIn(){
    n = -1;
    buffer = -1;
    at_eof = false;
    is_initialized = false;
}

void BinaryFIn::initialize(std::string file_name){
    /**
     * Private member for initializing the object
     * Opens the given file in ios::binary mode
     * Sets buffer and number of bits to 0
     * Marks as initialized
     * 
     * @param file_name Name of file to open
    */

    file.open(file_name, std::ios::binary);

    // Ensure that file was successfully opened
    if(!file.is_open()){
        std::cout << "Error opening file. Does it exist?" << std::endl;
        n = -1;
        buffer = -1;
        at_eof = false;
        is_initialized = false;
        return;
    }

    n = 0;
    buffer = 0;
    is_initialized = true;
}

void BinaryFIn::fill_buffer(){
    /**
     * Private member for filling the buffer with
     * the next byte from the file
     * Assumes will NOT be called when already at EOF
    */

    char input;
    file.get(input);

    buffer = static_cast<char>(input);
    n = 8;
    if(file.eof()) at_eof = true;
}

void BinaryFIn::close(){
    /**
     * Closes the file ifsream
    */
    if(!is_initialized) return;

    try{
        file.close();
        is_initialized = false;
        n = -1;
        buffer = -1;
        at_eof = false;
    }
    catch(const std::ifstream::failure& e){
       std::cout << "Failed to close file\n" << e.what() << std::endl; 
    }
}

char BinaryFIn::read_bit(){
    /**
     * Gets the next bit of data from file and returns as
     * 8-bit char
     * 
     * @returns Next bit of data from file as char
     * @throws ifstream::failure if at end of file
    */

    if(at_eof){
        throw(std::ifstream::failure("At end of file"));
    }

    char c = (buffer >> n) & 0x1;
    if(n == 0){
        n = 8;
        fill_buffer();
    }
    return c;
}

char BinaryFIn::read_char(){
    /**
     * Gets the next 8 bits of data from file and returns as
     * an 8-bit char
     * 
     * @returns Next 8 bts of data from file as char
     * @throws  ifstream::failure if at end of file (EOF set true)  
    */
   
    if(at_eof){
        throw(std::ifstream::failure("At end of file"));
    }

    int c = 0; // need signed type for arithmetic right shift

    // Check case for perfect bit-alignment
    if(n == 8){
        c = buffer;
        fill_buffer();
        return c;
    }

    // Need to perform bit-alignment for remaining buffer
    int offset = 8-n; // offset of remaining buffer from 8 bits 
    c = buffer;
    c <<= offset;
    fill_buffer();
    if(at_eof){
        throw(std::ifstream::failure("At end of file"));
    }
    n = 8-offset; // set n to old value
    c |= buffer >> n; // only want upper offset bits of buffer

    return c;
}

short BinaryFIn::read_short(){
    /**
     * Gets the next 16 bits of data from file and returns as
     * a 16-bit short
     * 
     * @returns Next 16 bits of data from file as short
     * @throws  ifstream::failure if at end of file
    */

    if(at_eof){
        throw(std::ifstream::failure("At end of file"));
    }

    short c = 0;

    // Just need to read 2 chars into the short
    for(int i=0; i<2; i++){
        c <<= 8;
        c |= read_char();
    }

    return c;
}

int BinaryFIn::read_int(){
    /**
     * Gets the next 32 bits of data from file as
     * a 32-bit int
     * 
     * @returns Next 32 bits of data from file as int
     * @throws ifstream::failure if at end of file
    */

    if(at_eof){
        throw(std::ifstream::failure("At end of file"));
    }

    int c = 0;

    // Need to read 2 shorts into the int
    for(int i=0; i<2; i++){
        c <<= 16;
        c |= read_short();
    }

    return c;
}

long BinaryFIn::read_long(){
    /**
     * Gets the next 64 bits of data from file as
     * a 64-bit long
     * 
     * @returns Next 64 bits of data from file as long
     * @throws  ifstream::failure if at end of file 
    */

    if(at_eof){
        throw(std::ifstream::failure("At end of file"));
    }

    long c = 0;

    // Need to read 2 ints into the long
    for(int i=0; i<2; i++){
        c <<= 32;
        c |= read_int();
    }

    return c;
}

int BinaryFIn::read_r(const int r){
    /**
     * Gets the next r bits of data from file as
     * 32-bit int
     * 
     * @param r     int between 1 and 32 to specify number of bits to get
     * @returns     Next r bits of data from file as int
     * @throws      ifstream::failure if at end of file
     * @throws      invalid_argument if r not between 1 and 32
    */

    if(!(r>=1 && r<=32)){
        throw(std::invalid_argument("Number of bits requested must be between 1 and 32"));
    } 

    // Handle optimized cases
    switch(r){
        case 8:
            return read_char();
            break;
        case 16:
            return read_short();
            break;
        case 32:
            return read_int();
            break;
    }

    // Read bits in individually
    int c = 0;
    for(int i=0; i<r; i++){
        c <<= 1;
        c |= read_bit();
    }

    return c; 
}

std::string BinaryFIn::read_string(){
    /**
     * Gets the remaining bytes of data from file
     * as a string
     * 
     * @returns Remaining data of file as string
     * @throws  ifstream::failure if at end of file 
    */

    if(at_eof){
        throw(std::ifstream::failure("At end of file"));
    }

    std::string c;
    // Iterate over every character in file and build string to return
    while(!at_eof){
        char next = read_char();
        c.append(1,next);
    }

    return c;
}

bool BinaryFIn::get_eof(){
    /**
     * Public getter method to return end-of-file
     * (EOF) flag status
     * 
     * @returns EOF flag
    */

    return at_eof;
}
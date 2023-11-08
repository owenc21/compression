#include <iostream>
#include <fstream>
#include "BinaryFOut.hh"

BinaryFOut::BinaryFOut(){
    /**
     * Default constructor to initialize all values
     * and set "initialized" flag to false
    */
    buffer = -1;
    n = -1;
    is_initialzied = false;
}

void BinaryFOut::initialize(std::string file_name){
    /**
     * Initializer for object
     * Opens file file_name in ios::out mode
     * Initializes members
     * 
     * @param file_name Name of file to output to
    */

    file.open(file_name, std::ios::binary);

    if(!file.is_open()){
        std::cout << "Error opening the given file." << std::endl;
    }

    n = 0;
    buffer = 0;
    is_initialzied = true;
}

void BinaryFOut::close(){
    /**
     * Closes the file stream and sets object as unitialized
    */

    if(!is_initialzied) return;

    try{
        file.close();
        n = -1;
        buffer = -1;
        is_initialzied = false;
    } 
    catch(const std::ifstream::failure& e){
       std::cout << "Failed to close file\n" << e.what() << std::endl; 
    }
}

void BinaryFOut::write_bit(bool bit){
    /**
     * Outpits given bit to file
     * 
     * @param bit   bool corresponding to bit to write (f = 0, t = 1)
     */  

    // Convert bool into char value (0 or 1)
    char c = (bit) ? 1 : 0;

    buffer <<= 1;
    buffer |= c;
    ++n;

    if(n == 8) clear_buffer();
}

void BinaryFOut::write_byte(char byte){
    /**
     * Outputs given byte to file
     * 
     * @param byte  char corresponding to byte to write
    */

    // Optimize when buffer at 0 bits (can write byte directly)
    if(n == 0){
        buffer = static_cast<unsigned char>(byte);
        n = 8;

    }
}

void BinaryFOut::clear_buffer(){
    /**
     * Writes remaining bits in buffer to file
     * Pads with 0s if necessary
     * Primary member for interfacing with file
    */

    if(!is_initialzied) return;
    if(n == 0) return;

    if(n > 0) buffer <<= (8 - n);
    
    try{
        file.put(static_cast<char>(buffer));
        n = 0;
        buffer = 0;
    }
    catch(const std::ifstream::failure& e){
        std::cout << "Failed to clear buffer\n" << e.what() << std::endl;
    }
}

void BinaryFOut::flush(){
    /** 
     * Flushes the file contents
    */

    clear_buffer();

    try{
        file.flush();
    }
    catch(const std::ifstream::failure& e){
        std::cout << "Failed to flush to file\n" << e.what() << std::endl;
    }
}
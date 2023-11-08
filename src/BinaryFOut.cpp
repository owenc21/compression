/**
 * Implementtaion of binary file output
 * Maintain a buffer of 8 bits to output to a binary file
 * Implementation based on BinarySTDOut.java
 * https://introcs.cs.princeton.edu/java/stdlib/BinaryStdOut.java.html
 * 
*/
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

    file.open(file_name, std::ios::out|std::ios::binary);

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
        clear_buffer();
        return;
    }

    // Write bit by bit
    for(int i=0; i<8; ++i){
        bool bit = (byte&0x80 == 1) ? 1 : 0;
        byte <<= 1;
        write_bit(bit);
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
        file.write(reinterpret_cast<const char*>(&buffer), sizeof buffer);
        // file.put(static_cast<char>(buffer));
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

void BinaryFOut::write(bool bit){
    /**
     * Public member to add the given bit
     * (represented as bool) to the buffer
     * 
     * @param bit   bool representing bit to write
    */

    write_bit(bit);
}

void BinaryFOut::write(char byte){
    /**
     * Public member to add the given byte
     * (represented as char) to the buffer
     * 
     * @param byte  char representing byte to write
    */

    write_byte(byte);
}

void BinaryFOut::write(short dbyte){
    /**
     * Public member to add given 16 bits
     * ("d"ouble byte, represented as char) to
     * the buffer
     * 
     * @param dbyte short representing the 16 bits to write
    */

    // Make unsigned for right shift
    unsigned short u_dbyte = static_cast<unsigned short>(dbyte);
    write_byte(static_cast<char>(u_dbyte >> 8));
}

void BinaryFOut::write(int qbyte){
    /**
     * Public member to add given 32 bits
     * ("q"uad byte, represented as int) to
     * the buffer
     * 
     * @param qbyte int representing the 32 bits to write
    */

    // Make unsigned for right shifting
    unsigned int u_qbyte = static_cast<unsigned int>(qbyte);

    write_byte(static_cast<char>(u_qbyte >> 24));
    write_byte(static_cast<char>(u_qbyte >> 16));
    write_byte(static_cast<char>(u_qbyte >> 8));
    write_byte(static_cast<char>(u_qbyte));
}

void BinaryFOut::write(long obyte){
    /**
     * Public member to add given 64 bits
     * ("o"cto byte, reprsented as a long) to
     * the buffer
     * 
     * @param obyte long representing the 64 bits to write
    */

    // Make unsigned for right shifting
    unsigned long u_obyte = static_cast<unsigned long>(obyte);

    write(static_cast<int>(u_obyte >> 32));
    write(static_cast<int>(u_obyte));
}

void BinaryFOut::write(int c, int r){
    /**
     * Public member to add given r bits
     * from int c
     * 
     * @param c value whose bits are to be written
     * @param r number of bits (big endian) of importance in c
    */
    
    // Optimize 32 bit case
    if(r == 32){
        write(c);
        return;
    }

    // Make unsiged for right shifting
    unsigned int w = static_cast<unsigned int>(c);
    for(int i=0; i<r; i++){
        bool bit = ((w >> (r -  i - 1)) & 1) == 1;
        write_bit(bit);
    }
}
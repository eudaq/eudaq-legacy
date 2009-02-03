//#include "eudaq/PluginManager.hh"
//#include "eudaq/RawDataEvent"
#include <iostream>
#include <cstdio>
#include <stdint.h>
#include <errno.h>
#include <cstring>

// initial buffer size is 2MB 
#define INITIAL_BUFFER_SIZE 0x200000

// helper function to interpert a big endian byte sequence as 32 bit words
uint32_t read32bitword(unsigned char const * const buffer)
{
    return  static_cast<uint32_t> (buffer[0])        |
	    static_cast<uint32_t> (buffer[1]) <<  8  |
	    static_cast<uint32_t> (buffer[2]) << 16  |
	    static_cast<uint32_t> (buffer[3]) << 24;
}

int main(unsigned int argc, char * argv[])
{
    if ( (argc != 2) && (argc != 3) )
    {
	std::cout << "usage: "<<argv[0]  << " infile.dat [outfile.slcio]" 
		  << std::endl;
	return 2;
    }

    // open file for reading
    std::FILE * infile = fopen( argv[1] , "r");

    // size of the memory buffer
    size_t buffersize = INITIAL_BUFFER_SIZE;
    unsigned char * inputbuffer = new unsigned char[INITIAL_BUFFER_SIZE];

    // some input variables we need during data processing
    unsigned int dataformat = 0;
    unsigned int runnumber  = 0;

    bool continue_reading = true;

    // read continuoutsly from file.
    // Loop will exit when end of file is reached
    // or an error occurs
    while (continue_reading)
    {
	// read one 4 byte word from the file,
	// it contains the length of the next blockINITIAL_BUFFER_SIZE
	if  ( fread(inputbuffer, 1, 4, infile) != 4 )
	{
	    // word could not be read , end of file is reached
	    break;
	}
	
	// read the first 32bit word. It is the (exclusive) number of 32-bit words
	size_t blocklength =  read32bitword(inputbuffer);
	
	// check if input buffer is large enough
	if ( buffersize < (blocklength+1)*4 )
	{
	    // delete inputbuffer and reacclocate sufficient memory
	    delete[] inputbuffer;
	    inputbuffer = new unsigned char[ (blocklength + 1) * 4 ];
	    std::cout << "DEBUG: allocated "<<(blocklength + 1) * 4 << " for inputbuffer"<<std::endl;
	    
	    // set the first four bytes to the length
	    inputbuffer[0] = static_cast<unsigned char>(blocklength & 0xFF) ;
	    inputbuffer[1] = static_cast<unsigned char>((blocklength & 0xFF00) >> 8 ) ;
	    inputbuffer[2] = static_cast<unsigned char>((blocklength & 0xFF0000) >> 16 ) ;
	    inputbuffer[3] = static_cast<unsigned char>((blocklength & 0xFF000000) >> 24 ) ;
	} 
	
	// read the complete block into memory, starting after the first 4 bytes which have
	// already been read
        size_t bytesread = fread(inputbuffer + 4, 1, blocklength*4 , infile);
	if  ( bytesread != blocklength*4 )
	{
	    // word could not be read , end of file is reached
	    
	    if ( ferror(infile) )
	    {
		std::cerr << "Error reading block, cannot read "<< blocklength*4<< " bytes from input file" << std::endl;   
		std::cerr << "Could only read "<< bytesread << " bytes" << std::endl;
		std::cerr <<std::strerror(errno)<< std::endl;
	    }
	    return 1;
	}
//	else
//	    std::cout << "DEBUG: Reading block with "<< blocklength*4<< " bytes from input file" << std::endl;
	

	// interpret block identifier
	unsigned int blockid = read32bitword(inputbuffer + 8);

	switch (blockid)
	{	
	    case 0x11111111: // start of run block
		dataformat = read32bitword(inputbuffer + 12);
		runnumber  = read32bitword(inputbuffer + 16);
		// create runheader and write it to the lcio file
		std::cout << "Starting run "<<runnumber << " with data format "<<dataformat<<std::endl;
		break;
	    case 0x22222222: // rawdata event
		// genereate eudaq event, convert to lcio and add lcevent to file
//		std::cout << "DEBUG: Reading event with "<<(blocklength+1) *4 << std::endl;
		std::cout << "Reading event number "<< read32bitword(inputbuffer + 12) 
			  << std::endl;
		break;
	    case 0x33333333: // end of run format
		// quit the loop?
		
		// genereate eudaq event, convert to lcio and add lcevent to file
		std::cout << "Ending run number "<< runnumber << std::endl;
		continue_reading = false;
		break;
	    case 0x11112222: // pause of run
		std::cout << "DEBUG: pausing run"<< std::endl;
		break;
	    case 0x11113333: // continue run
		// just ignore them
		std::cout << "DEBUG: resuming run"<< std::endl;
		break;
	    case 0x44444444: // begin of file which continues data from another file
	    case 0x55555555: // end of file, in continued in another file
		std::cout << "Warning: splitting of file not supported yet! Exiting" << std::endl;
		continue_reading = false;
		break;
	    default:
		std::cerr << "Error: Unknown block ID " <<std::hex << blockid << std::endl;
		
	}
    }

    // close the file
    fclose(infile);

    // release the memory of the inputbuffer
    delete[] inputbuffer;

    return 0;
}

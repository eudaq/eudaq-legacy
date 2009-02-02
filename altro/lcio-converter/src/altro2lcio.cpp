//#include "eudaq/PluginManager.hh"
//#include "eudaq/RawDataEvent"
#include <iostream>

int main(unsigned int argc, char * argv[])
{
    if ( (argc != 2) || (argc != 3) )
    {
	std::cout << "usage: "<<argv[0]  << " infile.dat [outfile.slcio]" 
		  << std::endl;
    }

  

    return 0;
}

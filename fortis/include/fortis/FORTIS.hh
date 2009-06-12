// put this next definition somewhere more sensible....
#define FORTIS_DATATYPE_NAME "FORTIS"
#define WORDS_IN_ROW_HEADER 2


struct ExecutableArgs
{
  std::string dir;
  std::string filename;
  std::string args;

  // Start of "Brown Paper Bag Code"
  //char dir[1000];
  //char filename[1000];
  //char args[1000];
  // End of embarassing code....
};

void* startExecutableThread(void * args);

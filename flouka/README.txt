

BUILDING THE LIBRARY
===============================================================================
1. Open a shell (command prompt).
2. Change directory to point to the project folder.
3. Build the library by running the following command: 

  make clean all
  
4. The library will be created with name that follows lib*.a convention.
 
 
USING THE LIBRARY AS BINARIES
===============================================================================
1. Copy the built library to the location containing external libraries in your 
   application tree.
2. Copy all header files (flouka_wrapper.h is optional) to the location 
   containing external header files in your application tree.
3. Include the header files in your application source.
4. Adjust your makefiles to link with the library.    
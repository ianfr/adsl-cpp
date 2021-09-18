#pragma once
// https://www.boost.org/doc/libs/1_55_0/doc/html/interprocess/sharedmemorybetweenprocesses.html helpful

#include <iostream>
#include <string>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include "DataFrame.h"

using namespace boost::interprocess;

namespace adsl {

    // utility function to *compile* (not run) the data loading code
    void compileLoadDataCpp(std::string vcpkgParentFolder) {
        std::cout << "Compiling LoadData.cpp..." << std::endl;
        std::stringstream cmdStream;
        cmdStream << "\"" << "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat" << "\"" << " amd64" << " && ";
        cmdStream << "set INC_VCPKG=" << vcpkgParentFolder << "\\vcpkg\\installed\\x64-windows\\include" << " && ";
        cmdStream << "set LIB_VCPKG=" << vcpkgParentFolder << "\\vcpkg\\installed\\x64-windows\\lib" << " && ";
        cmdStream << "echo %INC_VCPKG% && echo %LIB_VCPKG% && ";
        cmdStream << "cl /EHsc /std:c++17 /I %INC_VCPKG% MemDB\\LoadData.cpp /link /LIBPATH:%LIB_VCPKG%";
        std::system(cmdStream.str().c_str());
    }

	void createShared() {
        //Remove shared memory on construction and destruction
        struct shm_remove
        {
            shm_remove() { shared_memory_object::remove("MySharedMemory"); }
            ~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
        } remover;

        //Create a shared memory object.
        shared_memory_object shm(create_only, "MySharedMemory", read_write);

        //Set size
        shm.truncate(1000);

        //Map the whole shared memory in this process
        mapped_region region(shm, read_write);

        //Write all the memory to 1
        std::memset(region.get_address(), 1, region.get_size());
	}

	void loadShared() {
        //Open already created shared memory object.
        shared_memory_object shm(open_only, "MySharedMemory", read_only);

        //Map the whole shared memory in this process
        mapped_region region(shm, read_only);

        //Check that memory was initialized to 1
        char* mem = static_cast<char*>(region.get_address());
        for (std::size_t i = 0; i < region.get_size(); ++i)
            if (*mem++ != 1)
                std::cout << "error checking memory" << std::endl;;   //Error checking memory
	}

}

#include <iostream>

#include "CPdhQuery.hpp"

int main(int argc, char** argv) {
    std::cout << "Hello " << argc << " args!" << std::endl;
    CPdhQuery pdhQuery(
        std::tstring(_T("\\Processor(_Total)\\Interrupts/sec"))
    );

    for(int i=0; i<50; ++i)
    {
        Sleep(500);
        DumpMap(pdhQuery.CollectQueryData());
    }
    return 0;
}





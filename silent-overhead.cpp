/// Std lib
#include <iostream>

// Windows
#pragma warning(push, 0)
#include <corecrt.h>
#include <Windows.h>
#pragma warning(pop)

// Repo code
#include "CPdhQuery.hpp"

int main(int argc, char** argv) {
    std::cout << "Hello " << argc << " args!" << std::endl;
    
    /*
    CPdhQuery pdhQuery(
        std::tstring(_T("\\Processor(_Total)\\Interrupts/sec"))
    );
    for(int i=0; i<50; ++i)
    {
        Sleep(500);
        DumpMap(pdhQuery.CollectQueryData());
    }
    */

    DWORD bufLength = 0;
    const DWORD detailLevel = PERF_DETAIL_WIZARD;
    PDH_STATUS objStatus = PdhEnumObjectsW(nullptr, nullptr, nullptr, &bufLength, detailLevel, TRUE);
    //std::cout << ManageApp::getPdhStatusMsg(objStatus);
    std::cout << "bufLength: " << bufLength << std::endl;
    std::wstring namebuf(bufLength, '\0');
    PDH_STATUS status = PdhEnumObjectsW(nullptr, nullptr, &namebuf[0], &bufLength, detailLevel, FALSE);
    //std::cout << ManageApp::getPdhStatusMsg(status);

    std::wcout << "namebuf: " << namebuf << std::endl;

    /*
    for (int i=0; i<(int)mszObjectList_len; i+=1) {
        if (mszObjectList[i] != '\0') {
            tcout << mszObjectList[i] << std::endl;
        }
    }*/

    std::cout << "Goodbye!" << std::endl;
    return 0;
}





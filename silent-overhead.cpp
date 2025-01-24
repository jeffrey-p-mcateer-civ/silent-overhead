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

    DWORD objectsLength = 0;
    const DWORD detailLevel = PERF_DETAIL_WIZARD;
    PDH_STATUS status;
    status = PdhEnumObjectsW(nullptr, nullptr, nullptr, &objectsLength, detailLevel, TRUE);
    //std::cout << ManageApp::getPdhStatusMsg(objStatus);
    std::cout << "objectsLength: " << objectsLength << std::endl;
    std::wstring namebuf(objectsLength, '\0');
    status = PdhEnumObjectsW(nullptr, nullptr, &namebuf[0], &objectsLength, detailLevel, FALSE);
    //std::cout << ManageApp::getPdhStatusMsg(status);

    //std::wcout << "namebuf: " << namebuf << std::endl;
    int last_i=0;
    for (int i=0; i<objectsLength; i+=1) {
        if (namebuf[i] == '\0') {
            std::wstring object_name( namebuf.substr(last_i, i-last_i) );
            std::wcout << object_name << std::endl;
            last_i = i+1;
            DWORD pcchCounterListLength = 0;
            DWORD pcchInstanceListLength = 0;
            status = PdhEnumObjectItemsW(
                nullptr, nullptr,
                object_name.c_str(),
                nullptr, &pcchCounterListLength,
                nullptr, &pcchInstanceListLength,
                detailLevel,
                0 // "This parameter must be zero"
            );
            std::cout << "  pcchCounterListLength: " << pcchCounterListLength << std::endl;
            std::cout << "  pcchInstanceListLength: " << pcchInstanceListLength << std::endl;

            std::wstring pcchCounterList(pcchCounterListLength, '\0');
            std::wstring pcchInstanceList(pcchInstanceListLength, '\0');

            status = PdhEnumObjectItemsW(
                nullptr, nullptr,
                object_name.c_str(),
                &pcchCounterList[0], &pcchCounterListLength,
                &pcchInstanceList[0], &pcchInstanceListLength,
                detailLevel,
                0 // "This parameter must be zero"
            );

            int last_counter = 0;
            for (int counter=0; counter < pcchCounterListLength; counter+=1) {
                if (pcchCounterList[counter] == '\0') {
                    std::wstring counter_name( pcchCounterList.substr(last_counter, counter-last_counter) );
                    std::wcout << "c> " << counter_name << std::endl;
                    last_counter = counter+1;
                }
            }
            int last_instance = 0;
            for (int instance=0; instance < pcchInstanceListLength; instance+=1) {
                if (pcchInstanceList[instance] == '\0') {
                    std::wstring instance_name( pcchInstanceList.substr(last_instance, instance-last_instance) );
                    std::wcout << "i> " << instance_name << std::endl;
                    last_instance = instance+1;
                }
            }

            std::cout << std::endl;
        }
    }

    /*
    for (int i=0; i<(int)mszObjectList_len; i+=1) {
        if (mszObjectList[i] != '\0') {
            tcout << mszObjectList[i] << std::endl;
        }
    }*/

    std::cout << "Goodbye!" << std::endl;
    return 0;
}






#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

// Std lib
#include <iostream>
#include <locale>
#include <codecvt>
#include <string>
#include <algorithm>
#include <cctype>

// Windows
#pragma warning(push, 0)
#include <corecrt.h>
#include <Windows.h>
#pragma warning(pop)

// Repo code
#include "CPdhQuery.hpp"

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter; // Windorks sux -_-

int main(int argc, char** argv) {
    
    std::wstring print_containing;
    if (argc > 1) {
        print_containing.append(converter.from_bytes(argv[1]));
    }
    std::transform(
        print_containing.begin(), print_containing.end(), print_containing.begin(),
        [](auto c){ return std::tolower(c); }
    );

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
    for (int i=0; i<(int)objectsLength; i+=1) {
        if (namebuf[i] == '\0') {
            std::wstring object_name( namebuf.substr(last_i, i-last_i) );
            std::wstring lower_object_name( namebuf.substr(last_i, i-last_i) );
            std::transform(
                lower_object_name.begin(), lower_object_name.end(), lower_object_name.begin(),
                [](auto c){ return std::tolower(c); }
            );
            last_i = i+1;

            if (lower_object_name.find(print_containing) == lower_object_name.npos) {
                continue; // search string is NOT in parameter
            }

            std::wcout << object_name << std::endl;
            
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
            for (int counter=0; counter < (int)pcchCounterListLength; counter+=1) {
                if (pcchCounterList[counter] == '\0') {
                    std::wstring counter_name( pcchCounterList.substr(last_counter, counter-last_counter) );
                    last_counter = counter+1;
                    if (counter_name.length() > 0) {
                        std::wcout << "c> " << counter_name << std::endl;
                    }
                }
            }
            int last_instance = 0;
            for (int instance=0; instance < (int)pcchInstanceListLength; instance+=1) {
                if (pcchInstanceList[instance] == '\0') {
                    std::wstring instance_name( pcchInstanceList.substr(last_instance, instance-last_instance) );
                    last_instance = instance+1;
                    if (instance_name.length() > 0) {
                        std::wcout << "i> " << instance_name << std::endl;
                    }
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





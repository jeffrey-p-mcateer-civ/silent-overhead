
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

// Std lib
#include <iostream>
#include <locale>
#include <codecvt>
#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>

// Windows
#pragma warning(push, 0)
#include <corecrt.h>
#include <Windows.h>
#pragma warning(pop)

// Repo code
#include "CPdhQuery.hpp"

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter; // Windorks sux -_-

int main(int argc, char** argv) {
    
    std::wstring object_search;
    if (argc > 1) {
        object_search.append(converter.from_bytes(argv[1]));
    }
    std::transform(
        object_search.begin(), object_search.end(), object_search.begin(),
        [](auto c){ return std::tolower(c); }
    );

    std::wstring value_search;
    if (argc > 2) {
        value_search.append(converter.from_bytes(argv[2]));
    }
    std::transform(
        value_search.begin(), value_search.end(), value_search.begin(),
        [](auto c){ return std::tolower(c); }
    );

    
    CPdhQuery pdhQuery1(
        std::tstring(_T("\\Processor(_Total)\\Interrupts/sec"))
    );
    for(int i=0; i<4; ++i)
    {
        Sleep(500);
        DumpMap(pdhQuery1.CollectQueryData());
    }
    
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

    CPdhQuery* pdhQuery;

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

            if (object_search.length() > 1 && lower_object_name.find(object_search) == lower_object_name.npos) {
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
                    std::wstring lower_counter_name( pcchCounterList.substr(last_counter, counter-last_counter) );
                    std::transform(
                        lower_counter_name.begin(), lower_counter_name.end(), lower_counter_name.begin(),
                        [](auto c){ return std::tolower(c); }
                    );
                    last_counter = counter+1;
                    if (value_search.length() > 0 && lower_counter_name.find(value_search) == lower_counter_name.npos) {
                        continue;
                    }
                    if (counter_name.length() > 0) {
                        std::wcout << "c> " << counter_name << std::endl;

                        std::wstringstream counter_q_ss;
                        counter_q_ss << "\\" << object_name << "\\" << counter_name;
                        std::wstring counter_q(counter_q_ss.str());

                        std::wcout << counter_q << std::endl;

                        
                        if (pdhQuery != nullptr) {
                            delete pdhQuery;
                        }

                        pdhQuery = new CPdhQuery(
                            //std::tstring( reinterpret_cast<TCHAR*>(counter_q.data()) )
                            std::tstring( _T("\\Processor(_Total)\\Interrupts/sec") )
                        );
                        
                        for(int x=0; x<5; x+=1)
                        {
                            Sleep(100);
                            //std::cout << "Before pdhQuery->CollectQueryData()" << std::endl;
                            auto data = pdhQuery->CollectQueryData();
                            //std::cout << "Before DumpMap(data)" << std::endl;
                            DumpMap(data);
                        }
                        
                    }
                }
            }
            int last_instance = 0;
            for (int instance=0; instance < (int)pcchInstanceListLength; instance+=1) {
                if (pcchInstanceList[instance] == '\0') {
                    std::wstring instance_name( pcchInstanceList.substr(last_instance, instance-last_instance) );
                    std::wstring lower_instance_name( pcchInstanceList.substr(last_instance, instance-last_instance) );
                    std::transform(
                        lower_instance_name.begin(), lower_instance_name.end(), lower_instance_name.begin(),
                        [](auto c){ return std::tolower(c); }
                    );
                    last_instance = instance+1;
                    if (value_search.length() > 0 && lower_instance_name.find(value_search) == lower_instance_name.npos) {
                        continue;
                    }
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





#pragma once

#include <libds/adt/sorts.h>
#include <libds/heap_monitor.h>
#include <algorithm>
#include <locale>

class SortComparators
{
private:
    //std::string abeceda = "aáäbcèdïeéfghiíjklmnòóôpqràsštuúvwxyız";
    std::string spoluhlasky = "bcèdïfghjkl¾åmnòpqràsštvwxz";

public:
    template<typename TypTemplate>
    void compareAlphabetical(ds::amt::ImplicitSequence<TypTemplate*>& sequenceForSort) {
        ds::adt::QuickSort<TypTemplate*> quickSorter;
        std::locale loc;

        quickSorter.sort(sequenceForSort, [&](TypTemplate* a, TypTemplate* b) {
            std::string string1 = a->getStopName();
            std::string string2 = b->getStopName();

            return std::lexicographical_compare(
                string1.begin(), string1.end(),
                string2.begin(), string2.end(),
                [&](char c1, char c2) {
                    if (std::tolower(c1, loc) < std::tolower(c2, loc)) {
                        //return (-1);
                        return true;
                    }
                    if (std::tolower(c1, loc) > std::tolower(c2, loc)) {
                        //return (1);
                        return false;
                    }
                    return false;
                }
            );

          
        });
    }

    

    int countConsonants(std::string s) {
        int count = 0;
        for (char c : s) {
            char lowerC = std::tolower(c);
            if (spoluhlasky.find(lowerC) != std::string::npos) {
                count++;
            }
        }
        return count;
    }

    template<typename TypTemplate>
    void compareConsonantCount(ds::amt::ImplicitSequence<TypTemplate*>& sequenceForSort) {
        ds::adt::QuickSort<TypTemplate*> quickSorter;

        
        quickSorter.sort(sequenceForSort, [&](TypTemplate* a, TypTemplate* b) {

            std::string string1 = a->getStopName();
            std::string string2 = b->getStopName();

            int countA = countConsonants(string1);
            int countB = countConsonants(string2);
            if (countA < countB) return true;
            if (countA > countB) return false;
            return false;
        });

        
    }
};
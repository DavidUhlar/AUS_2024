#pragma once

#include <libds/adt/sorts.h>
#include <libds/heap_monitor.h>


class SortComparators
{
private:
    std::string abeceda = "a·‰bcËdÔeÈfghiÌjklmnÚÛÙpqr‡sötùu˙vwxy˝zû";
    std::string spoluhlasky = "bcËdÔfghjklæÂmnÚpqr‡sötùvwxzû";

public:
    

    
    std::size_t getCharPosition(char c) {
        
        char lowerC = std::tolower(c);
        std::size_t pos = abeceda.find(lowerC);
        return (pos != std::string::npos) ? pos : abeceda.size();
    }

    template<typename TypTemplate>
    void compareAlphabetical(ds::amt::ImplicitSequence<TypTemplate*>& sequenceForSort) {

        ds::adt::QuickSort<TypTemplate*> quickSorter;
        
        //ds::adt::BubbleSort<TypTemplate*> quickSorter;
        quickSorter.sort(sequenceForSort, [&](TypTemplate* a, TypTemplate* b) {

            std::size_t len1 = a->getStopName().length();
            std::size_t len2 = b->getStopName().length();

            std::size_t minLen;
            if (len1 < len2) {
                minLen = len1;
            }
            else {
                minLen = len2;
            }

            std::string string1 = a->getStopName();
            std::string string2 = b->getStopName();

            for (std::size_t i = 0; i < minLen; ++i) {
                std::size_t pos1 = getCharPosition(string1[i]);
                std::size_t pos2 = getCharPosition(string2[i]);

                if (pos1 != pos2) {
                    if (pos1 < pos2)
                    {
                        //return (-1);
                        return true;
                    }
                    if (pos1 > pos2)
                    {
                        //return 1;
                        return false;
                    }
                    //return 0;
                }
            }
            if (len1 != len2) {
            
                if (len1 < len2)
                {
                    //return (-1);
                    return true;
                }
                if (len1 > len2)
                {
                    //return 1;
                    return false;
                }
                
            }
            //return 0;
            return false;
            
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

        //ds::adt::BubbleSort<TypTemplate*> quickSorter;
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
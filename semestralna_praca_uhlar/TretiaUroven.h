#pragma once

#include <libds/heap_monitor.h>
#include <libds/adt/table.h>
#include <libds/amt/implicit_sequence.h>

class TableHandler
{
private:

    //ds::amt::MultiWayExplicitHierarchy<HierarchyNode*> hierarchy_;
    ds::amt::ImplicitSequence<ds::amt::ImplicitSequence<Zastavka*>*>* tableSequence;

public:



    template<typename InputIterator>
    ds::adt::HashTable<std::string, ds::amt::ImplicitSequence<Zastavka*>*> loadTable(InputIterator beginIterator, InputIterator endIterator) {

        ds::adt::HashTable<std::string, ds::amt::ImplicitSequence<Zastavka*>*> tableOfStops;

        tableSequence = new ds::amt::ImplicitSequence<ds::amt::ImplicitSequence<Zastavka*>*>;

        
        for (auto i = beginIterator; i != endIterator; ++i) {
            
            auto data = (*i);
            std::string tableKey = data->getStopName();

            if (!tableOfStops.contains(tableKey))
            {
                ds::amt::ImplicitSequence<Zastavka*>* valueSequence = new ds::amt::ImplicitSequence<Zastavka*>;
                valueSequence->insertLast().data_ = data;
                tableSequence->insertLast().data_ = valueSequence;
                tableOfStops.insert(tableKey, valueSequence);
            }
            else {

                auto existingKey = tableOfStops.find(tableKey);
                existingKey->insertLast().data_ = data;

                
            }

        }
        
        
        return tableOfStops;
    }

    template<typename TableToFilter>
    void filterTable(TableToFilter table, std::string searchString) {
    
        if (table.contains(searchString)) {
            auto size = table.find(searchString)->size();
            std::cout << "Pocet vyfiltrovanych: " << size << std::endl;
            std::cout << "\n-------------------------" << std::endl;
            for (size_t i = 0; i < size; i++) {
                std::cout << table.find(searchString)->access(i)->data_->getStopId() << "  | "
                    << table.find(searchString)->access(i)->data_->getStopName() << "  | "
                    << table.find(searchString)->access(i)->data_->getStopSite() << "  | "
                    << table.find(searchString)->access(i)->data_->getLatitude() << "  | "
                    << table.find(searchString)->access(i)->data_->getLongitude() << "  | "
                    << table.find(searchString)->access(i)->data_->getSysCode() << "  | "
                    << table.find(searchString)->access(i)->data_->getSystem() << "  | "
                    << table.find(searchString)->access(i)->data_->getMuni() << "  \n";
            }
            std::cout << "\n-------------------------" << std::endl;
        }
        else {
            std::cout << "Nespravny vstup\n" << std::endl;
        }
        std::cout << "\n" << std::endl;
    }

    
    void cleanUp() {
        for (auto i = tableSequence->begin(); i != tableSequence->end(); ++i) {
            
            auto data = (*i);
            data->size();
            delete (*i);
        }
        delete tableSequence;
    }
};
#pragma once
#include <iostream>
#include <libds/heap_monitor.h>
#include "StvrtaUroven.h"

class HierarchyNode
{
public:



    std::string getStopName() const { return name_; }
    Zastavka* getZastavka() const { return zastavka_; }
    bool getIsZastavka() { return isZastavka_; }

    void setZastavka(Zastavka* zastavka) { zastavka_ = zastavka; }
    void setName(const std::string& name) { name_ = name; }
    void setIsZastavka(bool value) { isZastavka_ = value; }


private:
    Zastavka* zastavka_;
    std::string name_;
    bool isZastavka_;


};

class HierarchyHandler
{
private:

    ds::amt::MultiWayExplicitHierarchy<HierarchyNode*> hierarchy_;
    ds::amt::ImplicitSequence<HierarchyNode*> nodeSeq;
    int datasetNumber_ = 0;

public:


    
    void createHierarchy() { 
        
        hierarchy_.emplaceRoot();
        
    }
    

    template<typename InputSequence>
    void loadHierarchy(InputSequence inputSequence) {

        
        HierarchyNode* systemData = new HierarchyNode;
        nodeSeq.insertLast().data_ = systemData;
        systemData->setName(inputSequence.accessFirst()->data_->getSystem());
        systemData->setIsZastavka(false);

        auto& systemBlock = hierarchy_.emplaceSon(*hierarchy_.accessRoot(), datasetNumber_);
        
        
        
        systemBlock.data_ = systemData;
        datasetNumber_++;
        
        //index syna v systeme
        int muniIndex = 0;
        std::string tempMuni = "";

        //index syna v obci
        int stopIndex = 0;
        
        
        
        for (auto i = inputSequence.begin(); i != inputSequence.end(); ++i) {
            auto* data = *i;


           
            if (tempMuni != data->getMuni())
            {
                HierarchyNode* muniData = new HierarchyNode;
                nodeSeq.insertLast().data_ = muniData;
                muniData->setName(data->getMuni());
                muniData->setIsZastavka(false);

                
                stopIndex = 0;
                tempMuni = data->getMuni();
                auto& muniBlock = hierarchy_.emplaceSon(systemBlock, muniIndex);


                muniBlock.data_ = muniData;
                

                muniIndex++;

                HierarchyNode* synData = new HierarchyNode;
                nodeSeq.insertLast().data_ = synData;
                synData->setName(data->getStopName());
                synData->setZastavka(data);
                synData->setIsZastavka(true);

                auto& syn = hierarchy_.emplaceSon(muniBlock, stopIndex);

                syn.data_ = synData;

                

            }
            else
            {
                
                HierarchyNode* synData = new HierarchyNode;
                nodeSeq.insertLast().data_ = synData;
                synData->setName(data->getStopName());
                synData->setZastavka(data);
                synData->setIsZastavka(true);

                auto& muniBlock = *hierarchy_.accessSon(systemBlock, muniIndex - 1); 
                auto& syn = hierarchy_.emplaceSon(muniBlock, stopIndex);


                syn.data_ = synData;
                
            }

            
            stopIndex++;

        }
        
    }

    void printHierarchy() {
        auto root = hierarchy_.accessRoot();

        
        for (size_t k = 0; k < root->sons_->size(); k++) {
            std::cout << "\n " << std::endl;
            std::cout << "\n " << std::endl;
            std::cout << "\n " << std::endl;
            std::cout << "\n------------------------- " << std::endl;
            
            auto systemBlock = hierarchy_.accessSon(*root, k);
            std::cout << "\n " << systemBlock->data_->getStopName() << std::endl;
            
            
                for (size_t i = 0; i < systemBlock->sons_->size(); i++) {
                    auto muniBlock = hierarchy_.accessSon(*systemBlock, i);
                    
                        std::cout << "\n---------------------------------------------------------------------------------------- " << std::endl;
                        std::cout << "zastavka tatko " << i << ": " << muniBlock->data_->getStopName() << std::endl;

                        for (size_t j = 0; j < muniBlock->sons_->size(); j++) {
                            auto stopBlock = hierarchy_.accessSon(*muniBlock, j);
                            std::cout << "synak " << j << ": " << stopBlock->data_->getStopName() << std::endl;
                        }
                }
        }
        std::cout << root->sons_->size() << std::endl;
    }
    
    ds::amt::MultiWayExplicitHierarchy<HierarchyNode*> getHierarchy() { return hierarchy_; }


    void cleanUp() {
        //hierarchy_;
        //for (auto i = hierarchy_.begin(); i != hierarchy_.end(); ++i) {
        //    delete (*i);
        //    //delete (*i);
        //}
        for (auto i = 0; i < nodeSeq.size(); i++)
        {
            delete nodeSeq.access(i)->data_;
        }
        nodeSeq.clear();
        hierarchy_.clear();
    }

    void iterateHierarchy() {
        std::cout << " \n " << std::endl;
        ds::amt::MultiWayExplicitHierarchyBlock<HierarchyNode*>* actualPosition = hierarchy_.accessRoot();
        int position = -1;
        
        
        while (true)
        {
            ds::amt::Hierarchy<ds::amt::MultiWayExplicitHierarchyBlock<HierarchyNode*>>::PreOrderHierarchyIterator currentPositionStart_(&hierarchy_, actualPosition);
        
            
            std::cout << "\n---------------------Parent--------------------------- \n" << std::endl;
            
            if (position <= 0)
            {
                std::cout << "je Root hierarchy" << std::endl;
            }
            else 
            {
                std::cout << hierarchy_.accessParent(*actualPosition)->data_->getStopName() << std::endl;
            }
            std::cout << "\n------------------------------------------------------ " << std::endl;
            

            
            std::cout << "\n-------------------Aktualna pozicia------------------- \n" << std::endl;
            
            if (actualPosition == nullptr) {
                std::cout << "Actual position is null" << std::endl;
            } else if (actualPosition->parent_ == nullptr) {
                std::cout << "Root hierarchy" << std::endl;
            }
            else 
            {
                std::cout << actualPosition->data_->getStopName() << std::endl;
            }
            std::cout << "\n------------------------------------------------------ " << std::endl;

            int indexMenu;
            std::cout << "\nAUS DRUHA UROVEN \n" << std::endl;
            std::cout << "0 Chod na synov" << std::endl;
            std::cout << "1 Chod na rodica" << std::endl;
            std::cout << "2 predikat" << std::endl;
            std::cout << "3 print hierarchy" << std::endl;
            std::cout << "4 exit" << std::endl;
            std::cout << "\n-------------------------------------- " << std::endl;
            std::cout << "Vyber index: " << std::endl;
            std::cin >> indexMenu;


            if (indexMenu == 0)
            {
                position++;
                //ist na syna
                std::cout << "\nSynovia\n";
                int indexSon;
                for (size_t i = 0; i < actualPosition->sons_->size(); i++)
                {
                    std::cout << i << " " << actualPosition->sons_->access(i)->data_->data_->getStopName() << std::endl;
                }
                std::cout << "\n-------------------------------------- " << std::endl;
                std::cout << "Vyber index: " << std::endl;
                std::cin >> indexSon;
                actualPosition = hierarchy_.accessSon(*actualPosition, indexSon);
            }
            if (indexMenu == 1)
            {
                
                //ist na rodica
                if (!hierarchy_.isRoot(*actualPosition))
                {
                    actualPosition = hierarchy_.accessParent(*actualPosition);
                    position--;
                }
            }
            if (indexMenu == 2)
            {
                //predikat
                int predicateChoice;
                std::string predicateString;
                BusStopManagerAlgoritmus busStopManager;
                do {
                    std::cout << "predikat:" << std::endl;
                    std::cout << "1.Starts" << std::endl;
                    std::cout << "2.Contains" << std::endl;


                    std::cin >> predicateChoice;

                } while (predicateChoice < 1 || predicateChoice > 2);
                std::cout << "Zadaj text predikatu:" << std::endl;

                std::cin.ignore();
                std::getline(std::cin, predicateString);
            
                //std::cin >> predicateString;
                

                ds::amt::ImplicitSequence<HierarchyNode*> filteredSequence;

                

                if (predicateChoice == 1) {
                    busStopManager.filterAndInsert(currentPositionStart_, hierarchy_.end(), filteredSequence,
                        [&, predicateString](HierarchyNode* node) {
                            
                            if (node) {
                                
                                if (!hierarchy_.isRoot(*actualPosition)) {
                                    std::string stopName = node->getStopName();
                                    if (!stopName.empty()) {
                                        return stopName.find(predicateString) == 0;
                                    }
                                }
                            }

                            return false;
                        });
                }
                else if (predicateChoice == 2) {
                    busStopManager.filterAndInsert(currentPositionStart_, hierarchy_.end(), filteredSequence,
                        [&, predicateString](HierarchyNode* node) {
                            
                            if (node) {
                                if (!hierarchy_.isRoot(*actualPosition)) {
                                    std::string stopName = node->getStopName();
                                    if (!stopName.empty()) {
                                        return stopName.find(predicateString) != std::string::npos;
                                    }
                                }
                            }
                            
                            return false;
                        });
                }
                int j = 1;
                std::cout << "\n-------------------------------------- " << std::endl;
                for (auto i = filteredSequence.begin(); i != filteredSequence.end(); ++i) {

                    std::cout << j << ".     " << (*i)->getStopName() << std::endl;

                    j++;

                }
                std::cout << "\n-------------------------------------- " << std::endl;

                SortComparators comparators;

                int menuKomparatov;
                std::cout << "Vyber moznost:" << std::endl;
                std::cout << "1 triedit podla abecedy" << std::endl;
                std::cout << "2 triedit podla spoluhlasok" << std::endl;
                std::cout << "0 exit" << std::endl;


                std::cout << "Zadaj cislo volby: ";
                std::cin >> menuKomparatov;

                j = 1;
                switch (menuKomparatov) {
                case 0:
                    break;
                case 1:
                    std::cout << "\n------------------------- " << std::endl;
                    std::cout << "sort abeceda" << std::endl;
                    std::cout << "\n " << std::endl;
                    
                    comparators.compareAlphabetical(filteredSequence);

                    
                    std::cout << "\n-------------------------------------- " << std::endl;
                    for (auto i = filteredSequence.begin(); i != filteredSequence.end(); ++i) {

                        std::cout << j << ".     " << (*i)->getStopName() << std::endl;

                        j++;

                    }
                    std::cout << "\n-------------------------------------- " << std::endl;
                    
                    
                    break;
                case 2:
                    std::cout << "\n------------------------- " << std::endl;
                    std::cout << "sort spoluhlasky" << std::endl;
                    std::cout << "\n " << std::endl;
                    
                    comparators.compareConsonantCount(filteredSequence);
                    
                    std::cout << "\n-------------------------------------- " << std::endl;
                    for (auto i = filteredSequence.begin(); i != filteredSequence.end(); ++i) {

                        std::cout << j << ".     " << (*i)->getStopName() << std::endl;

                        j++;

                    }
                    
                    
                    std::cout << "\n-------------------------------------- " << std::endl;
                    break;
                }

                std::cout << "\n " << std::endl;
            }
            if (indexMenu == 3)
            {
                printHierarchy();
            }
            if (indexMenu == 4)
            {

                //koniec
                break;
            }

        }
    }

};

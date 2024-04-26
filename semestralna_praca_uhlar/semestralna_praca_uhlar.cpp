// semestralna_praca_uhlar.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <libds/amt/implicit_sequence.h>
#include <Windows.h>
#include "PrvaUroven.h"
#include <libds/amt/explicit_hierarchy.h>
#include "DruhaUroven.h"
#include <libds/heap_monitor.h>

int main()
{
    initHeapMonitor();
    

    SetConsoleOutputCP(1250);
    SetConsoleCP(1250);

    
    
    

    //readerCsv reader;
    ds::amt::ImplicitSequence<Zastavka*> cow = readerCsv::readFile<Zastavka>("data/cow_busstops.csv");
    ds::amt::ImplicitSequence<Zastavka*> kam = readerCsv::readFile<Zastavka>("data/kam_busstops.csv");
    ds::amt::ImplicitSequence<Zastavka*> nan = readerCsv::readFile<Zastavka>("data/nan_busstops.csv");
    ds::amt::ImplicitSequence<Zastavka*> vic = readerCsv::readFile<Zastavka>("data/vic_busstops.csv");
    ds::amt::ImplicitSequence<Zastavka*> vly = readerCsv::readFile<Zastavka>("data/vly_busstops.csv");
    ds::amt::ImplicitSequence<Zastavka*> whi = readerCsv::readFile<Zastavka>("data/whi_busstops.csv");
    ds::amt::ImplicitSequence<Zastavka*> wil = readerCsv::readFile<Zastavka>("data/wil_busstops.csv");
    ds::amt::ImplicitSequence<Zastavka*> wkt = readerCsv::readFile<Zastavka>("data/wkt_busstops.csv");
    


    
    HierarchyHandler hierarchyHandler;
    

    hierarchyHandler.createHierarchy();
    hierarchyHandler.loadHierarchy(cow);
    hierarchyHandler.loadHierarchy(kam);
    hierarchyHandler.loadHierarchy(nan);
    hierarchyHandler.loadHierarchy(vic);
    hierarchyHandler.loadHierarchy(vly);
    hierarchyHandler.loadHierarchy(whi);
    hierarchyHandler.loadHierarchy(wil);
    hierarchyHandler.loadHierarchy(wkt);
    
    
    
    
    int menu;
    while (true)
    {
        std::cout << "AUS SEMESTRALNA PRACA:" << std::endl;
        std::cout << "Uroven 1" << std::endl;
        std::cout << "Uroven 2" << std::endl;
        std::cout << "Uroven 3" << std::endl;
        std::cout << "Exit 0" << std::endl;
        std::cout << "__________________________________" << std::endl;
        std::cout << "Write the number : ";
        std::cin >> menu;
        std::cout << std::endl;

        
        if (menu == 1)
        {
            ds::amt::ImplicitSequence<Zastavka*> filteredStarts;
            ds::amt::ImplicitSequence<Zastavka*> filteredContains;
        
            


            //////////////////////////////////////////////
            
            std::string searchString = "";
            //////////////////////////////////////////////
            
            std::cout << "Zadaj string: ";
            //std::cin >> searchString;

            std::cin.ignore();
            std::getline(std::cin, searchString);


            BusStopManager busAlgoritmus;

            int choice;
            do {

                std::cout << "Vyber dataset:" << std::endl;
                std::cout << "1. cow" << std::endl;
                std::cout << "2. kam" << std::endl;
                std::cout << "3. nan" << std::endl;
                std::cout << "4. vic" << std::endl;
                std::cout << "5. vly" << std::endl;
                std::cout << "6. whi" << std::endl;
                std::cout << "7. wil" << std::endl;
                std::cout << "8. wkt" << std::endl;
                
                std::cout << "Zadaj cislo volby: ";
                std::cin >> choice;


                switch (choice) {
                case 1:

                    /*dataset = cow;*/
                    busAlgoritmus.filterAndInsert(cow.begin(), cow.end(), filteredStarts,
                        [searchString](Zastavka* stop) {
                            return stop->getStopName().find(searchString) == 0;
                        });

                    busAlgoritmus.filterAndInsert(cow.begin(), cow.end(), filteredContains,
                        [searchString](Zastavka* stop) {
                            return stop->getStopName().find(searchString) != std::string::npos;
                        });
                    choice = -1;

                    break;
                case 2:
                    /*dataset = kam;*/
                    busAlgoritmus.filterAndInsert(kam.begin(), kam.end(), filteredStarts,
                        [searchString](Zastavka* stop) {
                            return stop->getStopName().find(searchString) == 0;
                        });

                    busAlgoritmus.filterAndInsert(kam.begin(), kam.end(), filteredContains,
                        [searchString](Zastavka* stop) {
                            return stop->getStopName().find(searchString) != std::string::npos;
                        });
                    choice = -1;
                    break;
                case 3:
                    /*dataset = nan;*/
                    busAlgoritmus.filterAndInsert(nan.begin(), nan.end(), filteredStarts,
                        [searchString](Zastavka* stop) {
                            return stop->getStopName().find(searchString) == 0;
                        });

                    busAlgoritmus.filterAndInsert(nan.begin(), nan.end(), filteredContains,
                        [searchString](Zastavka* stop) {
                            return stop->getStopName().find(searchString) != std::string::npos;
                        });
                    choice = -1;
                    break;
                case 4:
                    /*dataset = vic;*/
                    busAlgoritmus.filterAndInsert(vic.begin(), vic.end(), filteredStarts,
                        [searchString](Zastavka* stop) {
                            return stop->getStopName().find(searchString) == 0;
                        });

                    busAlgoritmus.filterAndInsert(vic.begin(), vic.end(), filteredContains,
                        [searchString](Zastavka* stop) {
                            return stop->getStopName().find(searchString) != std::string::npos;
                        });
                    choice = -1;
                    break;
                case 5:
                    /*dataset = vly;*/
                    busAlgoritmus.filterAndInsert(vly.begin(), vly.end(), filteredStarts,
                        [searchString](Zastavka* stop) {
                            return stop->getStopName().find(searchString) == 0;
                        });

                    busAlgoritmus.filterAndInsert(vly.begin(), vly.end(), filteredContains,
                        [searchString](Zastavka* stop) {
                            return stop->getStopName().find(searchString) != std::string::npos;
                        });
                    choice = -1;
                    break;
                case 6:
                    /*dataset = whi;*/
                    busAlgoritmus.filterAndInsert(whi.begin(), whi.end(), filteredStarts,
                        [searchString](Zastavka* stop) {
                            return stop->getStopName().find(searchString) == 0;
                        });

                    busAlgoritmus.filterAndInsert(whi.begin(), whi.end(), filteredContains,
                        [searchString](Zastavka* stop) {
                            return stop->getStopName().find(searchString) != std::string::npos;
                        });
                    choice = -1;
                    break;
                case 7:
                    /*dataset = wil;*/
                    busAlgoritmus.filterAndInsert(wil.begin(), wil.end(), filteredStarts,
                        [searchString](Zastavka* stop) {
                            return stop->getStopName().find(searchString) == 0;
                        });

                    busAlgoritmus.filterAndInsert(wil.begin(), wil.end(), filteredContains,
                        [searchString](Zastavka* stop) {
                            return stop->getStopName().find(searchString) != std::string::npos;
                        });
                    choice = -1;
                    break;
                case 8:
                    /*dataset = wkt;*/
                    busAlgoritmus.filterAndInsert(wkt.begin(), wkt.end(), filteredStarts,
                        [searchString](Zastavka* stop) {
                            return stop->getStopName().find(searchString) == 0;
                        });

                    busAlgoritmus.filterAndInsert(wkt.begin(), wkt.end(), filteredContains,
                        [searchString](Zastavka* stop) {
                            return stop->getStopName().find(searchString) != std::string::npos;
                        });
                    choice = -1;
                    break;
                default:
                    std::cout << "Zly vyber!" << std::endl;
                    choice = 0;
                    break;
                }
            } while (choice != -1);

            
            
            

            
            std::cout << "\n-------------------------" << std::endl;
            std::cout << "filter start: " << filteredStarts.size() << std::endl;
            busAlgoritmus.printSequence(filteredStarts);
            std::cout << "\n------------------------- " << std::endl;
            std::cout << "filter obsahuje: " << filteredContains.size() << std::endl;
            busAlgoritmus.printSequence(filteredContains);
            std::cout << "\n " << std::endl;
            
            
        }
        else if (menu == 2)
        {



            std::cout << "\nmohutny " << hierarchyHandler.getHierarchy().size() << std::endl;
            
            hierarchyHandler.iterateHierarchy();

            
            
        } else if (menu == 0)
        {
            readerCsv reader;    
            
            reader.deleteZastavkaSequence(cow);
            reader.deleteZastavkaSequence(kam);
            reader.deleteZastavkaSequence(nan);
            reader.deleteZastavkaSequence(vic);
            reader.deleteZastavkaSequence(vly);
            reader.deleteZastavkaSequence(whi);
            reader.deleteZastavkaSequence(wil);
            reader.deleteZastavkaSequence(wkt);

            hierarchyHandler.cleanUp();

            
            
            
            break;
        }
    }
    
    return 0;
    

}
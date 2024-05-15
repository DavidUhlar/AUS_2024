// semestralna_praca_uhlar.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <libds/amt/implicit_sequence.h>
#include <Windows.h>
#include "PrvaUroven.h"
#include <libds/amt/explicit_hierarchy.h>
#include "DruhaUroven.h"
#include <libds/heap_monitor.h>
#include "TretiaUroven.h"
#include "StvrtaUroven.h"

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
    TableHandler tableHandlerCow;
    TableHandler tableHandlerKam;
    TableHandler tableHandlerNan;
    TableHandler tableHandlerVic;
    TableHandler tableHandlerVly;
    TableHandler tableHandlerWhi;
    TableHandler tableHandlerWil;
    TableHandler tableHandlerWkt;

    hierarchyHandler.createHierarchy();
    hierarchyHandler.loadHierarchy(cow);
    hierarchyHandler.loadHierarchy(kam);
    hierarchyHandler.loadHierarchy(nan);
    hierarchyHandler.loadHierarchy(vic);
    hierarchyHandler.loadHierarchy(vly);
    hierarchyHandler.loadHierarchy(whi);
    hierarchyHandler.loadHierarchy(wil);
    hierarchyHandler.loadHierarchy(wkt);
    
    ds::adt::HashTable<std::string, ds::amt::ImplicitSequence<Zastavka*>*> tableCow = tableHandlerCow.loadTable(cow.begin(), cow.end());
    ds::adt::HashTable<std::string, ds::amt::ImplicitSequence<Zastavka*>*> tableKam = tableHandlerKam.loadTable(kam.begin(), kam.end());
    ds::adt::HashTable<std::string, ds::amt::ImplicitSequence<Zastavka*>*> tableNan = tableHandlerNan.loadTable(nan.begin(), nan.end());
    ds::adt::HashTable<std::string, ds::amt::ImplicitSequence<Zastavka*>*> tableVic = tableHandlerVic.loadTable(vic.begin(), vic.end());
    ds::adt::HashTable<std::string, ds::amt::ImplicitSequence<Zastavka*>*> tableVly = tableHandlerVly.loadTable(vly.begin(), vly.end());
    ds::adt::HashTable<std::string, ds::amt::ImplicitSequence<Zastavka*>*> tableWhi = tableHandlerWhi.loadTable(whi.begin(), whi.end());
    ds::adt::HashTable<std::string, ds::amt::ImplicitSequence<Zastavka*>*> tableWil = tableHandlerWil.loadTable(wil.begin(), wil.end());
    ds::adt::HashTable<std::string, ds::amt::ImplicitSequence<Zastavka*>*> tableWkt = tableHandlerWkt.loadTable(wkt.begin(), wkt.end());


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


            BusStopManagerAlgoritmus busAlgoritmus;
            BusStopManager busAlgoritmusHelper;

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
                        [&busAlgoritmusHelper, searchString](Zastavka* stop) {

                            return busAlgoritmusHelper.startsWithString(stop, searchString);
                        });

                    busAlgoritmus.filterAndInsert(cow.begin(), cow.end(), filteredContains,
                        [&busAlgoritmusHelper, searchString](Zastavka* stop) {

                            return busAlgoritmusHelper.containsString(stop, searchString);
                        });
                    choice = -1;

                    break;
                case 2:
                    /*dataset = kam;*/
                    busAlgoritmus.filterAndInsert(kam.begin(), kam.end(), filteredStarts,
                        [&busAlgoritmusHelper, searchString](Zastavka* stop) {

                            return busAlgoritmusHelper.startsWithString(stop, searchString);
                        });

                    busAlgoritmus.filterAndInsert(kam.begin(), kam.end(), filteredContains,
                        [&busAlgoritmusHelper, searchString](Zastavka* stop) {

                            return busAlgoritmusHelper.containsString(stop, searchString);
                        });
                    choice = -1;
                    break;
                case 3:
                    /*dataset = nan;*/
                    busAlgoritmus.filterAndInsert(nan.begin(), nan.end(), filteredStarts,
                        [&busAlgoritmusHelper, searchString](Zastavka* stop) {

                            return busAlgoritmusHelper.startsWithString(stop, searchString);
                        });

                    busAlgoritmus.filterAndInsert(nan.begin(), nan.end(), filteredContains,
                        [&busAlgoritmusHelper, searchString](Zastavka* stop) {

                            return busAlgoritmusHelper.containsString(stop, searchString);
                        });
                    choice = -1;
                    break;
                case 4:
                    /*dataset = vic;*/
                    busAlgoritmus.filterAndInsert(vic.begin(), vic.end(), filteredStarts,
                        [&busAlgoritmusHelper, searchString](Zastavka* stop) {

                            return busAlgoritmusHelper.startsWithString(stop, searchString);
                        });

                    busAlgoritmus.filterAndInsert(vic.begin(), vic.end(), filteredContains,
                        [&busAlgoritmusHelper, searchString](Zastavka* stop) {

                            return busAlgoritmusHelper.containsString(stop, searchString);
                        });
                    choice = -1;
                    break;
                case 5:
                    /*dataset = vly;*/
                    busAlgoritmus.filterAndInsert(vly.begin(), vly.end(), filteredStarts,
                        [&busAlgoritmusHelper, searchString](Zastavka* stop) {

                            return busAlgoritmusHelper.startsWithString(stop, searchString);
                        });

                    busAlgoritmus.filterAndInsert(vly.begin(), vly.end(), filteredContains,
                        [&busAlgoritmusHelper, searchString](Zastavka* stop) {

                            return busAlgoritmusHelper.containsString(stop, searchString);
                        });
                    choice = -1;
                    break;
                case 6:
                    /*dataset = whi;*/
                    busAlgoritmus.filterAndInsert(whi.begin(), whi.end(), filteredStarts,
                        [&busAlgoritmusHelper, searchString](Zastavka* stop) {

                            return busAlgoritmusHelper.startsWithString(stop, searchString);
                        });

                    busAlgoritmus.filterAndInsert(whi.begin(), whi.end(), filteredContains,
                        [&busAlgoritmusHelper, searchString](Zastavka* stop) {

                            return busAlgoritmusHelper.containsString(stop, searchString);
                        });
                    choice = -1;
                    break;
                case 7:
                    /*dataset = wil;*/
                    busAlgoritmus.filterAndInsert(wil.begin(), wil.end(), filteredStarts,
                        [&busAlgoritmusHelper, searchString](Zastavka* stop) {

                            return busAlgoritmusHelper.startsWithString(stop, searchString);
                        });

                    busAlgoritmus.filterAndInsert(wil.begin(), wil.end(), filteredContains,
                        [&busAlgoritmusHelper, searchString](Zastavka* stop) {

                            return busAlgoritmusHelper.containsString(stop, searchString);
                        });
                    choice = -1;
                    break;
                case 8:
                    /*dataset = wkt;*/
                    busAlgoritmus.filterAndInsert(wkt.begin(), wkt.end(), filteredStarts,
                        [&busAlgoritmusHelper, searchString](Zastavka* stop) {

                            return busAlgoritmusHelper.startsWithString(stop, searchString);
                        });

                    busAlgoritmus.filterAndInsert(wkt.begin(), wkt.end(), filteredContains,
                        [&busAlgoritmusHelper, searchString](Zastavka* stop) {

                            return busAlgoritmusHelper.containsString(stop, searchString);
                        });
                    choice = -1;
                    break;
                default:
                    std::cout << "Zly vyber!" << std::endl;
                    choice = 0;
                    break;
                }
            } while (choice != -1);



            int menuPredikatov;
            std::cout << "" << std::endl;
            std::cout << "1 StartsWith" << std::endl;
            std::cout << "2 Contains" << std::endl;
            //std::cout << "0 exit" << std::endl;


            std::cout << "Zadaj cislo volby: ";
            std::cin >> menuPredikatov;

            switch (menuPredikatov) {
            case 1:

                std::cout << "\n-------------------------" << std::endl;
                std::cout << "filter start: " << filteredStarts.size() << std::endl;
                busAlgoritmusHelper.printSequence(filteredStarts);
                std::cout << "\n " << std::endl;
                break;
            case 2:
                std::cout << "\n------------------------- " << std::endl;
                std::cout << "filter obsahuje: " << filteredContains.size() << std::endl;
                busAlgoritmusHelper.printSequence(filteredContains);
                std::cout << "\n " << std::endl;
                
                break;
            default:
                std::cout << "\n-------------------------" << std::endl;
                std::cout << "filter start: " << filteredStarts.size() << std::endl;
                busAlgoritmusHelper.printSequence(filteredStarts);
                std::cout << "\n------------------------- " << std::endl;
                std::cout << "filter obsahuje: " << filteredContains.size() << std::endl;
                busAlgoritmusHelper.printSequence(filteredContains);
                std::cout << "\n " << std::endl;
                break;
            }


            


            SortComparators comparators;
            
            std::cout << "\n " << std::endl;
            

            int menuKomparatov;
            std::cout << "Vyber moznost:" << std::endl;
            std::cout << "1 triedit podla abecedy" << std::endl;
            std::cout << "2 triedit podla spoluhlasok" << std::endl;
            std::cout << "0 exit" << std::endl;


            std::cout << "Zadaj cislo volby: ";
            std::cin >> menuKomparatov;


            switch (menuKomparatov) {
            case 0:
                break;
            case 1:
                std::cout << "\n------------------------- " << std::endl;
                std::cout << "sort abeceda" << std::endl;
                std::cout << "\n " << std::endl;
                if (menuPredikatov == 1)
                {
                    comparators.compareAlphabetical(filteredStarts);
                    busAlgoritmusHelper.printSequence(filteredStarts);
                }
                else 
                {
                    comparators.compareAlphabetical(filteredContains);
                    busAlgoritmusHelper.printSequence(filteredContains);
                }
                break;
            case 2:
                std::cout << "\n------------------------- " << std::endl;
                std::cout << "sort spoluhlasky" << std::endl;
                std::cout << "\n " << std::endl;
                if (menuPredikatov == 2)
                {
                    comparators.compareConsonantCount(filteredContains);
                    busAlgoritmusHelper.printSequence(filteredContains);
                }
                else
                {
                    comparators.compareConsonantCount(filteredStarts);
                    busAlgoritmusHelper.printSequence(filteredStarts);
                }
                break;
            }
            
            std::cout << "\n " << std::endl;
            
        }
        else if (menu == 2)
        {



            std::cout << "\nmohutnost hierarchie: " << hierarchyHandler.getHierarchy().size() << std::endl;

            hierarchyHandler.iterateHierarchy();



        }

        else if (menu == 3)
        { 
            


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

                //////////////////////////////////////////////

                std::string searchString = "";
                //////////////////////////////////////////////

                std::cout << "Zadaj string: ";
                //std::cin >> searchString;

                std::cin.ignore();
                std::getline(std::cin, searchString);
                std::cout << "\n" << std::endl; 
                std::cout << "\n";
                
                

                switch (choice) {
                case 1:

                    /*dataset = cow;*/
                    
                    tableHandlerCow.filterTable(tableCow, searchString);
                    
                    choice = -1;
                    break;
                case 2:
                    /*dataset = kam;*/
                    
                    tableHandlerKam.filterTable(tableKam, searchString);

                    choice = -1;
                    break;
                case 3:
                    /*dataset = nan;*/
                    tableHandlerNan.filterTable(tableNan, searchString);

                    choice = -1;
                    break;
                case 4:
                    /*dataset = vic;*/
                    
                    tableHandlerVic.filterTable(tableVic, searchString);
                    choice = -1;
                    break;
                case 5:
                    /*dataset = vly;*/
                    
                    tableHandlerVly.filterTable(tableVly, searchString);
                    choice = -1;
                    break;
                case 6:
                    /*dataset = whi;*/
                    
                    tableHandlerWhi.filterTable(tableWhi, searchString);
                    choice = -1;
                    break;
                case 7:
                    /*dataset = wil;*/
                    
                    tableHandlerWil.filterTable(tableWil, searchString);
                    choice = -1;
                    break;
                case 8:
                    /*dataset = wkt;*/
                    
                    tableHandlerWkt.filterTable(tableWkt, searchString);
                    choice = -1;
                    break;
                default:
                    std::cout << "Zly vyber!" << std::endl;
                    choice = 0;
                    break;
                }
            } while (choice != -1);
        }
        else if (menu == 0)
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

            tableHandlerCow.cleanUp();
            tableHandlerKam.cleanUp();
            tableHandlerNan.cleanUp();
            tableHandlerVic.cleanUp();
            tableHandlerVly.cleanUp();
            tableHandlerWhi.cleanUp();
            tableHandlerWil.cleanUp();
            tableHandlerWkt.cleanUp();
            
            break;
        }
    }
    
    return 0;
    
    
}
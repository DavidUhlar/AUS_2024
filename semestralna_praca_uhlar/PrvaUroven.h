#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <libds/amt/implicit_sequence.h>
#include <libds/heap_monitor.h>



class Zastavka
{
public:


    std::string getStopId() const { return stopid_; }
    std::string getStopName() const { return stopname_; }
    std::string getStopSite() const { return stopsite_; }
    std::string getLatitude() const { return latitude_; }
    std::string getLongitude() const { return longitude_; }
    std::string getSysCode() const { return syscode_; }
    std::string getSystem() const { return system_; }
    std::string getMuni() const { return muni_; }


    void setStopId(const std::string& id) { stopid_ = id; }
    void setStopName(const std::string& name) { stopname_ = name; }
    void setStopSite(const std::string& site) { stopsite_ = site; }
    void setLatitude(const std::string& lat) { latitude_ = lat; }
    void setLongitude(const std::string& lon) { longitude_ = lon; }
    void setSysCode(const std::string& code) { syscode_ = code; }
    void setSystem(const std::string& sys) { system_ = sys; }
    void setMuni(const std::string& m) { muni_ = m; }


    Zastavka(std::string stopid, std::string stopname, std::string stopsite, std::string latitude, std::string longitude, std::string syscode, std::string system, std::string muni)
    {
        stopid_ = stopid;
        stopname_ = stopname;
        stopsite_ = stopsite;
        latitude_ = latitude;
        longitude_ = longitude;
        syscode_ = syscode;
        system_ = system;
        muni_ = muni;
    }
    
    
private:
    std::string stopid_;
    std::string stopname_;
    std::string stopsite_;
    std::string latitude_;
    std::string longitude_;
    std::string syscode_;
    std::string system_;
    std::string muni_;

};



class readerCsv
{
public:
    
    template<typename TypTemplate>
    static ds::amt::ImplicitSequence<TypTemplate*> readFile(std::string fileName)
    {

        std::ifstream file(fileName);
        ds::amt::ImplicitSequence<TypTemplate*> structure;
        std::string riadok;
        int count = 0;
        while (std::getline(file, riadok))
        {
            if (count == 0)
            {
                count++;
                continue;
            }
            else
            {
                std::istringstream iss(riadok);
                //Zastavka zastavka;
                std::string stopid, stopname, stopsite, latitude, longitude, syscode, system, muni;

                std::getline(iss, stopid, ';');
                std::getline(iss, stopname, ';');
                std::getline(iss, stopsite, ';');
                std::getline(iss, latitude, ';');
                std::getline(iss, longitude, ';');
                std::getline(iss, syscode, ';');
                std::getline(iss, system, ';');
                std::getline(iss, muni, ';');

                structure.insertLast().data_ = new TypTemplate(stopid, stopname, stopsite, latitude, longitude, syscode, system, muni);
            }


        }

        file.close();
        return structure;
    }
    void deleteZastavkaSequence(ds::amt::ImplicitSequence<Zastavka*>& sequence)
    {
        
        for (auto it = sequence.begin(); it != sequence.end(); ++it)
        {
            delete *it;
            
            
        }
        sequence.clear();
        
    }
};





class BusStopManager
{
public:

    /*std::string splitString(std::string& input, int columnIndex, char delimiter) {
        std::stringstream ss(input);
        std::string columnValue;
        for (int i = 0; i <= columnIndex; ++i) {
            if (!std::getline(ss, columnValue, delimiter)) {
                std::cerr << "split failed" << std::endl;
                return "";
            }
        }
        return columnValue;
    }*/
    
    
    
    template<typename InputIterator, typename OutputContainer, typename Predicate>
    void filterAndInsert(InputIterator begin, InputIterator end, OutputContainer& outputSequence, Predicate predicate) {
        
        for (auto i = begin; i != end; ++i) {
            
            if (predicate(*i))
            {
                outputSequence.insertLast().data_ = (*i);
                
            }
        }
    }
    


    
    void printSequence(ds::amt::ImplicitSequence<Zastavka*>& sequence) {
        std::cout << "   ";

        for (auto i = sequence.begin(); i != sequence.end(); ++i) {
            Zastavka* stop = *i;

            std::cout << stop->getStopId() << "  | "
                << stop->getStopName() << "  | "
                << stop->getStopSite() << "  | "
                << stop->getLatitude() << "  | "
                << stop->getLongitude() << "  | "
                << stop->getSysCode() << "  | "
                << stop->getSystem() << "  | "
                << stop->getMuni() << "  \n   ";
        }
    };

    bool startsWithString(Zastavka* stop, std::string searchString) {
        return stop->getStopName().find(searchString) == 0;
        
    };
    bool containsString(Zastavka* stop, std::string searchString) {
        return stop->getStopName().find(searchString) != std::string::npos;

    };
};
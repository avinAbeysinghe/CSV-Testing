// #ifndef TEMPLATES_H
// #define TEMPLATES_H
#pragma once

#include <iostream>
#include <tuple> //this is similar to arrays but for templates basically 
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
extern std::string fileName;
extern std::ofstream sharedCSV;

class CSVManager{
private:
    int csv_counter;
    std::string tracker = "./tests/csv_tracker.txt";
    std::string prevPrefix; //gets the previous dating prefix of another file

    std::string dateTime() { //this is the time when the code was compiled(NOT IN REAL TIME)
        const char* date = __DATE__;
        const char* time = __TIME__;
        char createDate[20]; //this entire part will be 20 characters
        snprintf(createDate, sizeof(createDate), "%c%c_%c%c%c_%c%c_%c%c", //how the string will be formatted
            date[4]== ' ' ? '0': date[4], date[5], //day
            date[0], date[1], date[2], //month
            time[0], time[1], //hour
            time[3], time[4] //minutes
        );
        return std::string(createDate);
    };
    
public:
    inline CSVManager(){ //another goal is to change this count based on when it was last compiled 
        std::ifstream csv_tracker(tracker); //for the tracker file, the scope will automatically close it 
        if(csv_tracker.is_open()) {
            std::getline(csv_tracker, prevPrefix); //stores this 
            if(prevPrefix != dateTime()){
                csv_counter = 0;
                return;
            }
            csv_tracker >> csv_counter;
        }
        else csv_counter = 0;
    }
    inline std::string newDataLog(){
        csv_counter++;
        fileName = "./tests/" + dateTime() + "_data_v" + std::to_string(csv_counter) + ".csv";
        return fileName;
    }
    inline void saveCSVCountNum(){
        std::ofstream out(tracker);
        out << dateTime() << "\n" << csv_counter;
    }
};

/* Declarations for sensor types. Definitions live in src/templates.cpp */
struct Motor {
    std::string m_Name;
    Motor(std::string n);
    std::vector<std::string> headers(const std::string& name) const;
    std::vector<double> read() const;
};

struct Color {
    std::string m_Name;
    Color(std::string n);
    std::vector<std::string> headers(const std::string& name) const;
    std::vector<double> read() const;
};



template<typename... Sensors>
class SensorLogger{
private:
    std::tuple<Sensors...> sensors; //adds all of the sensors to an arraylike stucture
    template<typename T>
    void createHeaders(const T& sensor) { //write headers for a single sensor (no trailing comma)
        auto headers = sensor.headers(sensor.m_Name);
        for(std::size_t i = 0; i < headers.size(); ++i){ //adds all the headers of each sensor onto the CSV file
            sharedCSV << sensor.m_Name << ":" << headers[i];
            if(i + 1 < headers.size()) sharedCSV << ","; //if it's not the last one, add a comma
        }
    }

    template<typename T>
    void createData(const T& sensor) {
        auto data = sensor.read();
        for(std::size_t i = 0; i < data.size(); ++i){
            sharedCSV << data[i];
            if(i + 1 < data.size()) sharedCSV << ",";
        }
    }

    // helper to write a separator between sensors (no trailing comma after last sensor)
    template<std::size_t I, std::size_t N>
    void writeSensorSeparator() {
        if constexpr (I + 1 < N) sharedCSV << ",";
    }

    template <std::size_t... I> //writes the headers row
    void writeAllHeaders(std::index_sequence<I...>) {
        // For each sensor: write its headers, then write a separator if it's not the last sensor
        sharedCSV << "Time(s)" << ",";
        ((createHeaders(std::get<I>(sensors)), writeSensorSeparator<I, sizeof...(I)>()), ... );
        sharedCSV << "\n";
    }

    template <std::size_t... I> //writes the data rows
    void writeAllData(std::index_sequence<I...>) {
        sharedCSV << "1SCE"  << ",";
        ((createData(std::get<I>(sensors)), writeSensorSeparator<I, sizeof...(I)>()), ... );
        sharedCSV << "\n";
    }
public:
   SensorLogger(Sensors... s): sensors(std::move(s)...){
        if(!sharedCSV.is_open()){ //checks if the file isn't already opened
            CSVManager manager;
            manager.newDataLog();
            manager.saveCSVCountNum();
            sharedCSV.open(fileName);
        }  
        writeAllHeaders(std::index_sequence_for<Sensors...>{});
    }
    void logData(){
        writeAllData(std::index_sequence_for<Sensors...>{});
        sharedCSV.flush();
    }
};

//#endif
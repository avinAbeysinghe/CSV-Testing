/* Have a vector full of different classs of data 
    whenever the the conclassor is called, add the class to that global vector
*/
 

#include <iostream>
#include <tuple>
#include <vector>
#include <string>
#include <fstream>

std::string fileName;
std::ofstream sharedCSV;

class CSVManager{
private:
    int csv_counter;
    std::string tracker = "./tests/csv_tracker.txt";
public:
    CSVManager(){
        std::ifstream csv_tracker(tracker);
        if(csv_tracker.is_open()) csv_tracker >> csv_counter;
        else csv_counter = 0;
        csv_tracker.close();
    }
    std::string newDataLog(){
        csv_counter++;
        fileName = "./tests/data_log_" + std::to_string(csv_counter) + ".csv";
        return fileName;
    }
    void saveCSVCountNum(){
        std::ofstream out(tracker);
        out << csv_counter;
        out.close();
    }
};

template<typename... Sensors>
class SensorLogger{
private:
    std::tuple<Sensors...> sensors; //adds all of the sensors to an arraylike stucture
    template<typename T>
    void createHeaders(const T& sensor) { //write headers for a single sensor (no trailing comma)
        auto headers = sensor.headers();
        for(std::size_t i = 0; i < headers.size(); ++i){ //adds all the headers of each sensor onto the CSV file
            sharedCSV << headers[i];
            if(i + 1 < headers.size()) sharedCSV << ","; //if it's not the last one, add a comma
        }
    }

    template<typename T>
    void enterData(const T& sensor) {
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
        ((createHeaders(std::get<I>(sensors)), writeSensorSeparator<I, sizeof...(I)>()), ... );
        sharedCSV << "\n";
    }

    template <std::size_t... I> //writes the data rows
    void writeAllData(std::index_sequence<I...>) {
        ((enterData(std::get<I>(sensors)), writeSensorSeparator<I, sizeof...(I)>()), ... );
        sharedCSV << "\n";
    }
public:
   SensorLogger(Sensors... s): sensors(s...){
        if(!sharedCSV.is_open()){
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
/*Simp[lifying creation*/
struct Motor {
    std::string motorName;
    Motor(std::string motorName): motorName(motorName) {}
    std::vector<std::string> headers () const {
        return {motorName + ":VELOC", motorName +":POW"};
    }
    std::vector<int> read() const {
        return {67, 41};
    }
};
struct Color {
    std::string motorName;
    Color(std::string motorName): motorName(motorName) {}
    std::vector<std::string> headers () const {
        return {motorName + ":HUE", motorName +":BRIGHT"};
    }
    std::vector<int> read() const {
        return {1, 5};
    }
};

int main(){
    Motor m1("motor");
    Color c1("color");
    SensorLogger<Motor, Color> logger(m1, c1);
    for(int i = 0; i < 3; ++i){
        //sharedCSV << 6741 << ","; //return the time
        logger.logData();
    }
    return 0;
}
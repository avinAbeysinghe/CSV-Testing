/* Have a vector full of different classs of data 
    whenever the the conclassor is called, add the class to that global vector
*/
 

#include <iostream>
#include <vector>
#include <fstream>

std::string fileName;
class Sensor;
std::vector<Sensor*> SensorList;
std::vector<std::string> headerList;
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


class Sensor{
public:
   Sensor(){
        if(!sharedCSV.is_open()){
            CSVManager manager;
            manager.newDataLog();
            manager.saveCSVCountNum();
            sharedCSV.open(fileName);
        }  
        SensorList.emplace_back(this);
        
    }
    static void confirmList(){
        //auto lastHeader = std::prev(headerList.end());
        headerList.insert(headerList.begin(), "Time(sec)");
        for(int i = 0; i < headerList.size(); ++i){
            if(i == headerList.size() - 1){ // if this header equals the last one, no comma at the end
                sharedCSV << headerList[i] << '\n'; 
                break;
            }
            sharedCSV << headerList[i] << ",";
        }
    }
    static void newRow(){sharedCSV << "\n";}
    virtual void recordData(){}; //intentionally does nothing as a parent 
    virtual ~Sensor(){ //ideally this shuts down at the end of the program anyways
        //deletes the Sensor at at the end of the program
        if(sharedCSV.is_open()) sharedCSV.close(); 
    }

};


class Motor : public Sensor {
/*velocity, Voltage, Power*/
private:
    
public:
    /*Not used now, but in the actual code, will need to refrence which motor is being used*/
    Motor(std::string name): Sensor(){ 
        //different Headers
        headerList.emplace_back(name + ": power");
        headerList.emplace_back(name + ": voltage");
        headerList.emplace_back(name + ": veocity");

    }
    //Remember to delete Motor when done using it. 
    void recordData() override {
        sharedCSV << 1 << ",";
        sharedCSV << 2 << ",";
        sharedCSV << 3 << ",";
        //when it comes for checking multiple stuff, add a way so this ends in '\n' instead
    }
    ~Motor() override = default;
};

class ColorSen : public Sensor {
public:
    ColorSen(std::string name): Sensor(){
        headerList.emplace_back(name + ": hue");
        headerList.emplace_back(name + ": brightness");
        headerList.emplace_back(name + ": LED aura"); //check on why it skipped this one
    }
    void recordData() override {
        sharedCSV << 4 << ",";
        sharedCSV << 5 << ",";
        sharedCSV << 6; //non permanent solution, figure out how to do this automatically 
    }
    ~ColorSen() override = default;
};


int main(){
    Motor* exampleMotor = new Motor("Motor 1");
    ColorSen* exampleSen = new ColorSen("Color");
    Sensor::confirmList();
    for(int i = 0; i < 3; ++i){
        sharedCSV << 6741 << ","; //return the time
        for(Sensor* sensor:SensorList){
            sensor->recordData();
        }
        Sensor::newRow();
    }
    return 0;
}
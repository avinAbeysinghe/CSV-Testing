/* Have a vector full of different classs of data 
    whenever the the conclassor is called, add the class to that global vector
*/
 
#include "../include/templates.h"

/*Simplifying creation*/
// Motor and Color are declared in include/templates.h and defined in src/templates.cpp
SensorLogger<Motor, Motor, Color, Color>* logger = nullptr;
int main(){
    Motor m1("Motor 1");
    Motor m2("Motor 2");
    Color c1("Color 1"); 
    Color c2("Color 2");
    //Color c1("color");
    logger = new SensorLogger(m1, m2, c1, c2); //We need to keep track of the type and the object
    for(int i = 0; i < 500; ++i){
        logger->logData();

    }
    return 0;
}
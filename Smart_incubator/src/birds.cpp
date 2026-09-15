#include <Arduino.h>

typedef struct birds{
    char name[10];
    uint8_t temperature;
    uint8_t humidityNormal;
    uint8_t days;
    uint8_t humidityLockdown;
    uint8_t lockdownDays;
}Birds;

Birds chicken = {"Chicken", 37, 50, 21, 65, 18};
Birds duck = {"Duck", 37, 55, 28, 75, 25};
Birds turkey = {"Turkey", 37, 50, 28, 65, 25};
Birds guineaFowl = {"fowl", 37, 50, 27,50,24};
Birds birdsArray[4] = {chicken, duck, turkey, guineaFowl};

uint8_t birdsArrayIndex = 0;

Birds selectBirds(Birds *birdsArray, uint8_t index, uint8_t size){
    if(index < size){
     Birds selectedBirds = birdsArray[index];
     return selectedBirds;
    }
    else{
        Birds customBirds = {"Custom", 0, 0, 0, 0, 0}; 
        return customBirds;
    }
}


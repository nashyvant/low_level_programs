/**
 * @file main.c
 * @brief Main function for testing the NVM API
 */
#include <string.h>
#include <stdlib.h>
#include "nvm_api.h"

#define MAX_ANTENNA_ELEMENTS 5


int main()
{
    /*
     * Unit test 1a: set float value
     */
    float f = 65.8f;
    printf("Setting float value in memory: %f\n", f);
    
    uint8_t *v = (uint8_t*) &f;
    // for(int i=0; i<4; ++i)
        //printf("0x%x ", v[i]);
    
    gpNvm_SetAttribute(45, sizeof(f), v);
    
     /*
      * Unit test 1b: get float value
      */
    uint8_t len = 0;
    uint8_t val_read[1000];
    gpNvm_GetAttribute(45, &len, val_read);

    float new_f = *((float*)val_read);
    printf("Reading float value from memory: %f\n", new_f);
    
    /*
     * Unit test 2a: set int value
     */
    int a = 349587;
    v = (uint8_t*) &a;
    //for(int i=0; i<4; ++i)
       // printf("0x%x", v[i]);
    printf("Setting int value in memory: %d\n", a);
    gpNvm_SetAttribute(46, sizeof(a), v);

    /*
     * Unit test 2b: get int value
     */
    gpNvm_GetAttribute(46, &len, val_read);
    int new_a = *((int*)val_read);
    printf("Reading int value from memory: %d\n", new_a);

    /*
     * Unit test 3a: set double value
     */
    double d = 784564093.908446545;
    v = (uint8_t*) &d;
    //for(int i=0; i<sizeof(d); ++i)
       // printf("0x%x ", v[i]);
    printf("Setting double value in memory: %lf\n", d);
    gpNvm_SetAttribute(47, sizeof(d), v);

    /*
     * Unit test 3b: get double value
     */
    gpNvm_GetAttribute(47, &len, val_read);

    double new_d = *((double*)val_read);
    printf("Reading double value from memory: %lf\n", new_d);

    /*
     * Unit test 4a: set struct value
     */
    typedef struct 
    {
        uint8_t active;
        int64_t cal_data_phase[MAX_ANTENNA_ELEMENTS];
        double tx_pwr_dBm;
        uint16_t gains_dB[MAX_ANTENNA_ELEMENTS];
    } Antenna;
     
    Antenna left;
    left.active = 1;
    left.tx_pwr_dBm = -124.552052;
    for(int ele=0; ele< MAX_ANTENNA_ELEMENTS; ++ele )
    {
        left.cal_data_phase[ele] = (ele+1)*6;
        left.gains_dB[ele] = (ele+1)*10;
    }

    v = (uint8_t*) &left;
    //for(int32_t i=0; i<sizeof(Antenna); ++i)
       // printf("0x%x ", v[i]);
   
    gpNvm_SetAttribute(48, sizeof(Antenna), v);

    /*
     * Unit test 4b: get struct value
     */
    gpNvm_GetAttribute(48, &len, val_read);

    Antenna new_left_antenna = *((Antenna*)val_read);
    printf("Reading Antenna values from memory:\n");
    printf("Antenna active: %hhu\n", new_left_antenna.active);
    printf("Antenna TX power: %lf dBm\n", new_left_antenna.tx_pwr_dBm);
    for(int8_t ele = 0; ele < MAX_ANTENNA_ELEMENTS; ++ele)
        printf("Cal data phase[%d]: %ld\n", ele, new_left_antenna.cal_data_phase[ele]);

    for(int8_t ele = 0; ele < MAX_ANTENNA_ELEMENTS; ++ele)
        printf("Gain [%d]: %d dB\n", ele, new_left_antenna.gains_dB[ele]);


    /*
     * Unit test 5: set and get values of same attribute ID
     * If same attribute ID is entered, the value gets overwritten
     */
    a = -5628;
    v = (uint8_t*) &a;
    //for(int i=0; i<4; ++i)
       // printf("0x%x", v[i]);
    printf("Setting int value in memory: %d\n", a);
    gpNvm_SetAttribute(46, sizeof(a), v);

    a = -25;
    v = (uint8_t*) &a;
    //for(int i=0; i<4; ++i)
       // printf("0x%x", v[i]);
    printf("Setting int value in memory: %d\n", a);
    gpNvm_SetAttribute(46, sizeof(a), v);

    gpNvm_GetAttribute(46, &len, val_read);

    new_a = *((int*)val_read);
    printf("Reading int value from memory: %d\n", new_a); //this will display -25 because -5628 gets overwritten in the memory

    printf("finding attribute 50 with return code: %hhu", gpNvm_GetAttribute(50, &len, val_read));
}
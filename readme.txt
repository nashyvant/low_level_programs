SDK Documentation for Non-Volatile Memory (NVM) API

Overview:
The NVM API provides functions to interact with non-volatile memory, allowing users to set and get attribute values stored in the memory.

API Functions:

typedef uint8_t gpNvm_AttrId;
typedef uint8_t gpNvm_Result;

gpNvm_Result gpNvm_SetAttribute(gpNvm_AttrId attrId, uint8_t length, uint8_t* pValue);
gpNvm_Result gpNvm_GetAttribute(gpNvm_AttrId attrId, uint8_t* pLength, uint8_t* pValue);

1. gpNvm_SetAttribute:
   - Description: Sets the value of an attribute in the NVM.
   - Parameters:
     - attrId: Attribute identifier for the attribute to be stored.
     - length: Size of the value to be stored.
     - pValue: Pointer to the value to be stored.
   - Return Value: Result code indicating the success or failure of the operation.
   0XFF if operation failed.
   0X0 if success.

2. gpNvm_GetAttribute:
   - Description: Retrieves the value of an attribute from the NVM.
   - Parameters:
     - attrId: Attribute identifier for the attribute to be retrieved.
     - pLength: Pointer to store the size of the retrieved value.
     - pValue: Pointer to store the retrieved value.
   - Return Value: Result code indicating the success or failure of the operation.
   0XFF or 255 if operation failed.
   0 if success.

Usage Examples:
1. Setting a Float Value:
   
   float f = 65.8f;
   uint8_t *v = (uint8_t*) &f;
   gpNvm_SetAttribute(45, sizeof(f), v);
  

2. Getting a Float Value:
 
   uint8_t len = 0;
   uint8_t val_read[1000];
   gpNvm_GetAttribute(45, &len, val_read);
   float new_f = *((float*)val_read);

Important Note:
- Ensure appropriate endian conversions if there is a difference in architectures between the NVM and the hardware.

Building and Running:
1. Compile the SDK source files:
   gcc nvm_api.c main.c -o main.exe
   
2. Run the executable:
   ./main.exe

   The `main()` function will execute various test cases to demonstrate setting and getting attribute values for different data types.

Please refer to the source code comments and documentation for detailed information on each function and its usage.

The main() function will run through a few tests whose results will be displayed. We set and get attributes for various types of data structures such as:
1. int
2. float
3. word
4. double
5. user defined data types such as structs

Setting float value in memory: 65.800003
gpNvm_SetAttribute Complete!
getFromFile found! 45 4
gpNvm_GetAttribute Complete!
Reading float value from memory: 65.800003

Setting int value in memory: 349587
gpNvm_SetAttribute Complete!
getFromFile found! 46 4
gpNvm_GetAttribute Complete!
Reading int value from memory: 349587

Setting double value in memory: 784564093.908447
gpNvm_SetAttribute Complete!
getFromFile found! 47 8
gpNvm_GetAttribute Complete!
Reading double value from memory: 784564093.908447

gpNvm_SetAttribute Complete!
getFromFile found! 48 72
gpNvm_GetAttribute Complete!
Reading Antenna values from memory:
Antenna active: 1
Antenna TX power: -124.552052 dBm
Cal data phase[0]: 6
Cal data phase[1]: 12
Cal data phase[2]: 18
Cal data phase[3]: 24
Cal data phase[4]: 30
Gain [0]: 10 dB
Gain [1]: 20 dB
Gain [2]: 30 dB
Gain [3]: 40 dB
Gain [4]: 50 dB

Setting int value in memory: -5628
gpNvm_SetAttribute Complete!
Setting int value in memory: -25
gpNvm_SetAttribute Complete!
getFromFile found! 46 4
gpNvm_GetAttribute Complete!
Reading int value from memory: -25

gpNvm_GetAttribute Complete!
finding attribute 50 with return code: 255
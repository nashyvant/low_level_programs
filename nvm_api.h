#include <stdint.h>
#include <stdio.h>

typedef uint8_t gpNvm_AttrId;
typedef uint8_t gpNvm_Result;

gpNvm_Result gpNvm_GetAttribute(gpNvm_AttrId attrId, uint8_t* pLength, uint8_t* pValue);
gpNvm_Result gpNvm_SetAttribute(gpNvm_AttrId attrId, uint8_t length, uint8_t* pValue);
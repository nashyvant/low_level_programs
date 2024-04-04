#include <string.h>
#include <stdlib.h>
#include "nvm_api.h"

/**
 * @brief Gets value of an attribute from the Non-Volatile Memory (NVM).
 *
 * @param attrId The ID of the attribute to get.
 * @param length The length of the data pointed to by pValue.
 * @param pValue Pointer to the data to be stored in NVM.
 *
 */
gpNvm_Result gpNvm_GetAttribute(gpNvm_AttrId attrId, uint8_t* pLength, uint8_t* pValue)
{
    gpNvm_Result found = 0xFF;
    FILE* input_file = fopen("nv_data.bin", "rb");
	if (input_file == NULL)
	{
		perror("Error opening file");
		return 0xFF;
	}

    uint8_t curr_attrId, curr_sz;
    while(fscanf(input_file, "%hhu", &curr_attrId) == 1)
    {
        if(fscanf(input_file, "%hhu", &curr_sz) == 1)
        {
            if(curr_attrId != attrId)
            {
                uint8_t *pVal = (uint8_t*)malloc(curr_sz);
                uint8_t *old_p = pVal;
                for(int i=0; i< curr_sz; ++i)
                {
                    fscanf(input_file, "%hhu", pVal); //You could also use FSEEK here
                    pVal++;
                }

                free(old_p);
            }
            else
            {
                *pLength = curr_sz;
                uint8_t *old_p = pValue;
                for(int i=0; i< curr_sz; ++i)
                {
                    fscanf(input_file, "%hhu", pValue);
                    pValue++;
                }

                pValue = old_p;
                printf("getFromFile found! %d %d\n", attrId, *pLength);
                //for(int i=0; i< curr_sz; ++i)
                    //printf("0x%x ", pValue[i]);
                found = 0;
                break;
            }
        }
    }

	fclose(input_file);
    printf("gpNvm_GetAttribute Complete!\n");
    return found;
}


/**
 * @brief Sets an attribute in the Non-Volatile Memory (NVM).
 *
 * @param attrId The ID of the attribute to set.
 * @param length The length of the data pointed to by pValue.
 * @param pValue Pointer to the data to be stored in NVM.
 *
 */
gpNvm_Result gpNvm_SetAttribute(gpNvm_AttrId attrId, uint8_t length, uint8_t* pValue)
{
    FILE* input_file = fopen("nv_data.bin", "rb"); 
    uint8_t bFirstWrite = 0;
	if (input_file == NULL)
	{
        bFirstWrite = 1; //to create a new file if a file does not already exist
        input_file = fopen("nv_data.bin", "wb");
        if(input_file == NULL)
        {
		    perror("Error opening NV file");
		    return 0xFF;
        }
	}

	FILE* output_file = fopen("temp.bin", "ab");
	if (output_file == NULL)
	{
		perror("Error creating temporary file");
		fclose(input_file);
		return 0xFF;
	}

    if(bFirstWrite == 0)
    {
        uint8_t curr_attrId, curr_sz;
        while(fscanf(input_file, "%hhu", &curr_attrId) == 1)
        {
            if(fscanf(input_file, "%hhu", &curr_sz) == 1)
            {
                uint8_t *pVal = (uint8_t*)malloc(curr_sz);
                uint8_t *old_p = pVal;
                for(int i=0; i< curr_sz; ++i)
                {
                    fscanf(input_file, "%hhu", pVal);
                    pVal++;
                }

                if(curr_attrId != attrId) //copy all attributes except the new one which needs to replaced
                {
                    fprintf(output_file, "%hhu\n%hhu\n", curr_attrId, curr_sz);
                    for(int i=0; i< curr_sz; ++i)
                        fprintf(output_file, "%hhu\n", old_p[i]);
                }

                free(old_p);
            }
        }
    }

    //for(int i=0; i < length; ++i)
        //printf("0x%x ", pValue[i]);
        
	//now add the given set attribute
	fprintf(output_file, "%hhu\n%hhu\n", attrId, length);
    for(int i=0; i<length; ++i)
        fprintf(output_file, "%hhu\n", pValue[i]);

    fclose(input_file);
	fclose(output_file);
	remove("nv_data.bin");
	// Rename temporary file to original file
	if (rename("temp.bin", "nv_data.bin") != 0)
	{
		perror("Error renaming temporary file");
        return 0XFF;
	}

    printf("gpNvm_SetAttribute Complete!\n");

    return 0;
}
#include <string.h>
#include "nvm_api.h"

/**
 * @brief Sets an attribute in the Non-Volatile Memory (NVM).
 *
 * @param attrId The ID of the attribute to set.
 * @param length The length of the data pointed to by pValue.
 * @param pValue Pointer to the data to be stored in NVM.
 *
 */


gpNvm_Result gpNvm_GetAttribute(gpNvm_AttrId attrId, uint8_t* pLength, uint8_t* pValue)
{
	FILE* input_file = fopen("nvm_file.txt", "r");
	if (input_file == NULL)
	{
		perror("Error opening file");
		return 0xff;
	}
	uint8_t attr = 0; // Adjust the buffer size as needed

	// Read NV file line by line
	while (fgets((char*)attr, 1, input_file) != NULL)
	{
		uint8_t size = 0;
		if (fgets((char*) size, 1, input_file) != NULL)
		{
			uint8_t data[1000];
			while (fgets((char*)data, size, input_file) != NULL)
			{
				if (attr == attrId)
				{
					attrId = attr;
					*pLength = size;
					memcpy(pValue, data, size);
					return 0;
				}
			}
		}
	}
	return 0xff; //could not find the attrId
}

gpNvm_Result gpNvm_SetAttribute(gpNvm_AttrId attrId, uint8_t length, uint8_t* pValue)
{
	FILE* input_file = fopen("nvm_file.txt", "r");
	if (input_file == NULL)
	{
		perror("Error opening file");
		return 0xff;
	}

	FILE* output_file = fopen("temp.txt", "w");
	if (output_file == NULL)
	{
		perror("Error creating temporary file");
		fclose(input_file);
		return 0xff;
	}

	uint8_t attr=0; // Adjust the buffer size as needed

	// Read input file line by line
	while (fgets((char*)attr, sizeof(attr), input_file) != NULL)
	{
		uint8_t size = 0;
		if (fgets((char*)size, sizeof(size), input_file) != NULL)
		{
			char data[1000];
			while (fgets((char*)data, size, input_file) != NULL)
			{
				if (attr != attrId)
				{
					fputs((const char*)attrId, output_file);
					fputs((const char*)size, output_file);
					fputs(data, output_file);
				}
			}
		}
	}

	//now add the given set attribute
	fputs((const char*)attrId, output_file);
	fputs((const char*)length, output_file);
	fputs((const char*)pValue, output_file);

	fclose(input_file);
	fclose(output_file);
	remove("nvm_file.txt");
	// Rename temporary file to original file
	if (rename("temp.txt", "nvm_file.txt") != 0)
	{
		perror("Error renaming temporary file");
	}
	return 0;
}

char mem[1000];
void get_num(uint8_t attr, uint8_t *sz, uint8_t *val)
{
    printf("\nget_num called here:\n");
    *sz = 4;
    memcpy(val, mem, 4);
}

void set_num(uint8_t attr, uint8_t sz, uint8_t *val)
{
    printf("\n\n%d %d ", attr, sz);
    
    for(int i=0; i< sz; ++i)
        printf("\n%x", val[i]);
    
    /*float f;
    memcpy(&f, val, sz);
    printf("\nnew set_num value:%x %f", f, f);
    
    memcpy(mem, &f, sz);*/
    
    int d;
    memcpy(&d, val, sz);
    printf("\nnew set_num value:%x %d", d, d);
    
    memcpy(mem, &d, sz);
}

int main()
{
    float f = 65.8;
    printf("old f value: %x %f", f, f);
    
    uint8_t *v = &f;
    for(int i=0; i<4; ++i)
        printf("\n%x", v[i]);
    
    set_num(46, sizeof(f), (uint8_t*)&f);
    
    uint8_t len = 0;
    float ff = 0.0;
    get_num(46, &len, (uint8_t*)&ff);
    printf("\nsize: %d ff: %f", len, ff);
    
    int a = 349587;
    printf("\n a: %x %d", a, a);
    set_num(45, sizeof(a), (uint8_t*)&a);
    
    int aa = 0;
    get_num(45, &len, (uint8_t*)&aa);
    printf("\nsize: %d aa: %d", len, aa);

}
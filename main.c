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
    char *attr_s = "";
	uint8_t attr = 0; // Adjust the buffer size as needed

	// Read NV file line by line
	while (fgets(attr_s, 1, input_file) != NULL)
	{
        char *sz_s = "";
		uint8_t size = 0;
		if (fgets(sz_s, 1, input_file) != NULL)
		{
			char data[1000];
			while (fgets(data, size, input_file) != NULL)
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
    char *str_attr = "", *str_len = "";
    char str_val[1000];

    memcpy(str_attr, (char*) attrId, 1);
    memcpy(str_len, (char*) length, 1);
    memcpy(str_val, pValue, length);
    str_val[length] = '\0';

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

    char *attr_s = "";
    char *sz_s = "";
	uint8_t attr=0; // Adjust the buffer size as needed

	// Read input file line by line
	while (fgets(attr_s, 1, input_file) != NULL)
	{
		uint8_t size = 0;
		if (fgets(sz_s, 1, input_file) != NULL)
		{
			char data[1000];
			while (fgets((char*)data, size, input_file) != NULL)
			{
                memcpy(attr, attr_s, 1);
                memcpy(size, sz_s, 1);
                
				if (attr != attrId)
				{
					fputs(str_attr, output_file);
					fputs(str_len, output_file);
					fputs(str_val, output_file);
				}
			}
		}
	}

	//now add the given set attribute
	fputs(str_attr, output_file);
	fputs(str_len, output_file);
	fputs(str_val, output_file);

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

#if 0
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

#endif

void getFromFile(uint8_t attrId, uint8_t *pLength, uint8_t *pValue)
{
    FILE* input_file = fopen("nv_data.bin", "rb");
	if (input_file == NULL)
	{
		perror("Error opening file");
		return ;
	}

    uint8_t curr_attrId, curr_sz;
    while(fscanf(input_file, "%hhu", &curr_attrId) == 1)
    {
        if(fscanf(input_file, "%hhu", &curr_sz) == 1)
        {
            if(curr_attrId == attrId)
            {
                *pLength = curr_sz;
                uint8_t *old_p = pValue;
                for(int i=0; i< curr_sz; ++i)
                {
                    fscanf(input_file, "%hhu", pValue);
                    pValue++;
                }

                pValue = old_p;
                printf("\ngetFromFile found! %d %d\n", attrId, *pLength);
                for(int i=0; i< curr_sz; ++i)
                    printf("0x%x ", pValue[i]);
                break;
            }
        }
    }

	fclose(input_file);
    printf("getFromFile Complete\n");
}

void writeToFile(uint8_t attr, uint8_t sz, uint8_t *val)
{
    FILE* input_file = fopen("nv_data.bin", "rb");
	if (input_file == NULL)
	{
		perror("Error opening NV file");
		return ;
	}

	FILE* output_file = fopen("temp.bin", "ab");
	if (output_file == NULL)
	{
		perror("Error creating temporary file");
		fclose(input_file);
		return;
	}

    for(int i=0; i<sz; ++i)
        printf("0x%x ", val[i]);
	//now add the given set attribute
	fprintf(output_file, "%hhu\n%hhu\n", attr, sz);
    for(int i=0; i<sz; ++i)
        //fwrite(val++, sizeof(uint8_t), sizeof(uint8_t), output_file);
        fprintf(output_file, "%hhu\n", val[i]);

	fclose(output_file);
    printf("WriteToFile Complete\n");
}

int main()
{
    float f = 65.8f;
    printf("old f value: %f\n", f);
    
    uint8_t *v = (uint8_t*) &f;
    for(int i=0; i<4; ++i)
        printf("0x%x ", v[i]);
    
    //gpNvm_SetAttribute(46, sizeof(f), (uint8_t*)&f);
    writeToFile(45, 4, v);
    
    uint8_t len = 0;
    uint8_t val_read[1000];
    getFromFile(45, &len, val_read);

    float new_f = *((float*)val_read);
    printf("reading float value %f", new_f);

    #if 0
    uint8_t len = 0;
    float ff = 0.0;
    gpNvm_GetAttribute(46, &len, (uint8_t*)&ff);
    printf("\nsize: %d ff: %f", len, ff);
    
    int a = 349587;
    printf("\n a: %x %d", a, a);
    gpNvm_SetAttribute(45, sizeof(a), (uint8_t*)&a);
    
    int aa = 0;
    gpNvm_GetAttribute(45, &len, (uint8_t*)&aa);
    printf("\nsize: %d aa: %d", len, aa);
    #endif
}
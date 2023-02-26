#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int hash(unsigned char *data, unsigned int length);
unsigned int hash_file(char *file);

unsigned int hash(unsigned char *data, unsigned int length)
{
    unsigned int hash_int = 0;
    unsigned int *data_int = (int *)data;
    for (unsigned int i = 0; i < length / sizeof(int) / sizeof(char); i++)
    {
        hash_int ^= data_int[i];
    }
    if(length < sizeof(int))
        return hash_int ^ data_int[0];
    return hash_int;
}

unsigned int hash_file(char *name)
{
    FILE *file = fopen(name, "rb");
    // 计算文件的简易哈希值
    const int BUFFER_SIZE = sizeof(int);
    unsigned char buffer[BUFFER_SIZE];
    int bytesRead;
    unsigned int hash_int = 0;
    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, file)) > 0)
    {
        hash_int ^= hash(buffer, bytesRead);
    }
    fclose(file);

    return hash_int;
}
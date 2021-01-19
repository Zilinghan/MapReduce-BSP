#ifndef __TYPECONVERT_H__
#define __TYPECONVERT_H__
#include <vector>
#include "serialmap.h"
char *vector_to_array(std::vector<char> *input, int size);
std::vector<char> array_to_vector(char *input, int size);
char *map_to_array(serializable_map<std::string, int> *data, int *data_size);
serializable_map<std::string, int> array_to_map(char *data_array, int data_size);
float *vector_to_array_float(std::vector<float> *input, int size);
std::vector<float> array_to_vector_float(float *input, int size);
#endif
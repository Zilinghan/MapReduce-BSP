#include <vector>
#include "serialmap.h"
using namespace std;

/* typerconvert.cpp
 * these following four functions make some necessary 
 * type-conversion we may need in MapReduce. Conversion
 * to array is needed becase the communication between
 * different agents/processes required the communicated
 * data take a continuous region in the memory (array form).
 */

/* vector_to_array: 
 * This function converts a vector of chars into an array of chars
 */
char *vector_to_array(vector<char> *input, int size)
{
    char *output = (char *)malloc(size);
    copy((*input).begin(), (*input).end(), output);
    return output;
}

/* array_to_vector:
 * This function converts an array of chars into a vector of chars
 */
vector<char> array_to_vector(char *input, int size)
{
    vector<char> output;
    output.assign(input, input + size);
    return output;
}

/* map_to_array:
 * This function converts a map (a list of key-value pairs) into an array 
 * of chars. It first serialize the map into vector of chars and then employs
 * the vector_to_array function.
 */
char *map_to_array(serializable_map<string, int> *data, int *data_size)
{
    vector<char> data_vector = data->serialize();
    *data_size = data_vector.size() * sizeof(char);
    char *data_array = vector_to_array(&data_vector, *data_size);
    return data_array;
}

/* array_to_map:
 * This function converts back the array of chars back to the map it 
 * initially represents. It first convert array to vector, then use
 * the deserialize function to get the map back.
 */
serializable_map<string, int> array_to_map(char *data_array, int data_size)
{
    vector<char> data_vector = array_to_vector(data_array, data_size);
    serializable_map<string, int> data;
    data.deserialize(data_vector);
    free(data_array);
    return data;
}

/* vector_to_array_float
 * We need to convert the vector of floats to array of floats since the communication
 * only supports sending data in some continous memory region.
 */
float *vector_to_array_float(vector<float> *input, int size)
{
    float *output = (float *)malloc(size);
    copy((*input).begin(), (*input).end(), output);
    return output;
}

/* array_to_vector_float
 * Convert the received data (of type array) back to vector
 */
vector<float> array_to_vector_float(float *input, int size)
{
    vector<float> output;
    output.assign(input, input + size);
    return output;
}
#include <bsp.h>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <sstream>
#include <stdio.h>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include "serialmap.h"
#include "typeconvert.h"
#include "temp_mapreduce.h"
using namespace std;
#define N 4;
/* ------------------------MapReduce Memoryless Example-------------------------
 * Explanations:
 * This example implements the easiest type of Stream Query: The Memoryless Type.
 * We have a file containing the temperature collected from the temperature sensor.
 * In each MapReduce cycle, reading only a small portion of the data to simulate
 * that the temperature data comes as data streams. Each MapReduce cycle, the 
 * processors will print out all the temperature samples which are greater than 37.5
 * For this MapReduce Algorithm example, both the Map phase and Reduce phase are
 * easy and trivial, but it reveals the basic idea of stream queries.
 * (Note: there are overall 1,000,000 data in the given file)
 * Map phase:
 * Each processor receives the data send by the master and send out the "high T"
 * to the "reduce processor".
 * Reduce phase:
 * Each processor receives the data send by the "map processors" and print out 
 * the "high T" together with some warning messages to the monitor. 
 */
int main()
{
    bsp_begin(bsp_nprocs());
    /* just used for compliling */
    if (bsp_nprocs() == 1)
    {
        bsp_end();
        return 1;
    }
    /* define the file system */
    vector<string> file_blocks;
    file_blocks.push_back("d:\\bsponmpi\\test\\data0.txt");
    file_blocks.push_back("d:\\bsponmpi\\test\\data1.txt");
    file_blocks.push_back("d:\\bsponmpi\\test\\data2.txt");
    file_blocks.push_back("d:\\bsponmpi\\test\\data3.txt");
    file_blocks.push_back("d:\\bsponmpi\\test\\data4.txt");
    file_blocks.push_back("d:\\bsponmpi\\test\\data5.txt");
    file_blocks.push_back("d:\\bsponmpi\\test\\data6.txt");
    file_blocks.push_back("d:\\bsponmpi\\test\\data7.txt");
    /* define the local variables needed in mapreduce */
    int nonzeros, nonzeros_size, status, i, data_size;
    int end_file_flag = 0; // indicates that local file has been finished
    int reduce_flag = 0;   // indicates that processor is in reduce phase
    int reduce_proc = 0;   // indicates the processor which operates reduce next cycle
    vector<float> map_out;
    /* open the file which corresponds to the processor */
    char *myfile = (char *)(file_blocks[bsp_pid()].c_str());
    ifstream infile(myfile);
    if (!infile.is_open())
    {
        std::cerr << "there was a problem opening the input file!\n";
        exit(1); //exit or do additional error checking
    }
    /* begin the mapreduce cycle:
	 * while(1) indicates that it works forever theoretically since the data is assumed
	 * to come as infinite data streams. but in practical and in this simulation, the 
	 * mapreduce cycle will end(break) when 
	 */
    while (1)
    {
        /* check the input queue of the processor to determine the phase */
        bsp_get_tag(&status, nullptr);
        if (status == -1)
        {
            reduce_flag = 0;
        }
        else
        {
            reduce_flag = 1;
        }

        /* if this processor is in the map phase */
        if (reduce_flag == 0)
        {
            /* read the data for each cycle and store the data into vector */
            float data_sample;
            int data_size = 10000;
            vector<float> data;
            for (i = 0; i < data_size; i++)
            {
                infile >> data_sample;
                data.push_back(data_sample);
                if (infile.eof())
                { // check whether the file is at the end
                    end_file_flag = 1;
                    infile.close();
                    bsp_sync();
                    break;
                }
            }
            if (end_file_flag == 1)
            { // when file is finished, break from loop
                break;
            }
            /* invoke the map function to manipulate the data */
            map_out = map_function(data);
            /* send the result of map to processor for reduce */
            // serialize the vector to array
            data_size = map_out.size() * sizeof(float);
            float *data_array = vector_to_array_float(&map_out, data_size);
            bsp_send(reduce_proc, nullptr, data_array, data_size);
            free(data_array);
            reduce_proc = (reduce_proc + 1) % bsp_nprocs(); // update the reduce processor
            bsp_sync();
        }

        /* if this processor is in the reduce phase */
        else
        {
            /* receive data from the input queue and operate reduce function */
            bsp_qsize(&nonzeros, &nonzeros_size);
            for (i = 0; i < nonzeros; i++)
            {
                bsp_get_tag(&status, nullptr);
                float *collectdata_array = (float *)malloc(status);
                bsp_move(collectdata_array, status);
                int array_size = sizeof(collectdata_array);
                array_size = array_size / sizeof(float);
                vector<float> collect_data = array_to_vector_float(collectdata_array, array_size);
                //////cout << collect_data[0] << endl;
                reduce_function(collect_data);
            }
            reduce_proc = (reduce_proc + 1) % bsp_nprocs();
            bsp_sync();
        }
    }
    bsp_end();
    return 0;
}

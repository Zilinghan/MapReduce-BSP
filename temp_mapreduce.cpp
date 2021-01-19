#include <vector>
#include <iostream>
using namespace std;
/* ---------------------------------Map Function--------------------------------
 * Input: data: A vector contains the local data in current cycle
 * Output: highT: A vector contains all values which are higher that 37.5
 */
vector<float> map_function(vector<float> data)
{
    vector<float>::iterator it;
    vector<float> highT;
    float threshold = 37.5;
    for (it = data.begin(); it != data.end(); it++)
    {
        if (*it >= 37.5)
        {
            highT.push_back(*it);
            cout << "The temperature is: " << *it << ". Take care!" << endl;
        }
    }
    return highT;
}

/* --------------------------------Reduce Function------------------------------
 * Input: highT: A vector contains all values which are higher that 37.5
 * Output: No explicit output. This reduce function is trivial. Just print 
 *         out the high temperature with some warning messages.
 */
void reduce_function(vector<float> highT)
{
    vector<float>::iterator it;
    for (it = highT.begin(); it != highT.end(); it++)
    {
        cout.precision(8);
        //cout << "The temperature is: " << *it << ". Take care!" << endl;
    }
    return;
}
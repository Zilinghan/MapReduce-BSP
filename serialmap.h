#ifndef __SERIALMAP_H__
#define __SERIALMAP_H__
/*
# Copyright H. Watanabe 2017
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)
*/
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
/* ---------------------------Serialization of Type Map-------------------------
 * The serialization of Map requires to define a new data type: serializable_map,
 * which is inherited  from type Map.
 *
 * Two main functions responsible for the type conversion:
 * serialize(): change map<Key, Value> to vector<char>
 * deserialize(): change vector<char> back to map<Key, Value>
 */
template <class Key, class Value>
class serializable_map : public std::map<Key, Value>
{
private:
    size_t offset;

    template <class T>
    void write(std::stringstream &ss, T &t)
    {
        ss.write((char *)(&t), sizeof(t));
    }

    void write(std::stringstream &ss, std::string &str)
    {
        size_t size = str.size();
        ss.write((char *)(&size), sizeof(size));
        ss.write((char *)(str.data()), str.length());
    }

    template <class T>
    void read(std::vector<char> &buffer, T &t)
    {
        t = (T)(*(buffer.data() + offset));
        offset += sizeof(T);
    }

    void read(std::vector<char> &buffer, std::string &str)
    {
        size_t size = (int)(*(buffer.data() + offset));
        offset += sizeof(size_t);
        std::string str2(buffer.data() + offset, buffer.data() + offset + size);
        str = str2;
        offset += size;
    }

public:
    std::vector<char> serialize()
    {
        std::vector<char> buffer;
        std::stringstream ss;
        for (auto &i : (*this))
        {
            Key str = i.first;
            Value value = i.second;
            write(ss, str);
            write(ss, value);
        }
        size_t size = ss.str().size();
        buffer.resize(size);
        ss.read(buffer.data(), size);
        return buffer;
    }
    void deserialize(std::vector<char> &buffer)
    {
        offset = 0;
        while (offset < buffer.size())
        {
            Key key;
            Value value;
            read(buffer, key);
            read(buffer, value);
            (*this)[key] = value;
        }
    }
    void show(void)
    {
        for (auto &i : (*this))
        {
            std::cout << i.first << ":" << i.second << std::endl;
        }
        std::cout << std::endl;
    }
};
#endif

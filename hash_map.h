#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "../vector.h"

template <unsigned int hash_size, class K, class T>
class hash_map
{
private:
    vector <K> m_key_hash[hash_size];
    vector <T> m_value_hash[hash_size];
public:
    hash_map();
    int size () const;
    void insert (K key, T value, unsigned int hashcode);
    void remove (K key, T value, unsigned int hashcode);
    vector <T> query (K key, unsigned int hashcode);
};

#endif // HASH_MAP_H

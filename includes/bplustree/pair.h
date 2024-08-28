#ifndef PAIR_H
#define PAIR_H

using namespace std;
#include <iostream>
#include <vector>


template <typename K, typename V>
struct Pair{
    K key;
    V value;

    Pair(const K& k=K(), const V& v=V()){
        key = k;
        value = v;
    }
    friend ostream& operator <<(ostream& outs, const Pair<K, V>& print_me){
        outs << print_me.key << ":";
        outs << print_me.value;
        return outs;
    }
    friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        if (lhs.key == rhs.key) return true;
        return false;
    }
    friend bool operator < (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
       if (lhs.key != rhs.key) return true;
        return false;
    }
    friend bool operator > (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        if (lhs.key > rhs.key) return true;
        return false;
    }
    friend bool operator <= (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        if (lhs.key <= rhs.key) return true;
        return false;
    }
    friend Pair<K, V> operator + (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        lhs->value = rhs->value;
        return lhs;
    }
};

#endif
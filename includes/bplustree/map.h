#ifndef MAP_H
#define MAP_H

#include "bplustree.h"
#include "pair.h"

using namespace std;
#include <iostream>
#include <vector>
#include <cassert>

template <typename K, typename V>
class Map
{
public:
    typedef BPlusTree<Pair<K,V>> map_base;
    class Iterator{
    public:
        friend class Map;
        Iterator(typename map_base::Iterator it = nullptr){
            _it = it;
        }
        Iterator operator ++(int unused){
            typename map_base::Iterator it2 = _it;
            assert(_it != NULL);
            _it++;
            return it2;
        }
        Iterator operator ++(){
            assert(_it != NULL);
            _it++;
            return _it;
        }
        Pair<K,V> operator *(){
            return *_it;
            // Pair<K,V> ptr;
            // return ptr;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            if (lhs._it == rhs._it) return true;
            return false;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            if (lhs._it != rhs._it) return true;
            return false;
        }
    private:
        typename map_base::Iterator _it;

    };

    Map(){
        key_count = 0;
    }
//  Iterators
    Iterator begin(){
        cout << *map.begin();
        return Iterator(map.begin());
    }
    Iterator end(){
        return Iterator(NULL);
    }

//  Capacity
    int size() const{
        return map.size();
    }
    bool empty() const{
        return size() == 0;
    }

//  Element Access
    V& operator[](const K& key){
        if (!map.contains(key)){
            V v = 0;
            insert(key,v);
        }
        return map.get(key).value;
        // Pair<K, V> temp = map.get(key);
        // V* ptr;
        // return temp.value;
    }
    V& at(const K& key){
        return map.get(key).value;
        // V* ptr;
        // return *ptr;
    }
    const V& at(const K& key) const{
        return map.get(key).value;
        // V* ptr;
        // return *ptr;
    }


//  Modifiers
    void insert(const K& k, const V& v){
        map.insert(Pair<K,V>(k,v));
        key_count++;
    }
    void erase(const K& key){
        map.remove(Pair<K,V>(key));
    }
    void clear(){
        map.clear_tree();
    }
    V get(const K& key){
        V temp = map.get(Pair<K,V>(key)).value;
        return temp;
    }

//  Operations:
    Iterator find(const K& key){
        return map.find(key);
    }
    bool contains(const K& target) const{
        return map.contains(target);
    }

    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:
     int count(const K& key){
        return 0;
     }
    Iterator lower_bound(const K& key){
        return map.lower_bound(key);
    }
     Iterator upper_bound(const K& key){
        return map.upper_bound(key);
    }
    


    bool is_valid(){return map.is_valid();}

    friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me){
        outs<<print_me.map<<endl;
        return outs;
    }
private:
    int key_count;
    BPlusTree<Pair<K, V> > map;
};

#endif
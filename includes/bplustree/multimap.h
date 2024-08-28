#ifndef MULTIMAP_H
#define MULTIMAP_H

#include "bplustree.h"
#include "mpair.h"

using namespace std;
#include <iostream>
#include <vector>
#include <cassert>


template <typename K, typename V>
class MMap
{
public:
    typedef BPlusTree<MPair<K, V>> map_base;
    class Iterator{
    public:
        friend class MMap;
        Iterator(typename map_base::Iterator it){
            _it = it;
        }
        Iterator(){

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
        MPair<K, V> operator *(){
            // if (_it == NULL) return NULL;
            return *_it;
        }

        bool isNull(){
            return _it.is_null();
        }
        bool isNextNULL(){
            return _it.isNextNull();
        }
        
        bool end(){
            return _it.end();
        }
            
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            return (lhs._it == rhs._it);
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return (lhs._it != rhs._it);
        }
        friend ostream& operator <<(ostream& outs, const Iterator& print_me){
        // outs << "test";
        outs << " ";
        return outs;
    }

    private:
        typename map_base::Iterator _it;
    };

    MMap(){
      
    }

//  Iterators
    Iterator begin(){
        return Iterator(mmap.begin());
    }
    Iterator end(){
        return Iterator(NULL);
    }

//  Capacity
    int size() const{return mmap.size();}
    bool empty() const{return size() == 0;}

//  Element Access
    const vector<V>& operator[](const K& key) const{
        
       if (!mmap.contains(key)){
            V v;
            insert(key,v);
        }
        return mmap.get(key).value_list;
    }

    vector<V>& operator[](const K& key){
        if (!mmap.contains(key)){
            V v;
            insert(key,v);
        }
        return mmap.get(key).value_list;
    }

//  Modifiers
    void insert(const K& k, const V& v){
        
        mmap.insert(MPair<K,V>(k,v));
    }
    void erase(const K& key){

    }
    void clear(){
        mmap.clear_tree();
    }

//  Operations:
    bool contains(const K& key) const {
        return mmap.contains(key);
    }
    vector<V> &get(const K& key){
        return mmap.get(key).value_list;
    }

    Iterator find(const K& key){
        return mmap.find(key);
    }
    // int count(const K& key);
    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:
    int count(const K& key){
        return mmap.get(key).value_list.size();
    }
    Iterator lower_bound(const K& key){
        return mmap.lower_bound(key);
    }
    Iterator upper_bound(const K& key){
        return mmap.upper_bound(key);
    }

    string at (const K& key){
        return "0";
    }

    bool is_valid(){
        return mmap.is_valid();
    }

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
        outs<<print_me.mmap<<endl;
        return outs;
    }

    vector<V> lessThan(const K& key){
        vector<V> vec;
        MMap<K, V>::Iterator it = lower_bound(key);
        MMap<K, V>::Iterator temp = begin();
        MPair<K, V> test;
        if (it.isNull()){
            return vec;
        }
         
        test = *temp;
        vec + test.value_list; 
        while (*it != *temp){
            temp++;
            test = *temp;
            vec + test.value_list;
        }
       
        return vec;
    }

    vector<V> greaterThan(const K& key){
        vector<V> vec;
        Iterator its = mmap.upper_bound(key);
        if (its.isNull()){
            return vec;
        }
        MPair<K, V> test;
        while (!its.end()){
            test = *its;
            vec + test.value_list;
            its++;
        }
        test = *its;
        vec + test.value_list;
        return vec;
    }

    vector<V> greaterEqualThan(const K& key){
        vector<V> vec;
        Iterator its = mmap.lower_bound2(key);
        MPair<K, V> test;
        if (its.isNextNULL()){
            return vec;
        }  
        its++;
        while (!its.end()){
            test = *its;
            vec + test.value_list;
            its++;
        }
        test = *its;
        vec + test.value_list;
        return vec;
    }

    vector<V> lessEqualThan(const K& key){
        vector<V> vec;
        MMap<K, V>::Iterator it = upper_bound(key);
        MMap<K, V>::Iterator temp = begin();
        MPair<K, V> test;
        if (it.isNull()){
            while (!temp.end()){
                test = *temp;
                vec + test.value_list;
                temp++;
            }
            test = *temp;
            vec + test.value_list;
            return vec;
        }
       
        while (*it != *temp){
            test = *temp;
            vec + test.value_list;
            temp++;
        }
        return vec;
    }

    void print_lookup(){
        Iterator it = begin();
        if (it.end()) return ;
        cout << endl;
        cout << setw(15) << *it << endl;
        while (!it.end()){
            it++;
            cout << setw(15) << *it << endl;
        }
    }

private:
    BPlusTree<MPair<K, V> > mmap;
};
#endif
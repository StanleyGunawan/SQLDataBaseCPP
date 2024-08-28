#ifndef NODE_H
#define NODE_H
#include <iostream>
using namespace std;
template <class T>
struct node{
    T _data;
    node<T>* _next;
    node<T>* _prev;
    node(const T& data = T(), node<T>* next = NULL, node<T>* prev = NULL):
                                                                       _data(data), 
                                                                       _next(next),
                                                                       _prev(prev){}
    friend ostream& operator << (ostream& outs, const node<T>& print_me){
        outs<<"<-["<<print_me._data<<"]-> ";
        return outs;
    }

};

#endif //NODE_H


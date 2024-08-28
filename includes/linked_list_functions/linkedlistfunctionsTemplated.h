#ifndef LINKEDLISTFUNCTIONSTEMPLATED_H
#define LINKEDLISTFUNCTIONSTEMPLATED_H


#include "../node/node.h"
#include <iostream>
#include <cassert>
using namespace std;

template <typename T>
void _print_list(node<T>* head){
    node<T>* walker = head;
    cout << "Head->";
    cout << *walker;
    while (walker -> _next != NULL){
        walker = walker -> _next;
        cout << *walker;
    }
    cout << "|||" << endl;;
}

template <typename T>
void _print_list_backwards(node<T> *head){
    node<T>* walker = head;
    while (walker -> _next != NULL){
        walker = walker -> _next;
    }
    while (walker != head){
        cout << *walker ;
        walker = walker -> _prev;
    }
    cout << *walker ;
    cout << endl;
}

template <typename T>
node<T>* _search_list(node<T>* head, T key){
    node<T>* walker = head;
    while (walker -> _data != key && walker ->_next != NULL){
        walker = walker -> _next;
    }
    return walker;
}

template <typename T>
node<T>* _insert_head(node<T> *&head, T insert_this){
    node<T>* newNode = new node<T>;
    newNode -> _data = insert_this;
    if (head == NULL){
        head = newNode;
        return newNode;
    }
    newNode -> _next = head;
    head -> _prev = newNode;
    head = newNode;
    return newNode;
}


template <typename T>
node<T>* _insert_after(node<T>*& head, node<T> *after_this, T insert_this){
    node<T>* newNode = new node<T>;
    newNode -> _data = insert_this;
    if (after_this -> _next != NULL){
        node<T>* nextNode = after_this -> _next;
        newNode -> _next = nextNode;
        nextNode -> _prev = newNode;
    }
    after_this -> _next = newNode;
    newNode -> _prev = after_this;

    return newNode;
}

template <typename T>
node<T>* _insert_before(node<T>*& head, node<T>* before_this, T insert_this){
    node<T>* newNode = new node<T>;
    newNode -> _data = insert_this;
    if (before_this -> _prev != NULL){
        node<T>* prevNode = before_this -> _prev;
        prevNode -> _next = newNode;
        newNode -> _prev = prevNode;
    }
    if (head == NULL || before_this -> _prev == NULL){
        head = newNode;
    }
    before_this -> _prev = newNode;
    newNode -> _next = before_this;

    return newNode;

}

template <typename T>
node<T>* _previous_node(node<T>* prev_to_this){
    return prev_to_this -> _prev;
}            

template <typename T>
T _delete_node(node<T>*& head, node<T>* delete_this){
    if(delete_this -> _prev == NULL && delete_this -> _next == NULL){
        head = NULL;
    }
    else if (delete_this -> _next == NULL){
        delete_this -> _prev -> _next = NULL;
    }
    else if (delete_this -> _prev == NULL || delete_this == head){
        head = delete_this -> _next;
        head -> _prev = NULL;
    }
    else{       //if have front and back nodes
        node<T> *prev = delete_this -> _prev;
        node<T> *next = delete_this -> _next;
        prev -> _next = next;
        next -> _prev = prev;
    }
    T temp = delete_this -> _data;
    delete delete_this;
    return temp;
}

template <typename T>
node<T>* _copy_list(node<T>* head){
    assert(head != NULL && "List is empty");
    node<T>* current = new node<T>(head->_data);
    node<T>* temp = current;
    node<T>* head2 = current;
    head = head -> _next;
    while (head -> _next != NULL){
        current = new node<T>(head->_data);
        temp -> _next = current;
        current -> _prev = temp;
        temp = temp -> _next;
        head = head -> _next;
    }
    current = new node<T>(head->_data);
    temp -> _next = current;
    current -> _prev = temp;
    return head2;
}

//duplicate the list, return pointer to last node in dest... 
//     use this function in your queue big three 
template <typename T>
node<T> *_copy_list(node<T> *&dest, node<T> *src){
    if (src == NULL){
        return dest;
    }
    node<T>* current = new node<T>(src->_data);
    node<T>* temp = current;
    dest = current;
    src = src -> _next;
    while (src -> _next != NULL){
        current = new node<T>(src->_data);
        temp -> _next = current;
        current -> _prev = temp;
        temp = temp -> _next;
        src = src -> _next;
    }
    current = new node<T>(src->_data);
    temp -> _next = current;
    current -> _prev = temp;
    return current;
}


template <typename T>
void _clear_list(node<T>*& head){
    node<T>* current  = head;
    node<T>* temp;
    while (current != NULL && current -> _next != NULL){
        temp = current -> _next;
        delete current;
        current = temp;
    }
    
    head = NULL;
}

template <typename T>
T& _at(node<T>* head, int pos){
    node<T>* walker = head;
    for (int i = 0; i < pos; i++){
        assert(walker ->_next != NULL);
        walker = walker-> _next;
    }
    return walker -> _data;
}

#endif
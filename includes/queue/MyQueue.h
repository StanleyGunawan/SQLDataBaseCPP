#ifndef MYQUEUE_H
#define MYQUEUE_H

#include "../linked_list_functions/linkedlistfunctionsTemplated.h"

template <typename T>
class Queue
{
public:
    class Iterator{
    private:
        node<T>* _ptr;                          //pointer being encapsulated
    public:
        friend class Queue;                               //give access to list to access _ptr
        Iterator(){_ptr = NULL;}                          //default ctor

        Iterator(node<T>* p){
            _ptr = p;
        }                            //Point Iterator to where p is pointing to

        T& operator *(){
            assert(_ptr);
            return _ptr -> _data;
        }
         //dereference operator
        bool is_null(){                 //true if _ptr is NULL
            return (_ptr == NULL);
        }

        friend bool operator !=(const Iterator& left, const Iterator& right)  //true if left != right
        {
            return left._ptr != right._ptr;
        }

        friend bool operator ==(const Iterator& left, const Iterator& right)  //true if left == right
        {
            return left._ptr == right._ptr;
        }


        Iterator& operator++(){                         //member operator:++it  or ++it = new_value
            _ptr++;
            return *this;
        }

        friend Iterator operator++(Iterator& it, int unused){         //friend operator: it++
            assert(it._ptr!=NULL);
            Iterator hold = it;
            if(it._ptr->_next!=NULL){
                it._ptr = it._ptr -> _next;
            }
            return hold;
        }

        Iterator& operator--(){                         //member operator:++it  or ++it = new_value
            _ptr--;
            return *this;
        }

        friend Iterator operator--(Iterator& it, int unused){         //friend operator: it++
            assert(it._ptr!=NULL);
            Iterator hold = it;
            if(it._ptr->_prev!=NULL){
                it._ptr = it._ptr -> _prev;
            }
            return hold;
        }
    };

    Queue(){
        _front = NULL;
        _rear = NULL;
        _size = 0;
    }

    ~Queue(){
        _clear_list(_front);
        _size = 0;
        _front = NULL;
        _rear = NULL;
    }
    Queue(const Queue<T>& copyMe){
        assert(copyMe._front != NULL && "Queue is NULL");
        _rear = _copy_list(_front,copyMe._front);
        _size = copyMe._size;
    }
    Queue& operator=(const Queue<T>& RHS){
        // assert(RHS._front != NULL && "Queue is NULL");
        _rear = _copy_list(_front,RHS._front);
        _size = RHS._size;
        return *this;
    }

    bool empty(){
        return _size < 1;
    }
    T front(){
        assert(!empty() && "_front is NULL");
        return _front -> _data;
    }
    T back(){
        assert(!empty() && "_rear is NULL");
        return _rear ->_data;
    }

    void push(T item){
        if(_size == 0){
            _front = _insert_head(_front,item);
            _rear = _front;
        }
        else{
            _rear = _insert_after(_front,_rear,item);
        }
       _size++;
    }

    T pop(){
        assert(_size > 0);
        node<T>* pop_node;
        pop_node = _front;
        _size--;
        return _delete_node(_front, pop_node);
    }

    Iterator begin() const{     //Iterator to the head node
        return Iterator(_front);
    }

    Iterator end() const{        //Iterator to NULL
        return Iterator(NULL);
    }

    int size() const { return _size; }

    template<typename TT>
    friend ostream& operator << (ostream& outs, const Queue<TT>& printMe){
        outs << "Queue ";
        _print_list(printMe._front);
       return outs;
    }

private:
    node<T>* _front ;
    node<T>* _rear ;
    int _size ;
};  

#endif
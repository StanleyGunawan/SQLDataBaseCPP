#ifndef MYSTACK_H
#define MYSTACK_H

#include "../linked_list_functions/linkedlistfunctionsTemplated.h"

template <typename T>
class Stack
{
public:
    class Iterator{
    private:
        node<T>* _ptr;                          //pointer being encapsulated
    public:
        friend class Stack;                               //give access to list to access _ptr
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
    };

    Stack(){
        _top = NULL;
        _size = 0;
    }

    ~Stack(){
        _clear_list(_top);
        _size = 0;
        _top = NULL;
    }
    Stack(const Stack<T>& copyMe){
        assert(copyMe._top != NULL && "Stack is NULL");
        _top = _copy_list(_top,copyMe._top);
        _size = copyMe._size;
    }
    Stack& operator=(const Stack<T>& RHS){
        assert(RHS._top != NULL && "Stack is NULL");
        _top = _copy_list(_top,RHS._top);
        _size = RHS._size;
        return *this;
    }

    bool empty(){
        return (_size <= 0);
    }
    T top(){
        assert(!empty() && "_front is NULL");
        return _top -> _data;
    }

    void push(T item){
        _top = _insert_head(_top,item);
        // if(_size == 0){
        //     _top = _insert_head(_top,item);
        // }
        // else{
        //     _insert_before(_top,_top,item);
        // }
       _size++;
    }

    T pop(){
        assert(_size > 0);
        node<T>* pop_node;
        pop_node = _top;
        _size--;
        return _delete_node(_top, pop_node);
    }

    Iterator begin() const{     //Iterator to the head node
        return Iterator(_top);
    }

    Iterator end() const{        //Iterator to NULL
        return Iterator(NULL);
    }

    int size() const { return _size; }

    template<typename TT>
    friend ostream& operator << (ostream& outs, const Stack<TT>& printMe){
        outs << "Queue ";
        _print_list(printMe._top);
       return outs;
    }

private:
    node<T>* _top ;
    int _size ;
};  

#endif
#ifndef BPLUSTREE
#define BPLUSTREE

#include <iostream>
#include "btree_array_funcs.h"

using namespace std;
#include <iostream>
#include <vector>
#include <cassert>

template <class T>
class BPlusTree
{
public:
    class Iterator
    {
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T> *_it = NULL, int _key_ptr = 0)
        {
            it = _it;
            key_ptr = _key_ptr;
        }

        T operator*()
        {
            return it -> data[key_ptr];
        }

        // string operator*(){
        //     return it->in_order();
        // }
        

        Iterator operator++(int un_used)
        {
            assert(it != NULL);
            BPlusTree<T>* it2 = it;
            key_ptr++;
            int i = it->data_count;
            if (key_ptr >= i){
                if(it->next!=NULL){
                    it = it -> next;
                    key_ptr = 0;
                }
                else{
                    assert(it->next != NULL && "Reached end of linkedlist");
                    // return NULL;
                }
            }
            return it2;
        }

        Iterator operator++() {  
            assert(it!=NULL);
            key_ptr++;
            if (key_ptr >= it->data_count){
                if(it->next!=NULL){
                    it = it -> next;
                    key_ptr = 0;
                }
                else{
                    assert(it->next != NULL && "Reached end of linkedlist");
                }
            }
            return it; 
        }
        bool isNextNull(){
            return it->next == NULL;
        }
        friend bool operator==(const Iterator &lhs, const Iterator &rhs)
        {
            return lhs.it == rhs.it;
        }
        friend bool operator!=(const Iterator &lhs, const Iterator &rhs)
        {
            return lhs.it != rhs.it;
        }
        void print_Iterator()
        {
            if (it)
            {
                cout << "iterator: ";
                print_array(it->data, it->data_count, key_ptr);
            }
            else
            {
                cout << "iterator: NULL, key_ptr: " << key_ptr << endl;
            }
        }
        bool is_null() { return (it == NULL); }
        void info()
        {
            cout << endl
                 << "Iterator info:" << endl;
            cout << "key_ptr: " << key_ptr << endl;
            cout << "it: " << *it << endl;
        }
        bool end(){
            if (it == NULL){
                return true;
            }
            else if (it->next==NULL ){
                if (key_ptr >= it->data_count-1){
                    return true;
                }
            }
            return false;
        }

    private:
        BPlusTree<T> *it;
        int key_ptr;
    };


     Iterator find(const T &key)
    { // return an iterator to this key.  NULL if not there.
        if (!is_leaf()){
            return get_smallest_node()->find(key);
        }
        int index = first_ge(data,data_count,key);
        if (data[index] == key){
            return Iterator(this,index);
        }
        else if (next != NULL){
            return next->find(key);
        }
        return Iterator(NULL);
        
    }

    Iterator lower_bound(const T &key)
    { // return first that goes NOT BEFORE key entry or next if does not exist: >= entry
        if (!is_leaf()){
            return get_smallest_node()->lower_bound(key);
        }
        if(next != NULL && next->data[0] < key){
            return next->lower_bound(key);
        }
        if (key <= data[0]){
            return Iterator(NULL);
        }
        int i = 0;
        while (i < data_count-1 && data[i+1] < key){
            i++;
        }

        return Iterator(this,i);
    }

    Iterator lower_bound2(const T &key)
    { // return first that goes NOT BEFORE key entry or next if does not exist: >= entry
        if (!is_leaf()){
            return get_smallest_node()->lower_bound2(key);
        }
        if(next != NULL && next->data[0] < key){
            return next->lower_bound2(key);
        }
        if (key <= data[0]){
            return Iterator(this,-1);
        }
        int i = 0;
        while (i < data_count-1 && data[i+1] < key){
            i++;
        }   
        return Iterator(this,i);
    }

    Iterator upper_bound(const T &key)
    { // return first that goes AFTER key exist or not, the next entry >entry
         if (!is_leaf()){
            return get_smallest_node()->upper_bound(key);
        }
        if (next != NULL && data[data_count - 1] <= key){
            return next -> upper_bound(key);
        }
        int i = 0;
        while (i <= data_count){
            if (data[i] > key){
                return Iterator(this,i);
            }
            i++;
        }
        return Iterator(NULL);
    }

    Iterator upper_bound2(const T &key){
        if (!is_leaf()){
            return get_smallest_node()->upper_bound2(key);
        }
         int i = first_ge(data,data_count,key);
        if (next != NULL && i == data_count){
            return next -> upper_bound2(key);
        }
        if (data[i] == key){
            if (i < data_count-1){
                i++;
            }
            else if (next != NULL){
                return next -> upper_bound2(key);
            }
        }
        if (data[i] == key || i == data_count){
            return Iterator(this,data_count-1);
        }
        return Iterator(this,i);
    }

    BPlusTree(bool dups = false)
    {
        child_count = 0;
        next = NULL;
        data_count = 0;
    }
    BPlusTree(T *a, int size, bool dups = false)
    {
        child_count = 0;
        next = NULL;
        data_count = 0;
        for (int i = 0; i < size; i++)
        {
            insert(a[i]);
        }
    }

    // big three:
    BPlusTree(const BPlusTree<T> &other)
    {
        copy_tree(other);
    }
    ~BPlusTree()
    {
        clear_tree();
    }
    BPlusTree<T> &operator=(const BPlusTree<T> &RHS)
    {
        copy_tree(RHS);
        return *this;
    }

    void copy_tree(const BPlusTree<T> &other)// copy other into this object
    {
        BPlusTree<T> *ptr = nullptr;
        copy_tree(other, ptr);

    } 
    void copy_tree(const BPlusTree<T> &other, BPlusTree<T> *&last_node)
    {
        copy_array(data, other.data, data_count, other.data_count);
        child_count = other.child_count;
        if (is_leaf())
        {
            next = last_node;
            last_node = this;
            return;
        }
        for (int i = child_count - 1; i >= 0; i--)
        {
            subset[i] = new BPlusTree<T>;
            subset[i]->copy_tree(*other.subset[i], last_node);
        }
    }

    void insert(const T &entry) // insert entry into the tree
    { 
        loose_insert(entry);
        if (data_count > MAXIMUM)
        {
            BPlusTree *new_tree = new BPlusTree<T>;
            copy_array(new_tree->data, data, new_tree->data_count, data_count);
            copy_array(new_tree->subset, subset, new_tree->child_count, child_count);
            subset[0] = new_tree;
            data_count = 0;
            child_count = 1;
            fix_excess(0);
        }
    }
    void remove(const T &entry) {
        if (find(entry) == NULL){
            return;
        }
        loose_remove(entry); 
        if (data_count == 0 && child_count > 0){
            BPlusTree* child = subset[0];
            copy_array(data,child->data,data_count,child->data_count);
            copy_array(subset,child->subset,child_count,child->child_count);
            child ->data_count = 0;
            child->child_count = 0;
            child->clear_tree();
        }
    } // remove entry from the tree
    void clear_tree()
    {
        data_count = 0;
        for (int i = 0; i < child_count; i++)
        {
            subset[i]->clear_tree();
        }
        if (is_leaf())
        {
            next = NULL;
        }
        child_count = 0;
    } // clear this object (delete all nodes etc.)

    bool contains(const T &entry) const
    { // true if entry can be found
        int index = first_ge(data, data_count, entry);
        bool found = data[index] == entry && index < data_count;
        if (is_leaf())
        {
            if (found) {
                return true;
            }
            return false;
        }
        if (found)
        {
            return true;
        }
        else
        {
            return subset[index]->contains(entry);
        }
    }

    T &get(const T &entry)
    { // return a reference to entry
        // If entry is not in the tree, add it to the tree
        // assert(contains(entry));
        const bool debug = false;
        if (!contains(entry))
            insert(entry);

        return get_existing(entry);
    }
    const T &get(const T &entry) const
    { // return a reference to entry
        int index = first_ge(data, data_count, entry);
        if (data[index] == entry && index < data_count)
        {
            T item = subset[index + 1]->get_smallest_node()->data[0];
            return item;
        }
        if (is_leaf())
            assert(is_leaf());
        return subset[index]->find_ptr(entry);
    }
    T &get_existing(const T &entry)
    { // return a reference to entry
        const bool debug = false;
        int i = first_ge(data, data_count, entry);
        bool found = (i < data_count && data[i] == entry);
        if (is_leaf()){
            if (found)
            {
                return data[i];
            }
            else
            {
                if (debug)
                    cout << "get_existing was called with nonexistent entry" << endl;
            }
        }
        if (found) // inner node
            return subset[i + 1]->get(entry);
        else // not found yet...
            return subset[i]->get(entry);
    }


    int size() const
    { // count the number of elements
       int size = 0;
        if (is_leaf()){
            for (int i = 0; i < data_count; i++){
                size++;
            }
        }
        for (int i = 0; i < child_count ; i++ ){
            size += subset[i] -> size();
            if (i < data_count) size++;
        }
        return size;
    }

    bool empty() const { return size() == 0; } // true if the tree is empty

    void print_tree(int level = 0, ostream &outs = cout) const
    {
        if (is_leaf())
        {
            for (int i = data_count; i > 0; i--)
            {
                outs << setw(4 * level) << data[i - 1] << endl;
            }
            return;
        }
        for (int i = child_count - 1; i >= 0; i--)
        {
            if (i < data_count)
                outs << setw(4 * level) << data[i] << endl;
            subset[i]->print_tree(level + 1);
        }
    }

    friend ostream &operator<<(ostream &outs, const BPlusTree<T> &print_me)
    {
        print_me.print_tree(0, outs);
        return outs;
    }

    bool is_valid() { return true; }

    string in_order()
    {
        string str = "";
        if (is_leaf())
        {
            for (int i = 0; i < data_count; i++)
            {
                str += to_string(data[i]) + "|";
                // cout << to_string(data[i]) + "|";
            }
        }
        for (int i = 0; i < child_count; i++)
        {
            str += subset[i]->in_order();
            if (i < data_count)  {
                str += to_string(data[i]) + "|";
                // cout << to_string(data[i]) + "|";
            }     
        }
        return str;
    }

    string post_order()
    {
        string str;
        if (is_leaf())
        {
            for (int i = 0; i < data_count; i++)
            {
                str += to_string(data[i]) + "|";
            }
            return str;
        }
        str += subset[0]->post_order();
        for (int i = 0; i < child_count - 1; i++)
        {
            str += subset[i + 1]->post_order();
            if (i < data_count)
                str += to_string(data[i]) + "|";
        }
        return str;
    }

    string pre_order()
    {
        string str;
        if (is_leaf())
        {
            for (int i = 0; i < data_count; i++)
            {
                str += to_string(data[i]) + "|";
            }
        }
        for (int i = 0; i < child_count; i++)
        {
            if (i < data_count)
                str += to_string(data[i]) + "|";
            str += subset[i]->pre_order();
        }
        return str;
    }

    Iterator begin()
    {
        if (data_count == 0){
            return Iterator(NULL);
        }
        return Iterator(get_smallest_node());
    }
    Iterator end()
    {
        return Iterator(NULL);
    }
    ostream &list_keys(Iterator from = NULL, Iterator to = NULL)
    {
        if (from == NULL)
            from = begin();
        if (to == NULL)
            to = end();
        for (Iterator it = from; it != to; it++)
            cout << *it << " ";
        return cout;
    }

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                   // true if duplicate keys are allowed
    int data_count;                 // number of data elements
    T data[MAXIMUM + 1];            // holds the keys
    int child_count;                // number of children
    BPlusTree *subset[MAXIMUM + 2]; // subtrees
    BPlusTree *next;
    bool is_leaf() const
    {
        return child_count == 0;
    } // true if this is a leaf node

    T *find_ptr(const T &entry){ // return a pointer to this key, NULL if not there.
        int index = first_ge(data, data_count, entry);
        bool found = data[index] == entry && index < data_count;
        if (is_leaf())
        {
            if (found) {
                return &data[index];
            }
            return NULL;
        }
        if (found)
        {
            return subset[index + 1]->find_ptr(entry);
        }
        else
        {
            return subset[index]->find_ptr(entry);
        }
    }

    // insert element functions
    void loose_insert(const T &entry)
    { // allows MAXIMUM+1 data elements in the root
        int index = first_ge(data, data_count, entry);
        bool found = data[index] == entry && index < data_count;
        if (is_leaf())
        {
            if (found)
            {
                // data[index] + entry
                data[index] = entry;
            }
            else
            {
                insert_item(data, index, data_count, entry);
            }
            return;
        }
        if (found)
        {
            subset[index + 1]->loose_insert(entry);
            fix_excess(index + 1);
        }
        else
        {
            subset[index]->loose_insert(entry);
            fix_excess(index);
        }
    }

    void fix_excess(int i)
    { // fix excess in child i
        if (subset[i]->data_count <= MAXIMUM)
            return;
        insert_item(subset, i + 1, child_count, new BPlusTree<T>);
        split(subset[i]->data, subset[i]->data_count, subset[i + 1]->data, subset[i + 1]->data_count);
        split(subset[i]->subset, subset[i]->child_count, subset[i + 1]->subset, subset[i + 1]->child_count);
        T item;
        detach_item(subset[i]->data, subset[i]->data_count, item);
        insert_item(data, i, data_count, item);
        if (subset[i]->is_leaf())
        {
            insert_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, item);
            subset[i + 1]->next = subset[i]->next;
            subset[i]->next = subset[i + 1];
        }
    }

    // remove element functions:
    void loose_remove(const T &entry) { // allows MINIMUM-1 data elements in the root
        int index = first_ge(data,data_count,entry);
        bool found = (data[index]==entry && index < data_count);
        if (is_leaf()){
            if(found){
                T item;
                delete_item(data,index,data_count,item);
            }
            else{
                cout << "not found";
            }
        }
        else{
            if(found){
                subset[index+1]->loose_remove(entry); 
                fix_shortage(index+1);
                BPlusTree* ptr = subset[index+1]->get_smallest_node();
                data[index] = ptr->data[0];
                
            }
            else{
                subset[index]->loose_remove(entry);
                fix_shortage(index);
                if (index-1 > 0){
                    BPlusTree* ptr = subset[index-1]->get_smallest_node();
                    data[index-1] = ptr->data[0];
                }
                
            }
        }
    } 
                                         

    BPlusTree<T> *fix_shortage(int i)
    {
        if (subset[i]->data_count >= MINIMUM) return NULL;
        if (i+1 < child_count && subset[i+1]->data_count > MINIMUM){
            cout << "left\n";
            transfer_left(i);
        }  
        else if (i-1 >= 0 && subset[i-1]->data_count > MINIMUM ){
            cout << "right\n";
            transfer_right(i);
        } 
        else{
            cout << "merge\n";
            merge_with_next_subset(i);
        }
        return get_smallest_node();
    } //fix shortage in child i and return the smallest key in this subtree
    

    BPlusTree<T> *get_smallest_node()
    {
        if (is_leaf())
        {
            return this;
        }
        return subset[0]->get_smallest_node();
    }

    BPlusTree<T> *get_largest_node()
    {
        if (is_leaf())
        {
            return this;
        }
        return subset[child_count - 1]->get_largest_node();
    }

    void get_smallest(T &entry)
    { // entry := leftmost leaf
        entry = get_smallest_node()->data[0];
    }
    void get_biggest(T &entry)
    { // entry := rightmost leaf
        entry = get_largest_node()->data[data_count - 1];
    }
    void remove_biggest(T &entry)
    { // remove the biggest child of tree->entry
        BPlusTree<T> *temp = get_largest_node();
        detach_item(data, data_count, entry);
    }
    void transfer_left(int i)
    { // transfer one element LEFT from child i (at root lvl)
        BPlusTree* subsetItem;
        T item;
        delete_item(subset[i+1]->data,0,subset[i+1]->data_count,item);
        insert_item(subset[i]->data,subset[i]->data_count,subset[i]->data_count,item);
        if (subset[i]->is_leaf()){
            // data[i-1] = subset[i]->get_smallest_node()->data[0];
            data[i] = subset[i+1]->get_smallest_node()->data[0];
        }
        else{
            delete_item(subset[i+1]->subset,0,subset[i+1]->child_count,subsetItem);
            insert_item(subset[i]->subset,subset[i]->child_count,subset[i]->child_count,subsetItem);
        }
    }
    void transfer_right(int i)
    { // transfer one element RIGHT from child i
        BPlusTree* subsetItem;
        T item;
        detach_item(subset[i-1]->data,subset[i-1]->data_count,item);
        insert_item(subset[i]->data,0,subset[i]->data_count,item);
        
        if (subset[i]->is_leaf()){
            data[i-1] = subset[i]->get_smallest_node()->data[0];
        }
        else{
            detach_item(subset[i-1]->subset,subset[i-1]->child_count,subsetItem);
            insert_item(subset[i]->subset,0,subset[i]->child_count,subsetItem);
        }
    }
    void merge_with_next_subset(int i)
    {
        T entry;
        BPlusTree<T>* b;
        cout << "two";
        detach_item(data,data_count,entry);
        cout << "one";
        // delete_item(subset,i,child_count,b);
        // return get_small
    } // merge subset i with i+1

};

#endif
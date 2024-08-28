#ifndef BTREE_ARRAY_FUNCTIONS
#define BTREE_ARRAY_FUNCTIONS

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
using namespace std;

template <class T>
T maximal(const T& a, const T& b)                      //return the larger of the two items
{
      if (a > b) return a;
      return b;
}

template <class T>
void swap(T& a, T& b)  //swap the two items
{
      T temp;
      temp = a;
      a = b;
      b = temp;
}

template <class T>
int index_of_maximal(T data[ ], int n);                 //return index of the largest item in data

template <class T>
void ordered_insert(T data[ ], int& n, T entry);        //insert entry into the sorted array
                                                        //data with length n

template <class T>
int first_ge(const T data[ ], int n, const T& entry)   //return the first element in data that is
                                                        //not less than entry
{
      for (int i = 0; i < n;i++){
            if (entry <= data[i]) return i;
      }
      return n;
      // int i;
      // while (data[i] < entry && i < n){
      //       i++;
      // }
      // return i;
}

template <class T>
void attach_item(T data[ ], int& n, const T& entry)    //append entry to the right of data
{
      n++;
      data[n] = entry;
}

template <class T>
void insert_item(T data[ ], int i, int& n, T entry)    //insert entry at index i in data
{
            for (int j = n-1; j >= i ; j--){
                  data[j+1] = data[j];
            }
            data[i] = entry;
            n++;
      
}

template <class T>
void detach_item(T data[ ], int& n, T& entry)          //remove the last element in data and place
                                                        //it in entry
{
      entry = data[n-1];
      n--;
}

template <class T>
void delete_item(T data[ ], int i, int& n, T& entry )   //delete item at index i and place it in entry
{
      entry = data[i];
      for (int j = i; j < n-1; j++ ){
            data[j] = data[j+1];
      }
      n--;
}

template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2)   //append data2 to the right of data1
{     
      int j = 0;
      for (int i = n1; i < n2+n1; i++){
            data1[i] = data2[j];
            j++;
      }
      n1 = n1+n2;
      n2 = 0;
}

// template <class T>
// void preMerge(T data1[ ], int& n1, T data2[ ], int& n2)   //append data2 to the left of data1
// {     
//       int j = 0;
//       for (int i = n1; i < n2+n1; i++){
//             data1[i] = data2[j];
//             j++;
//       }
//       n1 = n1+n2;
//       n2 = 0;
// }

template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2)   //move n/2 elements from the right of data1
                                                        //and move to data2
{
      int index = ceil(n1/2.0);
      int j = 0;
      for (int i = index; i < n1;i++){
            data2[j] = data1[i];
            j++;
      }
      n1 = index;
      n2 = j;
}

template <class T>
void copy_array(T dest[], const T src[],
                int& dest_size, int src_size)              //copy src[] into dest[]
{   
      for (int i = 0; i < src_size; i++){
            dest[i] = src[i];
      }
      dest_size = src_size;
}

template <class T>
void print_array(const T data[], int n, int pos = -1)  //print array data
{
      for (int i = 0; i < n; i++){
            cout << data[i] << endl;
      }
}

template <class T>
bool is_gt(const T data[], int n, const T& item);       //item > all data[i]

template <class T>
bool is_le(const T data[], int n, const T& item);       //item <= all data[i]

//-------------- Vector Extra operators: ---------------------

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list) //print vector list
{
      outs << "[ ";
      for (int i = 0; i < list.size(); i++){
            outs << list[i] << " ";
      }
      outs << "]";
      return outs;
}

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme) //list.push_back addme
{
      list.push_back(addme);
      return list;
}

template <typename T>
vector<T>& operator +(vector<T>& list, vector<T>& list2) //list.push_back addme
{
      for (int i = 0; i < list2.size(); i++){
            list.push_back(list2[i]);
      }
      return list;
}

// bool operator <=(const string& i, const string& o){
//       if (i.compare(o) == 1 || i.compare(o) < 0 ){
//             return true;
//       }
//       return false;
// }

// bool operator <(const string& i, const string& o){
//       if (i.compare(o) < 0 ){
//             return true;
//       }
//       return false;
// }
#endif
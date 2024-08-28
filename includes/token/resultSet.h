#ifndef ResultSet_H
#define ResultSet_H
#include <vector>

#include "token.h"
#include "../../includes/bplustree/bplustree.h"
#include <algorithm> 


class ResultSet : public Token{
private:
    vector<long> vector_recnos;
public:
    ResultSet(){
        Token();
    }
    ResultSet(vector<long> recnos){
        vector_recnos = recnos;
        setString("SelectRecnos");
        setType(7);
    }
    ResultSet* and_with(vector<long> recnos){
        vector<long> commonItems;
        BPlusTree<long> bpt;
        for (int i = 0; i < recnos.size(); i++){
            bpt.insert(recnos[i]);
        }
        for (int i = 0; i < vector_recnos.size(); i++){
            if (bpt.contains(vector_recnos[i])){
                commonItems.push_back(vector_recnos[i]);
            }
        }
        ResultSet temp(commonItems);
        vector_recnos = commonItems;
        return this;
       
    }
    ResultSet* or_with(vector<long> recnos){
        BPlusTree<long> bpt;
        for (int i = 0; i < vector_recnos.size(); i++){
            bpt.insert(vector_recnos[i]);
        }
        for (int i = 0 ;i < recnos.size(); i++){
            if (!bpt.contains(recnos[i])){
                vector_recnos.push_back(recnos[i]);
            }
        }
        // cout << vector_recnos << endl << recnos;
        // sort(vector_recnos.begin(),vector_recnos.end());
        
        return this;
    }

    vector<long> getRecnos(){
        return vector_recnos;

    }
};

#endif
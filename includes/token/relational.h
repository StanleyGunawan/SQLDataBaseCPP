#ifndef RELATIONAL_H
#define RELATIONAL_H

#include "resultSet.h"
class Relational : public Token{
    private:
        ResultSet set;
    public:
    Relational(){
        Token();
    }
    Relational(string str){
        setString(str);
        setType(5);
    }
    ResultSet* eval(MMap<string,long> indices, Token* left, Token* right){
        vector<long> selectRecnos;
        // cout << getString();
        if (getString() == "="){
            selectRecnos = indices.get(right->getString());
        }
        else if (getString() == "<"){
            selectRecnos = indices.lessThan(right->getString());
        }
        else if (getString() == ">"){
            selectRecnos = indices.greaterThan(right->getString());
        }
        else if (getString() == ">="){
            selectRecnos = indices.greaterEqualThan(right->getString());
        }
        else if (getString() == "<="){
            selectRecnos = indices.lessEqualThan(right->getString());
        }
        ResultSet temp(selectRecnos);
        set = temp;
        // cout << set.getRecnos();
        return  &set;
    }
};

#endif
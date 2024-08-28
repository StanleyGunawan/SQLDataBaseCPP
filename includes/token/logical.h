#ifndef LOGICAL_H
#define LOGICAL_H


#include "resultSet.h"
class Logical : public Token{
    private:
  
    public:
    Logical(){
        Token();
    }
    Logical(string str, int i){
        setString(str);
        setType(i);
    }
    ResultSet* eval(MMap<string,long> indices, Token* left, Token* right){
        if (getString() == "and"){
            return dynamic_cast<ResultSet*>(left)->and_with(right->getRecnos());
        }
        return dynamic_cast<ResultSet*>(left)->or_with(right->getRecnos());
    }

    
};

#endif
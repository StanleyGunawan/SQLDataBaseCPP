#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>
using namespace std;

#include "../../includes/bplustree/multimap.h"

class Token
{
public:
    Token();
    Token(string str, int type);
    friend ostream& operator <<(ostream& outs, const Token& t);
    int type() const;
    string type_string() const;
    string token_str() const;
    void setString(string token);
    void setType(int type);
    string getString();
    friend bool operator >=(const Token &lhs, const Token &rhs);
    virtual Token* eval(MMap<string,long> indices, Token* left, Token* right){
        assert(type() != 2);
        return this;
    }
    virtual Token* and_with(vector<long> recnos){
        assert(type() == 4);
        return this;
    }
    virtual Token* or_with(vector<long> recnos){
        assert(type() == 4);
        return this;
    }
    virtual vector<long> getRecnos(){
        cout << "getRecnos fail -> in token.h";
        vector<long> temp;
        return temp;
    }
private:
    string _token;
    int _type;
    
};

// #include "token.cpp"

#endif // TOKEN_H
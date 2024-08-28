#ifndef TOKENSTR_H
#define TOKENSTR_H


#include "token.h"
class TokenStr : public Token{
    private:

    public:
    TokenStr(){
        Token();
    }
    TokenStr(string str){
        setString(str);
        setType(2);
    }
};

#endif
// LParen.h
#ifndef LPAREN_H
#define LPAREN_H

#include "token.h"

class LParen : public Token {
public:
    std::string getType() const  {
        return "LParen";
    }

    std::string getTokenStr() const  {
        return "(";
    }

    LParen(string str){
        setString(str);
        setType(1);
    }
};

#endif // LPAREN_H

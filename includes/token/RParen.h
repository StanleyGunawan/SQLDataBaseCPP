// RParen.h
#ifndef RPAREN_H
#define RPAREN_H

#include "token.h"

class RParen : public Token {
public:
    std::string getType() const  {
        return "RParen";
    }

    std::string getTokenStr() const  {
        return ")";
    }

    RParen(string str){
        setString(str);
        setType(1);
    }
};

#endif // RPAREN_H

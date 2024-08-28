#ifndef TOKEN_CPP
#define TOKEN_CPP
#include "token.h"

Token::Token(){
    _token = "NULL";
    _type = 0;
}


Token::Token(string str, int type){
    _token = str;
    _type = type;
}
ostream& operator <<(ostream& outs, const Token& t){
    // outs << t.type_string() << "     |" << t.token_str() << "|\n";
     outs << "     |" << t.token_str() << "|";
    return outs;
}
int Token::type() const{        //get type
    return _type;
}

string Token::getString(){  //get string
    return _token;
}

string Token::type_string() const{  //get type in string
    string type_string;
    switch (_type){
        case 1:
            type_string = "NUMBER";
            break;
        case 2:
            type_string = "ALPHA";
            break;
        case 3:
            type_string = "SPACE";
            break;
        case 4:
            type_string = "OPERATOR";
            break;
        case 5:
            type_string = "PUNC";
            break;
        default:
            type_string = "UNKNOWN";
            break;
    }
    return type_string;
}

string Token::token_str() const{
    return _token;
}

bool operator >=(const Token &lhs, const Token &rhs){
    if (lhs.type() >= rhs.type()) return true;
    return false;
}

void Token::setString(string token){
    _token = token;
}
void Token::setType(int type){
    _type = type;
}

#endif
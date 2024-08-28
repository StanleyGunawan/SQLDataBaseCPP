#ifndef PARSER_H
#define PARSER_H

#include "../bplustree/multimap.h"
#include "../tokenizer/stokenize.h"
#include "../bplustree/map.h"
using namespace std;

typedef MMap<string,string> mmap_ss;

class Parser{
private:
    vector<string> strings;
    enum myEnums {ZERO, SELECT, INSERT, INTO, CREATE, MAKE, FROM, WHERE, SYMBOL, STAR, VALUES, TABLE, FIELDS, LPAREN, RPAREN, COMA, OPERATOR, ANDOR};
    Map<string,myEnums> map;
    int _table[MAX_ROWS][MAX_COLUMNS];
    bool success = 0;

    void charToVector(char* c);
    void build_keyword_list();      //create map string to enum
    void makeTable();               //create adjency matrix.
    void addToMmap(int currentState, int previousState, string str, MMap<string,string>& mmap);
public:
    Parser();
    Parser(char* c);

    void set_string(char* c);
    MMap<string,string> parse_tree();
    bool get_parse_tree();

};

// #include "parser.cpp"
#endif
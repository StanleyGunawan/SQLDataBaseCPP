#ifndef PARSER_CPP
#define PARSER_CPP
#include "parser.h"

Parser::Parser()        //added
{
    makeTable();
    build_keyword_list();
}
Parser::Parser(char *c)
{
    charToVector(c);
    makeTable();
    build_keyword_list();
}

void Parser::set_string(char *c)
{
    charToVector(c);
    // for (int i = 0; i < strings.size();i++){
    //     cout << strings[i] << endl;
    // } 
}

bool Parser::get_parse_tree(){
    MMap<string,string> mmap = parse_tree();    //added
    return success;
}

MMap<string, string> Parser::parse_tree()
{
    MMap<string,string> mmap;
    myEnums enums;
    int currentState = 0;
    int previousState = 0;
    for (int index = 0; index < strings.size(); index++){
        if (map.contains(strings[index])){
            enums = map.get(strings[index]);
        }
        else{
            enums = SYMBOL;
        }
        currentState = _table[currentState][enums];
        if (currentState == -1){            //exit if state -1
            break;
        }
        addToMmap(currentState, previousState, strings[index], mmap);
        previousState = currentState;
    }
    if (_table[currentState][0] != 1){
        success = 0;
        mmap.clear();
        if (previousState == 3 && enums == WHERE) throw "expected table name";
        else if (previousState == 5 && enums == WHERE) throw "expected conditions ";
        else if (previousState < 4 && previousState > 1 && enums == SYMBOL) throw "expected coma or from";
        else if (enums == ANDOR || enums == OPERATOR) throw "expected search field value";
        else if (enums == SYMBOL && previousState == 8 )throw "expected AND or OR";
        else if (enums == SYMBOL && previousState == 6 )throw "expected operator";
        else if (enums == COMA) throw "expected field value";
        
        throw "incomplete query";
    }
    else{
        success = 1;
    }
    // cout << mmap;
    return mmap;
}

void Parser::addToMmap(int currentState, int previousState, string str, MMap<string,string>& mmap){
    string s;
    // cout << currentState << " : " << str << endl;
    switch (currentState){
        default:
            return;
        case 1: 
            mmap["command"] += str;
            break;
        case 2:
            mmap["fields"] += str;
             break;
        case 4:
            mmap["table_name"] += str;
             break;
        case 5:
            if (str == "where"){
                s = "yes";
                mmap["where"] += s;
            } 
            else{
                s = "no";
                mmap["where"] += s;
            }
            break;
        case 6:
            mmap["condition"] += str;
             break;
        case 7:
            mmap["condition"] += str;
             break;
        case 8:
            mmap["condition"] += str;
            break;
        case 9:
            mmap["condition"] += str;
            break;
        case 10:
            mmap["fields"] += str;
            break;

        case 20:
            mmap["command"] += str;
            break;
        case 22:
            mmap["table_name"] += str;
            break;
        case 24:
            mmap["values"] += str;
            break;

        case 40:
            mmap["command"] += str;
            break;
        case 42:
            mmap["table_name"] += str;
            break;
        case 44:
            mmap["col"] += str;
            break;
    }   
}

void Parser::charToVector(char *c)
{
    strings.clear();
    STokenizer stk;
    stk.set_string(c);
    Token t;
    //The all too familiar golden while loop:
    stk>>t;
    while(!stk.done()){
        if (t.getString() != ""){
            strings.push_back(t.getString());
        }
        t = Token();
        stk>>t;
    }
}

void Parser::build_keyword_list(){
    map.insert("select", SELECT);
    map.insert("from", FROM);
    map.insert("where", WHERE);
    map.insert("*", STAR);
    map.insert("insert", INSERT);
    map.insert("into", INTO);
    map.insert("values", VALUES);
    map.insert("create", CREATE);
    map.insert("make", MAKE);
    map.insert("table", TABLE);
    map.insert("fields", FIELDS);
    map.insert(",", COMA);
    map.insert("=", OPERATOR);
    map.insert(">=", OPERATOR);
    map.insert("<=", OPERATOR);
    map.insert("<", OPERATOR);
    map.insert(">", OPERATOR);
    map.insert("and", ANDOR);
    map.insert("or", ANDOR);
    map.insert("(", LPAREN);
    map.insert(")", RPAREN);
}

void Parser::makeTable(){
    init_table(_table);
    myEnums word;
    for (int i = 0; i < 30; i++){
        mark_fail(_table, i);
        
    }
    mark_fail(_table, -1);
    //select table
    mark_success(_table,8);
    mark_success(_table,4);
    mark_cell(0,_table,SELECT, 1);
    mark_cell(1,_table,SYMBOL,2);
    mark_cell(2,_table,COMA,1);
    // mark_cell(2,_table,SYMBOL,2);
    mark_cell(2,_table,FROM,3);
    mark_cell(3,_table,SYMBOL,4);
    mark_cell(4,_table,WHERE,5);

    mark_cell(5,_table,LPAREN, 9);
    mark_cell(5,_table,SYMBOL,6);
    mark_cell(6,_table,OPERATOR,7);
    mark_cell(7,_table,SYMBOL,8);
    mark_cell(8,_table,RPAREN,8);

    mark_cell(8,_table,ANDOR,9);
    mark_cell(9,_table,SYMBOL,6);
    mark_cell(9,_table,LPAREN, 9);


    mark_cell(1,_table,STAR,10);
    mark_cell(10,_table,FROM,3);

    //insert into table
    mark_success(_table,24);
    mark_cell(0,_table,INSERT,20);
    mark_cell(20,_table,INTO,21);
    mark_cell(21,_table,SYMBOL,22);
    mark_cell(22,_table,VALUES,23);
    mark_cell(23,_table,SYMBOL,24);
    mark_cell(24,_table,COMA,25);

    mark_cell(25,_table,SYMBOL,24);

    //create/make table;
    mark_success(_table,44);
    mark_cell(0,_table,CREATE,40);
    mark_cell(0,_table,MAKE,40);
    mark_cell(40,_table,TABLE,41);
    mark_cell(41,_table,SYMBOL,42);
    mark_cell(42,_table,FIELDS,43);
    mark_cell(43,_table,SYMBOL,44);
    mark_cell(44,_table,COMA,45);
    mark_cell(45,_table,SYMBOL,44);
}

#endif
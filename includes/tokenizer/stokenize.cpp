#ifndef STOKENIZE_CPP
#define STOKENIZE_CPP
#include "stokenize.h"

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

STokenizer::STokenizer(){
    make_table(_table); 
    _pos = 0;
    current_state = 0;
}
// STokenizer::~STokenizer(){

// }
STokenizer::STokenizer(char str[]){
    make_table(_table);
    strcpy(_buffer,str);
    _pos = 0;
    current_state = 0;
}

void STokenizer::set_string(char str[]){
    strcpy(_buffer,str);
    _pos = 0;
}

STokenizer& operator >> (STokenizer& s, Token& t){
    string token_string = s._buffer;
    int _current_state = 0;
    int type = 0;
    if (s.get_token(_current_state,token_string)){
        if (_current_state == 1){
            type = 2;
        }
        else if (_current_state == 4 || _current_state == 6){
            type = 1;
        }
        else if (_current_state == 9){
            type = 5;
        }
        else if (_current_state == 12){
            type = 4;
        }
        else if (_current_state == 15){
            type = 3;
        }
    
        t = Token(token_string, type);
        // show_string(s._buffer,s._pos);       //test
    }
    else{
        // cout << "\n" << "Something" << "\n";        //test
        t = Token("",3);
        // cout<<setw(10)<<t.type_string()<<setw(10)<<t<<endl;
    }
    return s;
}

void STokenizer::make_table(int _table[][MAX_COLUMNS]){
    init_table(_table);
  
    for (int i = 0; i < 30; i++){
        mark_fail(_table, i);
        
    }
    // mark_fail(_table, -1);
    //ALFA TABLE
    // mark_success(_table,1);
    // mark_cells(0, _table, ALFA, 1);
    // mark_cells(1, _table, ALFA, 1);

    //parser table
    mark_success(_table,1);
    mark_cells(0, _table, ALFA, 1);
    mark_cells(0, _table, "_", 1);
    mark_cells(1, _table, "_", 1);
    mark_cells(1, _table, DIGITS, 1);
    mark_cells(0, _table, DIGITS, 1);
    mark_cells(1, _table, ALFA, 1);

    //Number Table
    mark_success(_table,4);
    mark_fail(_table,5);
    mark_success(_table,6);
    mark_cells(0,_table, DIGITS, 4);
    mark_cells(4,_table, DIGITS, 4);
    mark_cell(4, _table,'.', 5);
    mark_cells(5,_table, DIGITS, 6);
    mark_cells(6,_table,DIGITS,6);
    
    //Operator Table
    mark_cells(0,_table,OPERATORS, 12);
    mark_success(_table,12);
    mark_cells(12,_table,OPERATORS,12);

    // PUNC Table
    mark_cells(0,_table,PUNC, 13);
    mark_success(_table,13);
   

    // Punctuation Table
    // mark_cells(0,_table,PUNC, 9);
    // mark_success(_table,9);
    // mark_cells(9,_table,PUNC,9);

    // " " Table
    mark_success(_table,21);
    mark_cells(0,_table,"\"", 20);
    mark_cells(20, _table, SPACES, 20);
    mark_cells(20, _table, ALFA, 20);
    mark_cells(20, _table, PUNC, 20);
    mark_cells(20, _table, OPERATORS, 20);
    mark_cells(20, _table, DIGITS, 20);
    mark_cells(20,_table,"\"",21);

    //SPACE TABLE
    // mark_cells(0,_table,SPACES, 15);
    // mark_success(_table,15);
    // mark_cells(15,_table,SPACES,15);
}


bool STokenizer::get_token(int& start_state, string& token){
    if(strlen(_buffer)< 1){
        strcpy(_buffer,"n");
        _pos++;
        return false;
    }
    int currentState = start_state;
    int token_length = 0;
    int last_succes_state = 0;
    int start_position = _pos;
    int i = _pos; 
    // cout << currentState;
    while (currentState != -1 && i < token.length()){                     
        currentState = _table[currentState][token[i]];
        if (_table[currentState][0] == 1){
            token_length = i -_pos + 1;
            last_succes_state = currentState;
            // cout << currentState;
        }
        i++;
    }
    start_state = last_succes_state;
    //  show_string(_buffer,_pos); 
    if(token_length > 0){          
        // current_state = last_succes_state;
        _pos+=token_length;
        if (last_succes_state == 21 ){
            start_position++;
            token_length = token_length-2;
        }
        token = token.substr(start_position,token_length);
        return true;
    }
    else{
        _pos++;
        return false;
    }
}

bool STokenizer::done(){
    return _pos > strlen(_buffer);
}

bool STokenizer::more(){
    return !done();
}

#endif
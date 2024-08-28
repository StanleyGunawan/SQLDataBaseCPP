#ifndef TOKENIZER_FTOKENIZE_CPP
#define TOKENIZER_FTOKENIZE_CPP
#include <cassert>
#include "ftokenize.h"

    FTokenizer::FTokenizer(char* fname){
        // string fileName = fname  ;
        // fileName = "../../includes/" + fileName ;
        // strcpy(fileName, );
        _f.open("C:/Users/stanl/Documents/PCC/CS008/st_05_file_tokenizer-StanleyGunawan/includes/solitude_mini.txt");
        assert(!_f.fail() && "File opening  failed");
        _more = true;
    }

    Token FTokenizer::next_token(){
    Token t;
    _stk >> t;
    return t;
    }

    FTokenizer& operator >> (FTokenizer& f, Token& t){
        if(f._stk.done()){
            f.get_new_block();
        }
        f._stk >> t; 

        return f;
    }

    bool FTokenizer::get_new_block(){
        if(!_f.read(_block,MAX_BLOCK)){
            _more = false;
        }
        // _f.read(_block,MAX_BLOCK);
        _block[_f.gcount()] = '\0';
        _pos = _pos + _f.gcount();
        _stk.set_string(_block);
        // cout << _block;
    
        return true;

    } //gets the new block from the file

    bool FTokenizer::more(){
        if (!_more && _stk.done()){
            return false;
        }
        return true;
    }        //returns the current value of _more

    int FTokenizer::pos(){
    return _pos;
    }          //returns the value of _pos

    int FTokenizer::block_pos(){
        return _blockPos;
    }    //returns the value of _blockPos


#endif

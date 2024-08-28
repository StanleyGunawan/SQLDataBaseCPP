#ifndef TABLE_TABLE_H
#define TABLE_TABLE_H

#include "typedefs.h"
#include "../binary_files/file_record.h"
#include "../bplustree/multimap.h"
#include "../bplustree/map.h"
// #include "../tokenizer/logical.h"
// #include "../tokenizer/tokenstr.h"
// #include "../tokenizer/relational.h"
// #include "../tokenizer/RParen.h"
// #include "../tokenizer/LParen.h"
#include "../token/child.h"
#include "../../includes/queue/MyQueue.h"
#include "../../includes/stack/MyStack.h"

using namespace std;


class Table {
public:
    Table();
    Table(string name);
    Table(string s, vectorstr field);

    void insert_into(vectorstr input);
    vector<long> select_recnos();
    Table select_all();
    
    Table select(vectorstr fields,string field, string op, string target);
    Table select(vectorstr fields, Queue<Token*>& queue);
    Table select(vectorstr fields, vectorstr condition);
    Table select(vectorstr fields);
    void setField(vectorstr fields);
    void closeFile();
    long getRecno(){
        return recno;
    }
    void setRecno(){
        recno = 0;
    }
    void clearIndice(){
        indices.clear();
    }

    void reset(vectorstr field);
    
    friend ostream& operator <<(ostream& outs, const Table& print_me);
private:
    string tableName;
    long recno = 0;
    FileRecord _fileRecord;
    vectorstr fieldVector;
    static int fileNumber;
    vector<MMap<string,long>> indices;
    Map<string,int> fieldMap;
    vector<long> selectRecnos;
    
    void readTxt();
    void writeTxt();
    void reIndex();
    Table vector_to_table(vector<long> ints, string name,vectorstr field = {"*"});
    Queue<Token*> RPN(Queue<Token*>& queue);
    Queue<Token*> strToQ(vectorstr strings);
};

void Table::reset(vectorstr field){
    fieldMap.clear();
    indices.clear();
    recno = 0;
    selectRecnos.clear();
    MMap<string,long> temp;
    
    for (int i = 0; i < field.size();i++){
        fieldMap[field[i]] = i;
        indices.push_back(temp);
    }
    fieldVector = field;
    fstream file;
    open_fileW(file,tableName.c_str());            //reset the file
    file.close();
    writeTxt();     //saves the field
}

int Table::fileNumber = 0;

Table::Table()
{
    tableName = "new_table";
}

Table::Table(string name){
    tableName = name;
    readTxt();
    MMap<string,long> temp;
    for (int i = 0; i < fieldVector.size();i++){
        fieldMap[fieldVector[i]] = i;
        indices.push_back(temp);
    }
    reIndex();
}

Table::Table(string name, vectorstr field){     
    MMap<string,long> temp;
    for (int i = 0; i < field.size();i++){
        fieldMap[field[i]] = i;
        indices.push_back(temp);
    }
    fieldVector = field;
    tableName = name;
    fstream file;
    open_fileW(file,tableName.c_str());            //reset the file
    file.close();
    writeTxt();     //saves the field

}


void Table::insert_into(vectorstr input){ 
    fstream file;   
    open_fileRW(file,tableName.c_str());            //usees RW so doesnt reset the file
    _fileRecord = FileRecord(input);
    _fileRecord.write(file);
    file.close();
    
    assert(indices.size() >= input.size() && "MMAP BUILT INCORRECTLY");
    
    for (int i = 0; i < input.size(); i++){         //updates the mmap bplustree (indices)
        indices[i][input[i]] += recno;
    }
    recno++;
}

Table Table::select(vectorstr fields, string field, string op, string target){
    //op may be >/< in which case use the upper/lower bounds from bplustree
    int i = fieldMap.get(field);
    if (op == "="){
        selectRecnos = indices[i].get(target);
    }
    else if (op == "<"){
        selectRecnos = indices[i].lessThan(target);
        // cout << selectRecnos;
    }
    else if (op == ">"){
        selectRecnos = indices[i].greaterThan(target);
        // cout << selectRecnos;
    }
    else if (op == ">="){
        selectRecnos = indices[i].greaterEqualThan(target);
        // cout << selectRecnos;
    }
    else if (op == "<="){
        selectRecnos = indices[i].lessEqualThan(target);
        // cout << selectRecnos;
    }
    return vector_to_table(selectRecnos,"select_table_", fields);
}


Table Table::select(vectorstr fields, Queue<Token*>& queue){
    Queue<Token*> temp = queue;
    Stack<Token*> stack;
    Token* token;
    Token* token1;
    Token* token2;
    int i;
    
    while (!temp.empty()){
        token = temp.pop();
        if (token->type() == 2){
            stack.push(token);
        }
        else if (token->type() != 2){
            if (stack.size() < 2) throw "Popping empty stack";
            token2 = stack.pop();
            token1 = stack.pop();
            i = fieldMap.get(token1->getString());
            // assert((token ->type() == 5 && token1->type() == 2 && token2->type() == 2) || (token->type() == 4 && token1->type() == 7 && token2->type() == 7) && "RPN ERROR");
            stack.push(token->eval(indices[i],token1,token2));
            // cout << token->eval(indices[i],token1,token2)->getRecnos();;
        }
    }
    token = stack.pop();
    selectRecnos = token->getRecnos();

    return vector_to_table(selectRecnos,"select_table_",fields);
}

Table Table::select(vectorstr fields, vectorstr condition){
    Queue<Token*> infix = strToQ(condition);
    Queue<Token*> postfix = RPN(infix);
    return select(fields,postfix);
}

Table Table::select(vectorstr fields){
    selectRecnos.clear();
    if (fields[0] == "*"){
        return select_all();
    }
    for (int i = 0; i < recno ;i++){
        selectRecnos.push_back(i);
    }
    return vector_to_table(selectRecnos,tableName,fields);
}

Table Table::select_all(){
   

    selectRecnos.clear();
    for (int i = 0; i < recno ;i++){
        selectRecnos.push_back(i);
    }
    // assert(1 == 0 &&  "here");
    // Table temp;
    // return temp;
    
    return vector_to_table(selectRecnos,tableName);
}

Table Table::vector_to_table(vector<long> ints,string name, vectorstr fields){
    // assert(ints.size() > 0);
    vectorstr local_fields;
    vectorstr output;
    vectorstr input;
    fstream f;
    if (fields[0] != "*"){
        local_fields = fields;
    }
    else{
        local_fields = fieldVector;
    }
    open_fileRW(f,tableName.c_str());
    Table temp(name + to_string(++fileNumber),local_fields);
    for (int i = 0; i < ints.size();i++){
        _fileRecord.read(f, ints[i]);
        output = _fileRecord.getRecord();
        input.clear();
        for (int i = 0; i < local_fields.size();i++){       //getting only the selected field
            int j = fieldMap.get(local_fields[i]);          
            input.push_back(output[j]);
        }
        temp.insert_into(input);
    }
    
    f.close();
    return temp;
}

vector<long> Table::select_recnos(){
    return selectRecnos;
}

void Table::writeTxt(){
    ofstream inFile;
    string name = tableName + "field.txt";
    // inFile.open("fieldFile.txt", ios::out);
    inFile.open(name, ios::out);
    for (int i = 0 ; i < fieldVector.size();i++){
        inFile << fieldVector[i] << '\n'; 
    }
    inFile.close();
}

void Table::readTxt(){
    ifstream outFile;
    string name = tableName + "field.txt";
    string t;
    outFile.open(name);
    fieldVector.clear();
    while(outFile){
        outFile >> t;
        fieldVector.push_back(t);
    }
    fieldVector.pop_back();
    outFile.close();
}

void Table::setField(vectorstr fields){
    indices.clear();
    MMap<string,long> temp;
    for (int i = 0; i < fields.size();i++){
        fieldMap[fields[i]] += i;
        indices.push_back(temp);
    }
    fieldVector = fields;
}

void Table::reIndex(){
    fstream f;
    open_fileRW(f,tableName.c_str());
    int i = 0;
    int byte = _fileRecord.read(f, i);
    vectorstr input;
    while(byte > 0){
        input = _fileRecord.getRecord();
        for (int i = 0; i < input.size(); i++){
            indices[i][input[i]] += recno;
        }
        recno++;   
        i++;
        byte = _fileRecord.read(f, i);
    }
    f.close();
}

Queue<Token*> Table::RPN(Queue<Token*>& queue){
    Token* token;
    Stack<Token*> stack;
    Queue<Token*> queue2;
    int ParenCount = 0;
    
    while (!queue.empty()){
        token = queue.pop();
        if (token->type() != 2){
            if (token ->token_str() == "(" ) {
                ParenCount++;
                stack.push(token);
            }
            else if (token->getString() == ")"){
                if (ParenCount < 1) throw "missing left parenthesis ";
                assert(ParenCount > 0);
                ParenCount--;
                token = stack.pop();
                while (token->getString() != "("){
                    queue2.push(token);
                    token = stack.pop();
                }
            }
            
            else if (!stack.empty() && (stack.top()->type() >= token->type())){
                while (!stack.empty() && (stack.top()->type() >= token->type())){
                    queue2.push(stack.pop()); 
                }
                stack.push(token);
            }
            
            else{
                stack.push(token);
            } 
        }
        else if (token->type() == 2){
            queue2.push(token);
        }
    }
    while (!stack.empty()){
        queue2.push(stack.pop());
    }
    // while (!queue2.empty()){
    //     cout << *queue2.pop() << endl;
    // }
    if (ParenCount > 0) throw "missing right parenthesis ";
    if (ParenCount < 0) throw "missing left parenthesis ";
    return queue2;
    
}

ostream& operator <<(ostream& outs, const Table& print_me){
        outs << "\nTable name: " << print_me.tableName << ", records: " << print_me.recno << endl;
        outs << setw(9) << " " << "record";
        for (int i = 0; i < print_me.fieldVector.size();i++){
            int width = strlen(print_me.fieldVector[i].c_str()) + 1;
            outs << setw(16 - width) << " " << print_me.fieldVector[i];
        }
        outs << endl << endl;
    
        fstream f;
        open_fileRW(f,print_me.tableName.c_str());
        int i =0;
        FileRecord record = print_me._fileRecord;
        long bytes = record.read(f, i); 
        while (bytes>0){
            cout<< setw(15) <<i << record <<endl;
            i++;
            bytes = record.read(f, i);
        }
        f.close();
       
        // print_me.file.close();
        return outs;
}


Queue<Token*> Table::strToQ(vectorstr strings){
    Queue<Token*> q;
    
    for (int i = 0; i < strings.size(); i++){
        if (strings[i] == "and"){
            q.push(new Logical("and",4));
        }
        else if (strings[i] == "or"){
            q.push(new Logical("or",3));
        }
        else if (strings[i] == "="){
            q.push(new Relational("="));
        }
        else if (strings[i] == ">="){
            q.push(new Relational(">="));
        }
        else if (strings[i] == "<="){
            q.push(new Relational("<="));
        }
        else if (strings[i] == ">"){
            q.push(new Relational(">"));
        }
        else if (strings[i] == "<"){
            q.push(new Relational("<"));
        }
        else if (strings[i] == "("){
            q.push(new LParen("("));
        }
        else if (strings[i] == ")"){
            q.push(new RParen(")"));
        }
        else{
            q.push(new TokenStr(strings[i]));
        }
    }
    return q;
}

#endif


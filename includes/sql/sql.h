#ifndef SQL_H
#define SQL_H
#include <cassert>

#include "../table/table.h"
#include "../parser/parser.h"

class SQL
{
private:
    Map<string, Table*> tableMap;
    Parser parser;
    vector<long> selectRecnos;
    vector<string> nameVector;

    Table makeTable(vectorstr field, string name);
    Table insertTable(vectorstr values, Table* table);
    Table selectTable(vectorstr fields, vectorstr conditions, Table* table);
    Table selectTable(vectorstr fields, Table* table);
    Table resetTable(string name, vectorstr field, Table* table);
    void storeTableName();
    void setupSQL();
    

public:
    SQL();
    Table command(string input);
    vector<long> select_recnos();
    void takeFromConsole();
    void batch();
};

SQL::SQL()
{
    setupSQL();
    cout << "SQL: Currently tracking these tables:\n";
    cout << "-------------------------------------\n";
    cout << nameVector << endl;
}

Table SQL::command(string input)
{
    Table t;
    try{
        char s[300];
        strcpy(s, input.c_str());
        parser.set_string(s);
        mmap_ss tree = parser.parse_tree();
        

        if (tree.get("command")[0] == "make")
        {
            if(tableMap.contains(tree.get("table_name")[0])){
                return resetTable(tree.get("table_name")[0],tree.get("col"),tableMap.get(tree.get("table_name")[0]));
            }   
            else{
                t =  makeTable(tree.get("col"), tree.get("table_name")[0]);
            }
        }
        else if (tree.get("command")[0] == "insert")
        {
            if(!tableMap.contains(tree.get("table_name")[0])){
                throw "Table doesn't exist";
            }      
            Table* temp = tableMap.get(tree.get("table_name")[0]);
            t = insertTable(tree.get("values"), temp);
        }
        else if (tree.get("command")[0] == "select")
        {
            
            if(!tableMap.contains(tree.get("table_name")[0])){
                throw "Table doesn't exist";
            }                                                   
            Table* temp = tableMap.get(tree.get("table_name")[0]);
            if (tree.contains("where")){
                t = selectTable(tree.get("fields"), tree.get("condition"), temp);
            }
            else{
                t = selectTable(tree.get("fields"), temp);
            }
        }
    }
    catch(const char* text){
        cout << "\nException Thrown: " << text;
    }

    
    return t;
}

vector<long> SQL::select_recnos()
{
    return selectRecnos;
}

Table SQL::resetTable(string name, vectorstr field, Table* table){
    // table->setRecno();
    // table->clearIndice();
    // Table* temp = new Table(name,field);
    // table = temp;
    // return *table;
    table->reset(field);
    return *table;
}

Table SQL::makeTable(vectorstr field, string name)
{
    nameVector.push_back(name);
    storeTableName();
    Table* table = new Table(name, field);
    tableMap.insert(name, table);
    return *table;
}

Table SQL::insertTable(vectorstr values, Table* table)
{
    table->insert_into(values);
    return *table;
}

Table SQL::selectTable(vectorstr fields, vectorstr conditions, Table* table)
{
    Table temp  = table->select(fields, conditions);
    selectRecnos = table->select_recnos();
    return temp;
}

Table SQL::selectTable(vectorstr fields, Table* table)
{
    Table temp  = table->select(fields);
    selectRecnos = table->select_recnos();
    return temp;
}

void SQL::storeTableName(){
    ofstream inFile;
    inFile.open("TableNames.txt");
    for (int i = 0 ; i < nameVector.size();i++){
        inFile << nameVector[i] << '\n'; 
    }
    inFile.close();
}

void SQL::setupSQL(){
    
    ifstream outFile;
    string names;
    outFile.open("TableNames.txt");
    nameVector.clear();
    
    while(outFile >> names){            //get the tracked tables
        nameVector.push_back(names);
    }
    outFile.close();
    vector<string> fields;
    string field;
    
    for (int i = 0 ; i < nameVector.size();i++){            //create table
        Table* table = new Table(nameVector[i]);
        tableMap.insert(nameVector[i], table);
    }
}

void SQL::takeFromConsole(){
    string input;
    cout << "Input Something: \n \"END\" to end the program\n";
    do {
        getline(cin,input);
        cout << command(input);
        cout << "input another command or \"END\" to end the program\n";
    }while (input != "END");
    
}

void SQL::batch(){
    vector<string> commands = {
    /*1.valid command:*/ "select lname, fname, major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
    /*2. Expected comma:*/ "select lname fname, major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
    /*3. Expected: field name */ "select lname, , major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
    /*4. Expected from:*/ "select lname, fname, major  student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
    /*5. Expected table name:*/ "select lname, fname, major from  where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
    /*6. Expected condition:*/ "select lname, fname, major from student where",
    /*7. Missing left paren:*/ "select lname, fname, major from student where (lname=Yang or major=CS) and age<23 )or lname=Jackson",
    /*8. Missing right paren:*/ "select lname, fname, major from student where ((lname=Yang or major=CS and age<23 )or lname=Jackson",
    /*9. :*/ "select lname, fname, major from student where ((lname= or major=CS) and age<23 )or lname=Jackson",
    /*10. :*/ "select lname, fname, major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
    /*11. :*/  "select lname, fname, major from student where ((lname=Yang  major=CS) and age<23 )or lname=Jackson",
    /*12. :*/ "select lname, fname, major from student where ((lname=Yang or ) and age<23 )or lname=Jackson",
    /*13. :*/ "select lname, fname, major from student where ((lname=Yang or major=CS) and age<23 )or", 
    /*14. :*/ "select lname, fname, major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson"
    };
    for (int i = 0; i < commands.size();i++){
        cout << endl<< i + 1 << ") "<< commands[i];
        cout << command(commands[i]);
    }
}   
#endif
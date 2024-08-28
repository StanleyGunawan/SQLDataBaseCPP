// #ifndef SQL_PARSER_FUNCTIONS_CPP
// #define SQL_PARSER_FUNCTIONS_CPP

// #include "sql_parser_functions.h"
    
//     void init_table(int _table[][MAX_COLUMNS]){
//         for (int i = 0; i < MAX_ROWS; i++){
//             for (int j = 1; j < MAX_COLUMNS; j++){
//                 _table[i][j] = -1;
//             }
//         }
//     }

//     //Mark this state (row) with a 1 (success)
//     void mark_success(int _table[][MAX_COLUMNS], int state){
//         _table[state][0] = 1;
//     }

//     //Mark this state (row) with a 0 (fail)
//     void mark_fail(int _table[][MAX_COLUMNS], int state){
//         _table[state][0] = 0;
//     }

//     //true if state is a success state
//     bool is_success(int _table[][MAX_COLUMNS], int state){
//         return _table[state][0] == 1;
//     }

//     //Mark a range of cells in the array. 
//     void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state){

//         for (from; from <=to; from++){
//             _table[row][from] = state;
//         }
//     }
//     //Mark columns represented by the string columns[] for this row
//     void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state){       //probably change this
//         int i = 0;
//         while (columns[i] != '\0'){
//             _table[row][columns[i]] = state;
//             i++;
//         }
//     }

//     //Mark this row and column
//     void mark_cell(int row, int _table[][MAX_COLUMNS], int column, int state){
//         _table[row][column] = state;
//     }

//     //This can realistically be used on a small table
//     void print_table(int _table[][MAX_COLUMNS]){
//         for (int i = 0; i < MAX_ROWS; i++){
//             for (int j = 0; j < MAX_COLUMNS; j++){
//                 cout << _table[i][j] << "] ";
//             } 
//             cout << endl;
//         }
//     }

//     //show string s and mark this position on the string:
//     //hello world   pos: 7
//     //       ^
//     void show_string(char s[], int _pos){
//         cout << "\n" << s;
//         cout << "   pos: " << _pos << endl;
//         cout << setw(_pos) <<"^" << '\n';

//     }

// #endif //
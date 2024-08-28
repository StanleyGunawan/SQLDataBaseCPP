#ifndef BINARY_FILES_FILE_RECORD_H
#define BINARY_FILES_FILE_RECORD_H

#include "utilities.h"
typedef std::vector<string> vectorstr;

class FileRecord{
private:
    static const int MAX = 100;
    int recno;
    
public:
    char _record[MAX+1][MAX+1];
    //when you construct a Record, it's either empty or it
    //  contains a word
    FileRecord(){
        for (int i = 0; i <MAX+1; i ++){
            for (int j = 0; j <MAX+1; j ++){
                _record[i][j] = '\0';
            }
        }
        // for (int i = 0; i <= MAX; i++){
        //     _record[i][0] = '\0';
        // }
        
        recno = -1;
    }

    // FileRecord(char str[]){
    //     strncpy(_record, str, MAX);
    // }

    FileRecord(vectorstr vec){ 
        for (int i = 0; i <MAX+1; i ++){
            for (int j = 0; j <MAX+1; j ++){
                _record[i][j] = '\0';
            }
        }
        for (int i = 0; i < vec.size(); i++){
            strcpy(_record[i], vec[i].c_str());
        }
        recno = -1;
    }
    // FileRecord(string s){
    //     strncpy(_record, s.c_str(), MAX);
    // }

    long write(fstream& outs);              //returns the record number
    long read(fstream& ins, long recno);    //returns the number of bytes
                                            //      read = MAX, or zero if
                                            //      read passed the end of file

    
    friend ostream& operator<<(ostream& outs,
                               const FileRecord& r);
    
    vectorstr getRecord();
};

long FileRecord::write(fstream &outs){
    //r.write(f); //take the Record r and write it into file f
    //  and return the record number of this Record

    //write to the end of the file.
    int i = 0;
    long pos;
    while (_record[i][0] != '\0'){
        outs.seekg(0, outs.end);        //goes to end of file
        pos = outs.tellp();    //retrieves the current position of the file pointer
        outs.write(_record[i], sizeof(_record[0]));
        i++;
    }
    
    char temp[sizeof(_record)] = {'\0'};
    outs.write(temp,sizeof(_record) - (i * sizeof(_record[0])));
    return (pos)/sizeof(_record);  //record number
}


long FileRecord::read(fstream &ins, long recno){
    //returns the number of bytes read.
    //    r.read(f, 6);
    //    cout<<r<<endl;
    long pos= recno * sizeof(_record);
    ins.seekg(pos, ios_base::beg);
    // int i = 0;
    for (int i = 0; i < (sizeof(_record)/sizeof(_record[0]));i++){
        ins.read(_record[i], sizeof(_record[i]));
        _record[i][ins.gcount()] = '\0';
    }
    
    //don't you want to mark the end of  the cstring with null?
    //_record[] => [zero'\0'trash trash trash trash]
    //don't need the null character, but for those of us with OCD and PTSD:
    return ins.gcount();

}

vectorstr FileRecord::getRecord(){
        vectorstr vec;
        int i =0;
         while (_record[i][0] != '\0'){
            vec.push_back(_record[i]);
            i++;
         }
        
        return vec;
    }


ostream& operator<<(ostream& outs,const FileRecord& r){
    int i = 0;
    while (r._record[i][0] != '\0'){
        int width = strlen(r._record[i]) + 1;
        outs << setw(16 - width) << " " << r._record[i];
        i++;
    }
    return outs;
}

#endif

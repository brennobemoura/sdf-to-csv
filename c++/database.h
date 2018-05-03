#ifndef DATABASE_H
#define DATABASE_H

// FilaKey is a Queue that saves the header data in other that will appearing
#include "filakey.h"

// FilaValues is a Queue that saves the columns for each dataHeader
// Each node is a columns that has the QueueKey, in this case, a queue for values
#include "filavalues.h"

// Built-in file i/o
#include <fstream>

class dataBase {
private:
    FilaKey * key;
    FilaValues* values;
    ifstream* in;

    bool haschar(char c, string s, int offset = 0);
    bool hasstring(string c, string s, int at = 0);

    string removeLine(string str, int countingLast = 0);
    void checkConsistency(int ival, int offset=0);

    string readLines();
    string* breakEspace(string line, int offset=0, int length=0);
    string getKey();
    string removeEspaces(string line);
    string strReplace(string rep, string to, string str);

public:
    dataBase();

    bool insert(string key, string value);

    bool read_sdf(string file);
    bool to_csv(string file);

    ~dataBase();
};

#endif // DATABASE_H

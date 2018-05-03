#include "database.h"

dataBase::dataBase() {
    this->key = new FilaKey();
    this->values = new FilaValues();
    this->in = 0;
}

dataBase::~dataBase() {
    delete this->key;
    delete this->values;

    this->key = 0;
    this->values = 0;
    this->in = 0;
}

// Check if the string has the character staring at offset
// By default, offset is setted with 0 index
bool dataBase::haschar(char c, string s, int offset) {
    for (unsigned int i = offset; i < s.length(); i++)
        if (c == s[i])
            return true;

    return false;
}

// Check if the string has some string inside
bool dataBase::hasstring(string c, string s, int at) {
    if (!s[at])
        return false;
    for (unsigned int i = 0; i < c.length(); i++) {
        if (c[i] != s[i + at]) {
            return false;
        }
    }

    return true;
}

// Maybe not working properly
// Haddle with \n character, trying to ignore the rest of the string
// Need to fix
string dataBase::removeLine(string str, int countingLast) {
    if (!str.length())
        return "";
    if (!countingLast) {
        string nstr = "";
        for (unsigned int i = 0; i < str.length(); i++)
            if (str[i] != '\n')
                nstr += str[i];

        return nstr;
    }

    for (int k = 0; k < countingLast; k++) {
        string nstr = "";
        for (int i = str.length()-1; i >= 0; i--) {
            if (str[i] == '\n') {
                for (int j = 0; j < i; j++)
                    nstr += str[j];
                break;
            }
        }
        if (nstr.length())
            str = nstr;
    }
    
    return str;
}

// This fuction check the lenght of headers block
// If the columns doesn't has the same length, this fuction will fill the rows with empty data
void dataBase::checkConsistency(int ival, int offset) {
    int max = this->values->lengthValues(0);
    int cmp = this->values->lengthValues(ival);

    if (cmp == max)
        return;

    for (; cmp < max+offset; cmp++)
        this->values->insert(ival, "");
}

// This haddle the header of each block
// The sdf that I got, all them has
/*
*   --Some Block header identifier
*
*   Atom map
*   M END
*
*   > <The headers data>
*   The values
*/
string dataBase::getKey() {
    string key = "";
    while (!key.length() && !this->in->eof()) {
        key = removeLine(readLines());
    }

    return removeEspaces(key);
}

// Get line with just one space
// string readed: This      is a example
// string out: This is a example
string dataBase::removeEspaces(string line) {
    string lout = "";
    for (char c: line) {
        if (lout.length() != 0 || c != ' ') {
            if (!(lout[lout.length()-1] == ' ' && c == ' '))
                if (c != '\t')
                    lout += c;
        }
    }

    if (lout[lout.length()-1] == ' ')
        lout.pop_back();

    return lout;
}

// strReplace change the text specified to other
// string rep is the string to replace
// string to is the string to insert
// str is the string specified
string dataBase::strReplace(string rep, string to, string str) {
    string srep = "";
    for (unsigned int i = 0; i < str.length(); i++) {
        unsigned int j = 0;
        for (; j < rep.length(); j++)
            if (rep[j] != str[i+j])
                break;

        if (j == rep.length()) {
            srep += to;
            if (rep.length())
                i += rep.length() - 1;
        } else
            srep += str[i];
    }

    return srep;
}

// This fuction don't work properly
// The intetion is to read the lines that has data
// Cannot read header data, because it is a data title
/*
*   > <Data header>
*   Some value
*
*   Data line problem
*
*   > <Another Header>
*   Example
*/
// Has created to read data that is sequencial lines, like matrix
/*
*   > <Data header>
*   0, 1, 4, Some Value
*   2, 1, 4, Another Value
*
*   > <Some Header>
*   Example
*/
// Need to fix
string dataBase::readLines() {
    string line, aux;
    do {
        getline(*this->in, aux);
        if (hasstring("M  END", aux))
            break;
        line += aux + '\n';
    } while(aux.length() && !this->in->eof());
    return removeLine(line, 2);
}

// Push data in the Queue
bool dataBase::insert(string key, string value) {
    if (haschar('\"', key))
        key = strReplace("\"", "\"\"", key);
    if (haschar('\"', value))
        value = strReplace("\"", "\"\"", value);

    if (haschar(',', key))
        key = "\"" + key + "\"";
    if (haschar(',', value) || haschar('\n', value))
        value = "\"" + value + "\"";
    
    int ikey = this->key->hasValue(key);
    if (ikey >= 0) {
        this->checkConsistency(ikey, -1);
        return this->values->insert(ikey, value);
    }

    this->key->insert(key);
    ikey = this->key->hasValue(key);
    this->checkConsistency(ikey, -1);
    return this->values->insert(ikey, value);
}

// Trash file has the content that the dataBase can't understand like data ou column header
// The indention is to not lose data, but because off some new lines between data, the algorithm can't read properly
bool dataBase::read_sdf(string file) {
    ifstream arq(file + ".sdf", ios::in);
    ofstream trash("trash_"+file+".csv", ios::out);

    this->in = &arq;
    if (!this->in->is_open() || !trash.is_open()) {
        cout << "Can't open file: " << file << "\n\n";
        return false;
    }

    string key = "";
    key = getKey();
    if (key.length()) {
        this->insert("index", key);
        this->insert("atom_block", readLines());
    }

    while (!this->in->eof()) {
        key = "";
        getline((*this->in), key);
        key = removeLine(key);

        if (key == "$$$$") {
            key = getKey();
            if (key.length()) {
                this->insert("index", key);
                this->insert("atom_block", readLines());
            }

        } else if (hasstring("> <", key) && haschar('>', key, 3)) {
            string ky = "";
            for (unsigned int i = 3; i < key.length(); i++) {
                if (key[i] == '>')
                    break;
                ky += key[i];
            }

            this->insert(ky, readLines());

        } else if (key.length())
            trash << key << "\n";
    }

    for (int i = 1; i < this->key->length(); i++)
        checkConsistency(i);

    this->in->close();
    trash.close();
    return true;
}

bool dataBase::to_csv(string file) {
    ofstream off(file +".csv", fstream::out);
    if (!off.is_open()) {
        cout << "Can't open out file\n\n";
        return false;
    }

    const int size = this->key->length();
    string keys = "",  aux = "";
    while (this->key->popFirst(&aux))
        keys += aux + ',';

    // Error if the dataBase didn't reads the sdf file
    if (keys == "") {
        cout << "Nothing to show\n";
        return false;
    }

    keys.pop_back();
    off << ',' << keys << endl;

    // Poping data til end
    int i = 0;
    cout << "Itens collected: " << this->values->lengthValues(0) << endl;
    while(this->values->lengthValues(0)) {
        off << i;
        string row = "";
        for (int j = 0; j < size; j++) {
            if (!this->values->popFirst(j, &aux))
                cout << "Erro! " << j << " - " << i << endl;

            row += "," + aux;
        }

        off << row << endl;
        i++;
    }

    // Cleaning all structure
    off.close();
    this->values->popAll();
    this->key->popAll();
    return true;
}


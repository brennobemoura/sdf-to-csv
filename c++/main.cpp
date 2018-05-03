// dataBase is the main Class
#include "database.h"

// given a file name like "example.sdf", de dataBase::read_sdf(string) can haddle the rest. That's it file + ".sdf"
// After that, the dataBase will allocate in memory the sdf, but in structural form
// You can easely mixed sdf files calling read_sdf more than one time
// To save the data in csv form, just call to_csv(string), the string will be the out file + ".csv"

int main() {
    // Create the dataBase haddle
    dataBase db;

    // Reading sdf file 
    db.read_sdf("example");

    // Saving in csv form
    db.to_csv("out_"+file);
    // After to_csv, the structure will be cleaned up
    
    return 0;
}

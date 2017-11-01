#define MYSQLPP_MYSQL_HEADERS_BURIED
#include <mysql++/mysql++.h>
#include <string>
#include <iostream>

void insert(mysqlpp::Connection& myDB, 
        const std::string& name,
        int pk) {
  mysqlpp::Query query = myDB.query();
  query << "INSERT Into test (pk,name) "
        << "VALUES (%0, %1q);";
  query.parse();  // Syntax check
  // Actually run query with values 
  // for bind variables
  query.store(pk,name);
}

int main(int argc, char *argv[]) {
    // Connect to database with: database, server, userID, password
    mysqlpp::Connection myDB("cse381", "localhost", "cse381", "m1am1");

	insert(myDB,"sherrie",2);
    // Create a query 
    mysqlpp::Query query = myDB.query();
    query << "SELECT pk,name from test";
    query.parse();  // check to ensure it is correct
    // Run the query and get stored results
    mysqlpp::StoreQueryResult result = query.store();
    // Results is a 2D vector of mysqlpp::String objects.
    // Print the results.
    for (size_t row = 0; (row < result.size()); row++) {
        std::string name = result[row][1].c_str();  
        int pk       = result[row][0];
        std::cout << pk << "\t" << name << std::endl;
    }
    // All done
    return 0;
}


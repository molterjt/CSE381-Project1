#define MYSQLPP_MYSQL_HEADERS_BURIED
#include <mysql++/mysql++.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "moviesdb.h"

#include <cstring>

mysqlpp::Query connect() {
    // Connect to database with: database, server, userID, password
    mysqlpp::Connection myDB("cse381", "localhost", "cse381", "m1am1");
    // Create a query 
    mysqlpp::Query query = myDB.query();
    std::cout << "Connected to DB\n" << std::endl;
    return myDB.query();
}
mysqlpp::Query movieList(){
    mysqlpp::Connection myDB("cse381", "localhost", "cse381", "m1am1");
    mysqlpp::Query query = myDB.query();
    query << "SELECT movies.title from movies";
    query.parse();
    mysqlpp::StoreQueryResult results = query.store();
    return myDB.query();
}


void getMovieListing(std::vector<std::string> &movies) {
    std::vector<std::string,> movieTitles;
    movieTitles = movieList();
    for(size_t row = 0; (row < movieTitles.size()); row++) {
        std::string title = movieTitles[row][0].c_str();
        movies.push_back(title);
        std::cout << title << "\n" << std::endl;
    }
    
}

void getMovieInfo(const char * path, std::string &buf) {
	buf += "title = " + std::string(path) + "\n";
}

int main(int argc, char *argv[]) {
    connect();
    (void) argc;
    (void) argv;
    //movieList();
    return 0;
}






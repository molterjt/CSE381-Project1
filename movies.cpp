#define MYSQLPP_MYSQL_HEADERS_BURIED
#include <mysql++/mysql++.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "moviesdb.h"
#include <cstring>
#include <algorithm>
std::vector<std::string> movies;

mysqlpp::Query connect() {
    // Connect to database with: database, server, userID, password
    mysqlpp::Connection myDB("cse381", "localhost", "cse381", "m1am1");
    // Create a query 
    mysqlpp::Query query = myDB.query();
    return myDB.query();
}

/*
mysqlpp::Query movieList(){
    mysqlpp::Connection myDB("cse381", "localhost", "cse381", "m1am1");
    mysqlpp::Query query = myDB.query();
    query << "SELECT movies.title from movies";
    query.parse();
    mysqlpp::StoreQueryResult results = query.store();
    return myDB.query();
}
*/

void getMovieListing(std::vector<std::string> &movies) {
    mysqlpp::Connection myDB("cse381", "localhost", "cse381", "m1am1"); 
    mysqlpp::Query query = myDB.query();
    query << "SELECT movies.title FROM movies";
    query.parse();
    mysqlpp::StoreQueryResult results = query.store();
    for(size_t row = 0; (row < results.size()); row++) {
        std::string title = results[row][0].c_str();
        movies.push_back(title);
        std::cout << title << "\n" << std::endl;
    }
    
}

void getMovieInfo(const char * path, std::string &buf) {
    
   // std::cout << movieDetail << "\n";
   // std::string movieDetail = std::string(path).erase(0,1);
    std::string movieDetail = std::string(path);
    char chars[] = "/";
    for(unsigned int i = 0; i < strlen(chars); ++i){
       movieDetail.erase(std::remove(movieDetail.begin(), movieDetail.end(), chars[i]), movieDetail.end());
    }
    mysqlpp::Connection myDB("cse381", "localhost", "cse381", "m1am1"); 
    mysqlpp::Query query = myDB.query();
    query << "SELECT title,tagline,genre,overview,release_date,budget,revenue,homepage,vote_average,comments FROM movies WHERE movies.title = %0q";
    query.parse();
    mysqlpp::StoreQueryResult results = query.store(movieDetail);
<<<<<<< HEAD
    if(results.size() < 1){
        buf += "The provided movie: " + movieDetail + " is not in the database. Try again.\n";
    } else {
            for(size_t row = 0; (row < results.size()); row++) {
                std::string title = results[row][0].c_str();
                std::string tagline = results[row][1].c_str();
                std::string genre = results[row][2].c_str();
                std::string overview = results[row][3].c_str();
                std::string release_date = results[row][4].c_str();
                std::string budget = results[row][5].c_str();
                std::string revenue= results[row][6].c_str();
                std::string homepage= results[row][7].c_str();
                std::string voteAvg = results[row][8].c_str();
                std::string comments = results[row][9].c_str();
                // std::cout << "title: " << title << "\n";
                //std::cout << "tagline: " << tagline << "\n";
                // std::cout << "genre: " << genre << "\n";
                // std::cout << "release_date: " << release_date << "\n";
                // std::cout << "budget: " << budget << "\n";
                // std::cout << "revenue: " << revenue << "\n";    
                //  buf += "title = " + std::string(path) + "\n";
                buf += "Title = " + title + "\n";
                buf += "Tagline = " + std::string(tagline) + "\n";
                buf += "Genre = " + genre + "\n"; 
                buf += "Overview = " + overview + "\n";
                buf += "Release_date = "  + release_date + "\n";
                buf += "Budget = $" + budget + "\n";
                buf += "Revenue = $" + revenue + "\n";
                buf += "HomePage = " + homepage + "\n";
                buf += "Vote Average = " + voteAvg + "\n";
                buf += "Comments = " + comments + "\n";
                }
            }
=======
    for(size_t row = 0; (row < results.size()); row++) {
        std::string title = results[row][0].c_str();
        std::string tagline = results[row][1].c_str();
        std::string genre = results[row][2].c_str();
        std::string release_date = results[row][3].c_str();
        std::string budget = results[row][4].c_str();
        std::string revenue= results[row][5].c_str();
        std::cout << "title: " << title << "\n";
        std::cout << "tagline: " << tagline << "\n";
        std::cout << "genre: " << genre << "\n";
        std::cout << "release_date: " << release_date << "\n";
        std::cout << "budget: " << budget << "\n";
        std::cout << "revenue: " << revenue << "\n";
    
    
    buf += "title = " + std::string(path) + "\n";
    buf += "tagline = " + std::string(tagline) + "\n";
    buf += "genre = " + genre + "\n";
    buf += "release_date = " + release_date + "\n";
    buf += "budget = $" + budget + "\n";
    buf += "revenue = $" + revenue + "\n";

    }
>>>>>>> 9148b05d1828885c6291fec668efeb88a1d5f38a
}


//main() created for testing purposes only
/*
int main(int argc, char *argv[]) {
   if( connect() ){
      std::cout << "Connected" << "\n";   }else {
      std::cout << "Error connecting \n";
    }
    (void) argc;
    (void) argv;
   // getMovieListing(movies);
    //movieList();
    const char * path = "/Space Jam";
    std::string buf;
    getMovieInfo(path, buf);
    std::string searchTitle = "";
    std::cout << "Enter a movie title for details: \n";
    std::getline(std::cin, searchTitle);
    std::cout << "Searching for " << searchTitle << "... ... \n";
    mysqlpp::Connection myDB("cse381", "localhost", "cse381", "m1am1");
    mysqlpp::Query query = myDB.query();
    query << "SELECT title,tagline,genre,release_date,budget,revenue FROM movies WHERE movies.title = %0q";
    query.parse();
    mysqlpp::StoreQueryResult results = query.store(searchTitle);
    for(size_t row = 0; (row < results.size()); row++) {
        std::string title = results[row][0].c_str();
        std::string tagline = results[row][1].c_str();
        std::string genre = results[row][2].c_str();
        std::string release_date = results[row][3].c_str();
        std::string budget = results[row][4].c_str();
        std::string revenue= results[row][5].c_str();
        std::cout << "title: " << title << "\n";
        std::cout << "tagline: " << tagline << "\n";
        std::cout << "genre: " << genre << "\n";
        std::cout << "release_date: " << release_date << "\n";
        std::cout << "budget: " << budget << "\n";
        std::cout << "revenue: " << revenue << "\n";
    }
    return 0;
}
*/

#include "adjacencyList.h"
#include "adjacencyMatrix.h"

/*
g++ -std=c++14 -Werror -Wuninitialized -o main src/main.cpp && ./main
*/

int main() {
    ifstream infile("text.txt");
    AdjacencyList list;
    AdjacencyMatrix matrix;
    string line, id, name, type, year, temp;
    int time;
    getline(infile, temp);

    while (getline(infile, line)) {
        vector<string> genre;
        stringstream ss(line);

        getline(ss, id, '\t');
        getline(ss, temp, '\t');
        getline(ss, name, '\t');
        getline(ss, temp, '\t');
        getline(ss, temp, '\t');
        getline(ss, year, '\t');
        getline(ss, temp, '\t');
        getline(ss, temp, '\t');
        getline(ss, type);

        id = id.substr(2,7);
        if (year == "\\N") {
            time = 0000;
        }
        else {
            time = stoi(year);
        }

        stringstream gen(type);
        while (getline(gen, temp, ',')) {
            if (temp == "\\N") {
                temp = "Unclassified";
            }
            genre.push_back(temp);
        }

        list.insert(time, name, genre);
        matrix.insert(time, name, genre);
    }

    cout << "\nWelcome to Movie Matchmaker!" << endl;
    cout << "\nAfter inputting some desired criteria and some movie ratings," << endl;
    cout << "your movie recommendations will be tailored to you.\n" << endl;

    cout << "List of All Movie Genres:" << endl;
    list.print(list.movieGenre());
    string userGenre = "";
    cout << "\nPlease input a movie genre from the selection above to narrow down your movie recommendations." << endl;
    cin >> userGenre;

    cout << "\nList of All Movie Years:" << endl;
    list.printTimeSpan();
    int userStartYear = 0, userEndYear = 0;
    cout << "\nPlease input a range of years from the selection above to narrow down your movie recommendations. (see example below)\n" << endl;
    cout << "Example:" << endl;
    cout << "Start year: 2003" << endl;
    cout << "End year: 2010" << endl;
    cout << "Range of years: 2003 - 2010\n" << endl;
    cout << "Start year: ";
    cin >> userStartYear;
    cout << "End year: ";
    cin >> userEndYear;

    cout << "\nUser inputted:" << endl;
    cout << "Genre: " << userGenre << endl;
    cout << "Range of years: " << userStartYear << " - " << userEndYear << endl;
    cout << endl;

    AdjacencyList userPrefList;

    // data structure implementations:

    // key: year
    // value: pair.first = genre, pair.second = movie name
    //map<int, vector<pair<string, vector<string>>>> graph;

    // key: year
    // value: pair.first = genre, pair.second.first = movie name, pair.second.second = user movie rating
    //map<int, vector<pair<string, vector<pair<string, int>>>>> userGraph;   //with ratings

    for (auto i = list.graph.begin(); i != list.graph.end(); i++) {
        for (auto j = i->second.begin(); j != i->second.end(); j++) {
            for (auto k = j->second.begin(); k != j->second.end(); k++) {
                if ( (j->first == userGenre) && (i->first >= userStartYear && i->first <= userEndYear) ) {
                    userPrefList.copyToUserGraph(i->first, j->first, *k, -1);
                }
            }
        }
    }

    cout << "Please give your ratings for the following movies from 0-20, to help personalize your recommendations." << endl;
    cout << "There are 10 iterations, with 3 movies to rate in each iteration.\n" << endl;
    int userRating = 0;

    for (int i = 0; i < 10; i++) {
        cout << "\n~Iteration " << (i+1) << ":\n" << endl;
        for (int j = 0; j < 3; j++) {
            int random_year = rand() % (userEndYear - userStartYear + 1) + userStartYear;
            
            vector<pair<string, vector<pair<string, int>>>> movies = userPrefList.userGraph[random_year];
            vector<pair<string, int>> genre_movies_ratings;

            for (auto& movie : movies) {
                if (movie.first == userGenre) {
                    genre_movies_ratings = movie.second;
                    break;
                }
            }

            int num_movies = genre_movies_ratings.size();
            int random_movie_index = rand() % num_movies;
            pair<string, int> movie_rating = genre_movies_ratings[random_movie_index];

            cout << movie_rating.first << ": ";
            cin >> userRating;
            cout << endl;

            vector<pair<string, vector<pair<string, int>>>>& tempMovies = userPrefList.userGraph[random_year];

            for (auto& genre_movies : tempMovies) {
                if (genre_movies.first == userGenre) {
                    for (auto& tempMovie_rating : genre_movies.second) {
                        if (tempMovie_rating.first == movie_rating.first) {
                            tempMovie_rating.second = userRating;
                            break;
                        }
                    }
                    break;
                }
            }                
        }
    }

    vector<pair<string, int>> highestUserRatings;

    for (auto& year_movies : userPrefList.userGraph) {
        for (auto& genre_movies : year_movies.second) {
            for (auto& movie_rating : genre_movies.second) {
                highestUserRatings.push_back(make_pair(movie_rating.first, movie_rating.second));
            }
        }
    }

    sort(highestUserRatings.begin(), highestUserRatings.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
    });

    cout << "\nHere's your top 10 movie recommendations!\n" << endl;
    cout << "Movie and Rating:" << endl;

    int count = 0;
    for (auto& rating : highestUserRatings) {
        if (count >= 10) {
            break;
        }
        cout << (count+1) << ".\t" << rating.first << " (" << rating.second << "/20)" << endl;
        count++;
    }

    return 0;
}


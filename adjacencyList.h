// 1984 -- Group 84
// Members: Jackie Wu, Jonas Wood, Victoria App

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
using namespace std;

class AdjacencyList {
public:
    // key: year
    // value: pair.first = genre, pair.second = movie name
    map<int, vector<pair<string, vector<string>>>> graph;

    // key: year
    // value: pair.first = genre, pair.second.first = movie name, pair.second.second = user movie rating
    map<int, vector<pair<string, vector<pair<string, int>>>>> userGraph;   //with ratings

    void insert(int year, string name, vector<string> genre);
    void copyToUserGraph(int year, string genre, string name, int rating);
    void printAll();
    vector<int> timeSpan();
    vector<string> movieGenre();
    vector<string> findYear(int year);
    vector<string> findGenre(string genre);
    map<int, pair<string, vector<string>>> findName(string name);
    void printTimeSpan();
    void print(vector<string> temp);
    void printFindName(map<int, pair<string, vector<string>>> nameList);
};

// inserts into the graph
void AdjacencyList::insert(int year, string name, vector<string> genre) {
    for (int i = 0; i < genre.size(); i++) {
        if (!graph.count(year)) {
            vector<string> temp;
            temp.push_back(name);
            graph[year].push_back(make_pair(genre[i], temp));
        }
        else {
            bool genreFound = false;
            int genreLoc;
            for (int j = 0; j < graph[year].size(); j++) {
                if (graph[year][j].first == genre[i]) {
                    genreFound = true;
                    genreLoc = j;
                }
            }
            if (genreFound) {
                graph[year][genreLoc].second.push_back(name);
            }
            else {
                vector<string> temp;
                temp.push_back(name);
                graph[year].push_back(make_pair(genre[i], temp));
            }
        }
    }
}

// copies applicable data into a new graph, based off desired years and genre
void AdjacencyList::copyToUserGraph(int year, string genre, string name, int rating) {
    if (!userGraph.count(year)) {
        vector<pair<string, int>> temp;
        temp.push_back(make_pair(name, rating));
        userGraph[year].push_back(make_pair(genre, temp));
    }
    else {
        bool genreFound = false;
        int genreLoc;
        for (int j = 0; j < userGraph[year].size(); j++) {
            if (userGraph[year][j].first == genre) {
                genreFound = true;
                genreLoc = j;
            }
        }
        if (genreFound) {
            pair<string, int> tempPair = make_pair(name, rating);
            userGraph[year][genreLoc].second.push_back(tempPair);
        }
        else {
            vector<pair<string, int>> temp;
            temp.push_back(make_pair(name, rating));
            userGraph[year].push_back(make_pair(genre, temp));
        }
    }
}

void AdjacencyList::printAll() {
    for (auto i = graph.begin(); i != graph.end(); i++) {
        for (auto j = i->second.begin(); j != i->second.end(); j++) {
            for (auto k = j->second.begin(); k != j->second.end(); k++) {
                cout << i->first << "   " << j->first << "   " << *k << endl;
            }
        }
    }
}

vector<int> AdjacencyList::timeSpan() {
    vector<int> temp;
    for (auto i = graph.begin(); i != graph.end(); i++) {
        temp.push_back(i->first);
    }
    return temp;
}

vector<string> AdjacencyList::movieGenre() {
    vector<string> temp;
    set<string> temp1;
    for (auto i = graph.begin(); i != graph.end(); i++) {
        for (auto j = i->second.begin(); j != i->second.end(); j++) {
            temp1.insert(j->first);
        }
    }
    for (auto i = temp1.begin(); i != temp1.end(); i++) {
        temp.push_back(*i);
    }
    return temp;
}

vector<string> AdjacencyList::findYear(int year) {
    vector<string> temp;
    set<string> nameSet;

    if (graph.count(year)) {
        for (auto i = graph[year].begin(); i != graph[year].end(); i++) {
            for(auto j = i->second.begin(); j != i->second.end(); j++) {
                nameSet.insert(*j);
            }
        }
        for (auto i = nameSet.begin(); i != nameSet.end(); i++) {
            temp.push_back(*i);
        }
        return temp;
    }
    temp.push_back("Year Not Found");
    return temp;
}

vector<string> AdjacencyList::findGenre(string genre) {
    vector<string> temp;
    set<string> nameSet;
    bool genreFound = false;

    for (auto i = graph.begin(); i != graph.end(); i++) {
        for (auto j = i->second.begin(); j != i->second.end(); j++) {
            if (j->first == genre) {
                genreFound = true;
                for (auto k = j->second.begin(); k != j->second.end(); k++) {
                    nameSet.insert(*k);
                }
            }
        }
    }

    if (genreFound) {
        for (auto i = nameSet.begin(); i != nameSet.end(); i++) {
            temp.push_back(*i);
        }
        return temp;
    }
    temp.push_back("Genre Not Found");
    return temp;
}

map<int, pair<string, vector<string>>> AdjacencyList::findName(string name) {
    map<int, pair<string, vector<string>>> nameList;
    bool nameFound = false;

    for (auto i = graph.begin(); i != graph.end(); i++) {
        for (auto j = i->second.begin(); j != i->second.end(); j++) {
            for (auto k = j->second.begin(); k != j->second.end(); k++) {
                if (*k == name) {
                    nameFound = true;

                    if (nameList[i->first].first != name) {
                        vector<string> temp;
                        temp.push_back(j->first);
                        nameList[i->first] = make_pair(name, temp);
                    }
                    else {
                        nameList[i->first].second.push_back(j->first);
                    }
                }
            }
        }
    }

    if (nameFound) {
        return nameList;
    }
    else {
        map<int, pair<string, vector<string>>> notFound;
        vector<string> temp;
        notFound[-1] = make_pair("Name Not Found", temp);
        return notFound;
    }
}

void AdjacencyList::printTimeSpan() {
    vector<int> temp = timeSpan();
    for (int i = 0; i < temp.size() - 1; i++) {
        cout << temp[i] << " || ";
    }
    cout << temp.back() << endl;
}

void AdjacencyList::print(vector<string> temp) {
    for (int i = 0; i < temp.size() - 1; i++) {
        cout << temp[i] << " || ";
    }
    cout << temp.back() << endl;
}

void AdjacencyList::printFindName(map<int, pair<string, vector<string>>> nameList) {
    if (nameList.count(-1)) {
        cout << "Name Not Found" << endl;
    }
    else {
        for (auto i = nameList.begin(); i != nameList.end(); i++) {
            cout << i->first << "   " << i->second.first << "   ";
            for (int j = 0; j < i->second.second.size() - 1; j++) {
                cout << i->second.second[j] << " || ";
            }
            cout << i->second.second.back() << endl;
        }
    }
}


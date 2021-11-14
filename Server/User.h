#pragma once

#ifndef User_h_
#define User_h_
#include <iostream>
#include <string>
#include <algorithm>
#include<vector>

class User
{
public:
    int id;
    std::string name;
    std::string status;     // success, miss, win, lose
    std::string ans;        // answer 1 character
    std::string final_ans;  // answer whole keyword
    int mode;               // mode of answer (0 = character, 1 =  whole keyword)
    int score;
    bool turn = false;    // current turn
    int rank;


    User(int id, std::string name, int score) {
        this->id = id;
        this->name = name;
        this->score = score;
    }

    User(int id, std::string name, std::string status, std::string ans, std::string final_ans, int mode, int score, bool turn, int rank) {
        this->id = id;
        this->name = name;
        this->status = status;
        this->ans = ans;
        this->final_ans = final_ans;
        this->mode = mode;
        this->score = score;
        this->turn = turn;
        this->rank = rank;
    };

    void printUser();

};

#endif // !User_h_
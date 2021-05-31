//
// Created by Cihat Özdemir on 7.05.2021.
//

#ifndef QLEARNALGORITHM_QLEARN_H
#define QLEARNALGORITHM_QLEARN_H

#include <iostream>
#include <math.h>
#include <vector>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>

using namespace std;

class Vector2d{
public:
    int x, y;

public:
    Vector2d(int _x, int _y):x(_x), y(_y){
    }

    void Set(int _x, int _y){
        x = _x;
        y = _y;
    }

};

class QLearn {
private:
    int** map;
    int** reward_table;
    int reward_size;
    double** q_table;
    double gamma;
    int try_times;
    void(* adressLocation)();
    int GetReward(int value);
    int GetMax(int nodeX, int nodeY);
public:
    int width , heigth;
    int startXpoint, startYpoint, finishXpoint, finishYpoint;
    vector<int> episodeviastep;
    vector<int> episodeviacost;
public:
    QLearn();
    int**  GetMap();
    void GenerateMap(int x, int y);
    void GenerateMap(int ** _map, int _width, int _height, int _startX, int _startY, int _finishX, int _finishY);
    void DrawMap();
    void Init(double _gamma, int trytimes);
    void CreateRewardTable();
    void ForceToLearn();
    vector<int> GetShortestPath();
    void MovePlayer(int direction, int* playerX, int* playerY);
    void SetStartLocation(int _x, int _y);
    void SetEndLocation(int _x, int _y);
    void SetLearnFinishEvent(void(* _adressLocation)());
    bool PlayerLocationsIsContains(vector<Vector2d> locations, int locationX, int locationY);
};


#endif //QLEARNALGORITHM_QLEARN_H

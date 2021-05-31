//
// Created by Cihat Özdemir on 7.05.2021.
//

#include "qlearn.h"

QLearn::QLearn()
{

}

void QLearn::SetStartLocation(int _x, int _y){
    startXpoint = _x;
    startYpoint = _y;
}

void QLearn::SetEndLocation(int _x, int _y){
    finishXpoint = _x;
    finishYpoint = _y;
}

int** QLearn::GetMap(){
    return map;
}

/// 0 -> ROAD   1 -> WALL    2-> START    3 -> END
void QLearn::GenerateMap(int** _map, int _width, int _height, int _startX, int _startY, int _finishX, int _finishY) {
    map = _map;
    width = _width;
    heigth = _height;
    startXpoint = _startX;
    startYpoint = _startY;
    finishXpoint = _finishX;
    finishYpoint = _finishY;
}

void QLearn::GenerateMap(int x, int y) {
    srand(time(NULL));

    map = new int *[y];
    for(int i = 0;i < y;i++)
        map[i] = new int[x];

    width = x;  heigth = y;

    for(int i = 0;i < x;i++){
        for(int j = 0;j < y;j++){
            map[i][j] = rand() % 2 * rand() % 2;
        }
    }
    //startXpoint = rand() % x;   startYpoint = rand() % y;
    map[startXpoint][startYpoint] = 2;
    //finishXpoint = rand() % x;   finishYpoint = rand() % y;
    map[finishXpoint][finishYpoint] = 3;
}



void QLearn::DrawMap() {

    QFile file(QCoreApplication::applicationDirPath() + "/engel.txt");
    QTextStream stream(&file);
    file.open(QIODevice::ReadWrite);

    for(int i = 0;i < width;i++){
        for(int j = 0;j < heigth;j++){
            switch (map[i][j]) {
                case 0:
                    cout << " ";
                    break;
                case 1:
                    cout << "*";
                    break;
                case 2:
                    cout << "S";
                    break;
                case 3:
                    cout << "E";
                    break;
            }

            stream << map[i][j] << " ";
        }
        cout << endl;
        stream << Qt::endl;
    }

    file.close();
}

void QLearn::Init(double _gamma, int trytimes) {
    gamma = _gamma;
    try_times = trytimes;

    CreateRewardTable();

    cout << "Q TABLE X : " << finishXpoint << " Y : " << finishYpoint << " START : " << startXpoint << " " << startYpoint << endl;
    //START LEARNING
    ForceToLearn();
    //GET THE SHORTEST PATH
    vector<int> shortPath = GetShortestPath();
    cout << "EN KISA YOL EKTEDİR" << endl;
    for(int i = 0;i < shortPath.size();i++)
        cout << shortPath.at(i) << ", ";
}

void QLearn::ForceToLearn() {
    int costCounter = 0;
    //CREATE Q TABLE
    q_table = new double *[reward_size];
    for(int i = 0; i < reward_size; i++)
        q_table[i] = new double[8];

    Vector2d lastLocation(0, 0);

    for(int i = 0; i < try_times; i++){
        //RESET PLAYER POSITION
        int playerX = startXpoint, playerY = startYpoint;
        int stepcounter = 0;    costCounter = 0;

        while(true){
            int randomWay = rand() % 8;

            if(reward_table[playerX * width + playerY][randomWay] == -1){
                q_table[playerX * width + playerY][randomWay] = -1000;
                continue;
            }


            lastLocation.Set(playerX, playerY);

            MovePlayer(randomWay, &playerX, &playerY);
            stepcounter++;

            //if(playerX == lastLocation.x && playerY == lastLocation.y)
            //    continue;

            //cout << "X : " << playerX << " Y : " << playerY << " DIZI KONUM " << playerX * width + playerY << endl;
            //ASSIGN Q TABLE
            q_table[lastLocation.x * width + lastLocation.y][randomWay] =
                    reward_table[lastLocation.x * width + lastLocation.y][randomWay] + (gamma * GetMax(playerX, playerY));

            if(playerX == finishXpoint && playerY == finishYpoint){
                break;
            }

            //EPISODE VIA COST
            costCounter += reward_table[lastLocation.x * width + lastLocation.y][randomWay];
        }

        episodeviacost.push_back(costCounter);
        episodeviastep.push_back(stepcounter);
    }

    for(int i = 0; i < reward_size; i++){
        cout << i <<". ";
        for(int j = 0;j < 8;j++){
            cout << q_table[i][j] << " ";
        }
        cout << endl;
    }
}

void QLearn::CreateRewardTable() {
    reward_size = width * heigth;
    reward_table = new int *[reward_size];
    for(int i = 0;i < reward_size;i++)
        reward_table[i] = new int[8];

    for(int i = 0; i < reward_size; i++){
        for(int j = 0;j < 8;j++){
            reward_table[i][j] = -1;
        }
    }
    ///0 -> KUZEY
    for(int i = 0; i < width; i++){
        for(int j = 0; j < heigth; j++){
            if(i > 0)
                reward_table[i * width + j][0] = GetReward(map[i - 1][j]);
            if(i < width - 1)
                reward_table[i * width + j][4] = GetReward(map[i + 1][j]);

            if(j > 0)
                reward_table[i * width + j][6] = GetReward(map[i][j - 1]);
            if(j < heigth - 1)
                reward_table[i * width + j][2] = GetReward(map[i][j + 1]);

            if(i > 0 && j > 0)
                reward_table[i * width + j][7] = GetReward(map[i - 1][j - 1]);
            if(i < width - 1 && j < width - 1)
                reward_table[i * width + j][3] = GetReward(map[i + 1][j + 1]);

            if(j > 0 && i < width - 1)
                reward_table[i * width + j][5] = GetReward(map[i + 1][j - 1]);
            if(j < heigth - 1 && i > 0)
                reward_table[i * width + j][1] = GetReward(map[i - 1][j + 1]);
        }
    }
    cout << "ÖDÜL TABLOSU" << endl;
    for(int i = 0; i < reward_size; i++){
        cout << i <<". ";
        for(int j = 0;j < 8;j++){
            cout << reward_table[i][j] << " ";
        }
        cout << endl;
    }
}

int QLearn::GetReward(int value) {
    if(value == 0)
        return 3;
    if(value == 1)
        return -5;
    if(value == 2)
        return 3;
    if(value == 3)
        return 1000;

    return 0;
}

int QLearn::GetMax(int nodeX, int nodeY) {
    int maxValue = -9999;

   /* int startNodeX = clamp(nodeX - 1, 0, width - 1);
    int startNodeY = clamp(nodeY - 1, 0, heigth - 1);
    int endNodeX = clamp(nodeX + 1, 0, width - 1);
    int endNodeY = clamp(nodeY + 1, 0, heigth - 1);

    vector<Vector2d> nodeList;

    for(int i = startNodeX;i < endNodeX;i++){
        for(int j = startNodeY;i < endNodeY;j++){
            nodeList.push_back(Vector2d(i, j));
        }
    } */

    for(int i = 0;i < 8;i++){
        if(q_table[nodeX * width + nodeY][i] > maxValue){
            maxValue = q_table[nodeX * width + nodeY][i];
        }
    }

    return maxValue;
}

vector<int> QLearn::GetShortestPath() {
    vector<int> pathList;
    int playerX = startXpoint, playerY = startYpoint, nextX, nextY;
    vector<Vector2d> playerPositions;
    while(true){
        double max = -9999;
        int selectedDirection = 0;

        for(int i = 0;i < 8;i++){
            if(q_table[playerX * width + playerY][i] > max){
                if((pathList.size() > 0 && (i < 4 ? i + 4 : i - 4)  == pathList.at(pathList.size() - 1)) ||
                        reward_table[playerX * width + playerY][i] == -1)
                    continue;

                nextX = playerX;    nextY = playerY;
                MovePlayer(i, &nextX, &nextY);

                if(PlayerLocationsIsContains(playerPositions, nextX, nextY))
                    continue;

                max = q_table[playerX * width + playerY][i];
                selectedDirection = i;
            }
        }

        pathList.push_back(selectedDirection);

        MovePlayer(selectedDirection, &playerX, &playerY);

        playerPositions.push_back(Vector2d(playerX, playerY));

        cout << selectedDirection << " " << playerX << " " << playerY << endl;

        if(playerX == finishXpoint && playerY == finishYpoint)
            break;
    }

    return pathList;
}

bool QLearn::PlayerLocationsIsContains(vector<Vector2d> locations, int locationX, int locationY){
    for(int i = 0, n = locations.size();i < n;i++){
        if(locations.at(i).x == locationX && locations.at(i).y == locationY)
            return true;
    }

    return false;
}

void QLearn::MovePlayer(int direction, int* playerX, int* playerY) {
    switch (direction) {
        case 0:
            //if(*playerX > 0)
                *playerX-=1;
            break;
        case 1:
            //if(*playerX > 0 && *playerY < heigth - 1){
                *playerX-=1;
                *playerY+=1;
            //}
            break;
        case 2:
            //if(*playerY < heigth - 1)
                *playerY+=1;
            break;
        case 3:
            //if(*playerX < width - 1 && *playerY < width - 1){
                *playerY+=1;
                *playerX+=1;
            //}
            break;
        case 4:
            //if(*playerX < width - 1)
                *playerX+=1;
            break;
        case 5:
            //if(*playerX < width - 1 && *playerY > 0){
                *playerX+=1;
                *playerY-=1;
            //}
            break;
        case 6:
            //if(*playerY > 0)
                *playerY-=1;
            break;
        case 7:
            //if(*playerY > 0 && *playerX > 0){
                *playerY-=1;
                *playerX-=1;
            //}
            break;
    }
}

void QLearn::SetLearnFinishEvent(void(* _adressLocation)()){
    adressLocation = _adressLocation;
}

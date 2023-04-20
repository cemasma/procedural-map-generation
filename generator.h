#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <vector>
#include <map>
#include <string>

enum directions {
    UP = 0,
    DOWN = ~UP,
    RIGHT = 2,
    LEFT = ~RIGHT
};

struct ELEMENT {
    int x;
    int y;
    int z;
    int t;
    int direction;
};

typedef std::vector<ELEMENT> elementVector;

struct NEIGHBOR {
    bool isOk;
    int selection;
    elementVector elements;
};

struct NEIGHBORS {
    NEIGHBOR horizontal;
    NEIGHBOR vertical;
};

struct POINT {
    int x;
    int y;
};

struct ROOM {
    POINT point;
    int height;
    int width;
};

typedef std::map<std::string, std::vector<std::string>> wallRecordsMap;
typedef std::map<std::string, std::map<int, bool>> spacesMap;
typedef std::vector<ROOM> roomVector;

int random(int max);
bool labyrintProgress(wallRecordsMap wallRecords, int row, int col, int total);
bool recordCheck(int x, int y, int z, int t, wallRecordsMap wallRecords);
bool checkIfPointIsRemained(int x, int y, std::map<std::string, bool> remainedPointsMap);
NEIGHBORS getNeighbors(int x, int y, int row, int col, wallRecordsMap wallRecords, std::map<std::string, bool> remainedPointsMap);
bool isNeighborAvailable(int x, int y, NEIGHBOR neighbor, std::map<std::string, std::vector<std::string>> wallRecords);
void addRecord(int x, int y, int z, int t, std::map<std::string, std::vector<std::string>> *wallRecords);
void createRoads(int x, int y, int row, int col, std::map<std::string, std::vector<std::string>> *wallRecords, elementVector *map, std::map<std::string, bool> remainedPointsMap);
bool isItInsideOfRooms(int x, int y, roomVector rooms);
bool checkIfRechanglesHaveIntersection(int x, int y, int height, int width, roomVector rooms);
roomVector getRooms(int row, int col);
std::vector<POINT> getRemainedPoints(int row, int col, roomVector rooms);
std::map<std::string, bool> prepareRemainedPointsMap(std::vector<POINT> remainedPoints);
void prepareMazeSpaces(elementVector elements, spacesMap *spaces);
void prepareRoomSpaces(roomVector rooms, spacesMap *spaces);
spacesMap generateMap(int row, int col);

#endif
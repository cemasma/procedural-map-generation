#include "generator.h"
#include <SDL2/SDL.h>

bool labyrintProgress(wallRecordsMap wallRecords, int row, int col, int total) {
    return (
        wallRecords.size() == 1 && wallRecords.begin()->second.size() == total
    );
}

int random(int max) {
    if (max == 0) {
        return -1;
    }

    return rand() % max;
}

bool recordCheck(int x, int y, int z, int t, wallRecordsMap wallRecords) {
    std::string xyKey = std::to_string(x) + ":" + std::to_string(y);
    std::string ztKey = std::to_string(z) + ":" + std::to_string(t);

    for (wallRecordsMap::iterator it = wallRecords.begin(); it != wallRecords.end(); ++it) {
        if ((it->first == xyKey || it->first == ztKey) && it->second.empty() == false) {
            return false;
        }

        bool xyExists = false;
        bool ztExists = false;
        std::vector<std::string>::iterator vectorIterator = std::find(wallRecords[it->first].begin(), wallRecords[it->first].end(), xyKey);
        if (vectorIterator != wallRecords[it->first].end()) {
            xyExists = true;
        }

        vectorIterator = std::find(wallRecords[it->first].begin(), wallRecords[it->first].end(), ztKey);
        if (vectorIterator != wallRecords[it->first].end()) {
            ztExists = true;
        }

        if (xyExists && ztExists) {
            return false;
        }
    }
    return true;
}

bool checkIfPointIsRemained(int x, int y, std::map<std::string, bool> remainedPointsMap) {
    return remainedPointsMap[std::to_string(x) + ":" + std::to_string(y)] == true;
}

NEIGHBORS getNeighbors(int x, int y, int row, int col, wallRecordsMap wallRecords, std::map<std::string, bool> remainedPointsMap) {
    NEIGHBORS neighbors;
    NEIGHBOR horizontalNeighbor;
    NEIGHBOR verticalNeighbor;

    horizontalNeighbor.isOk = random(2);
    horizontalNeighbor.selection = -1;
    elementVector horizontalNeighborElements;
    horizontalNeighbor.elements = horizontalNeighborElements;

    verticalNeighbor.isOk = !horizontalNeighbor.isOk;
    verticalNeighbor.selection = -1;
    elementVector verticalNeighborElements;
    verticalNeighbor.elements = verticalNeighborElements;

    neighbors.horizontal = horizontalNeighbor;
    neighbors.vertical = verticalNeighbor;


    if (y - 1 > -1 && recordCheck(x, y, x, y - 1, wallRecords) && checkIfPointIsRemained(x, y - 1, remainedPointsMap)) {
        ELEMENT element;
        element.x = x;
        element.y = y - 1;
        element.direction = UP;

        neighbors.vertical.elements.push_back(element);
    }
    if (y + 1 < row && recordCheck(x, y, x, y + 1, wallRecords) && checkIfPointIsRemained(x, y + 1, remainedPointsMap)) {
        ELEMENT element;
        element.x = x;
        element.y = y + 1;
        element.direction = DOWN;

        neighbors.vertical.elements.push_back(element);
    }
    if (x + 1 < col && recordCheck(x, y, x + 1, y, wallRecords) && checkIfPointIsRemained(x + 1, y, remainedPointsMap)) {
        ELEMENT element;
        element.x = x + 1;
        element.y = y;
        element.direction = RIGHT;

        neighbors.horizontal.elements.push_back(element);
    }
    if (x - 1 > -1 && recordCheck(x, y, x - 1, y, wallRecords) && checkIfPointIsRemained(x - 1, y, remainedPointsMap)) {
        ELEMENT element;
        element.x = x - 1;
        element.y = y;
        element.direction = LEFT;

        neighbors.horizontal.elements.push_back(element);
    }

    if (neighbors.horizontal.isOk == true && neighbors.horizontal.elements.size() == 0 && neighbors.vertical.isOk == false) {
        neighbors.vertical.isOk = true;
    } else if (neighbors.vertical.isOk == true && neighbors.vertical.elements.size() == 0 && neighbors.horizontal.isOk == false) {
        neighbors.horizontal.isOk = true;
    }

    neighbors.horizontal.selection = random(neighbors.horizontal.elements.size());
    neighbors.vertical.selection = random(neighbors.vertical.elements.size());
    if (neighbors.horizontal.selection == -1) {
        neighbors.horizontal.isOk = false;
    }
    if (neighbors.vertical.selection == -1) {
        neighbors.vertical.isOk = false;
    }

    return neighbors;
}

bool isNeighborAvailable(int x, int y, NEIGHBOR neighbor, std::map<std::string, std::vector<std::string>> wallRecords) {
    return (neighbor.isOk && neighbor.elements.size() > 0 &&
        recordCheck(x, y, neighbor.elements[neighbor.selection].x, neighbor.elements[neighbor.selection].y, wallRecords));
}

void addRecord(int x, int y, int z, int t, std::map<std::string, std::vector<std::string>> *wallRecords) {
    std::string xyKey = std::to_string(x) + ":" + std::to_string(y);
    std::string ztKey = std::to_string(z) + ":" + std::to_string(t);
    std::string xyOwner;
    std::string ztOwner;
    bool xyState = false;
    bool ztState = false;

    for (std::map<std::string, std::vector<std::string>>::iterator it = wallRecords->begin(); it != wallRecords->end(); ++it) {
        std::vector<std::string> vector = it->second;
        std::vector<std::string>::iterator vectorIterator = std::find(vector.begin(), vector.end(), xyKey);

        if (vectorIterator != vector.end()) {
            xyOwner = it->first;
            xyState = true;
        }

        vectorIterator = std::find(vector.begin(), vector.end(), ztKey);
        if (vectorIterator != vector.end()) {
            ztOwner = it->first;
            ztState = true;
        }

        if (xyState && ztState) {
            break;
        }
    }

    if (xyState == true && ztState == true) {
        std::vector<std::string> xyOwnerVector = (*wallRecords)[xyOwner];
        std::vector<std::string> ztOwnerVector = (*wallRecords)[ztOwner];
        xyOwnerVector.insert(xyOwnerVector.end(), ztOwnerVector.begin(), ztOwnerVector.end());
        xyOwnerVector.push_back(ztOwner);

        (*wallRecords)[xyOwner] = xyOwnerVector;

        wallRecords->erase(ztOwner);
    } else if (xyState == false && ztState == true) {
        (*wallRecords)[ztOwner].push_back(xyKey);
    } else if (xyState == true && ztState == false) {
        (*wallRecords)[xyOwner].push_back(ztKey);
    } else if (xyState == false && ztState == false) {
        (*wallRecords)[xyKey].push_back(ztKey);
    }
}

void createRoads(int x, int y, int row, int col, std::map<std::string, std::vector<std::string>> *wallRecords, elementVector *map, std::map<std::string, bool> remainedPointsMap) {
    NEIGHBORS neighbors = getNeighbors(x, y, row, col, *wallRecords, remainedPointsMap);

    if (isNeighborAvailable(x, y, neighbors.vertical, *wallRecords)) {
        ELEMENT element;
        element.x = x;
        element.y = y;
        element.z = neighbors.vertical.elements[neighbors.vertical.selection].x;
        element.t = neighbors.vertical.elements[neighbors.vertical.selection].y;
        element.direction = neighbors.vertical.elements[neighbors.vertical.selection].direction;

        map->push_back(element);

        addRecord(
            x,
            y,
            neighbors.vertical.elements[neighbors.vertical.selection].x,
            neighbors.vertical.elements[neighbors.vertical.selection].y,
            wallRecords
        );
    }

    if (isNeighborAvailable(x, y, neighbors.horizontal, *wallRecords)) {
        ELEMENT element;
        element.x = x;
        element.y = y;
        element.z = neighbors.horizontal.elements[neighbors.horizontal.selection].x;
        element.t = neighbors.horizontal.elements[neighbors.horizontal.selection].y;
        element.direction = neighbors.horizontal.elements[neighbors.horizontal.selection].direction;

        map->push_back(element);

        addRecord(
            x,
            y,
            neighbors.horizontal.elements[neighbors.horizontal.selection].x,
            neighbors.horizontal.elements[neighbors.horizontal.selection].y,
            wallRecords
        );
    }
}

bool isItInsideOfRooms(int x, int y, roomVector rooms) {
    bool overlaps = false;
    for (int i = 0; i < rooms.size(); i++) {
        ROOM room = rooms.at(i);
        if (x >= room.point.x && x < room.point.x + room.width && y >= room.point.y && y < room.point.y + room.height) {
            overlaps = true;
            break;
        }
    }

    return overlaps;
}

bool checkIfRechanglesHaveIntersection(int x, int y, int height, int width, roomVector rooms) {
    bool overlaps = false;
    for (int i = 0; i < rooms.size(); i++) {
        ROOM room = rooms.at(i);

        SDL_Rect r1;
        r1.x = room.point.x;
        r1.y = room.point.y;
        r1.h = room.height + 1;
        r1.w = room.width + 1;

        SDL_Rect r2;
        r2.x = x;
        r2.y = y;
        r2.h = height + 1;
        r2.w = width + 1;

        if (SDL_HasIntersection(&r1, &r2)) {
            overlaps = true;
            break;
        }
    }

    return overlaps;
}

roomVector getRooms(int row, int col) {
    roomVector rooms;
    const int retryCount = 50;
    const int roomExtraSize = 1;
    
     for (int i = 0; i < retryCount; i++) {
        int size = (random(2 + roomExtraSize) + 1) * 2 + 1;
        int rectangularity = random(1 + size / 2) * 2;
        int width = size;
        int height = size;
        if (random(2)) {
            width += rectangularity;
        } else {
            height += rectangularity;
        }

        int x = random((col - width) / 2) * 2 + 1;
        int y = random((row - height) / 2 ) * 2 + 1;

        width = x + width >= col ? col - 1 : width; 
        height = y + height >= row ? row - 1 : height;

        if (x < 0 || y < 0) {
            continue;
        }

        ROOM room;
        room.point.x = x;
        room.point.y = y;
        room.height = height;
        room.width = width;

        if (checkIfRechanglesHaveIntersection(x, y, height, width, rooms) == false) {
            rooms.push_back(room);
        }
     }

     return rooms;
}

std::vector<POINT> getRemainedPoints(int row, int col, roomVector rooms) {
    std::vector<POINT> remainedPoints;
    for (int i = 0; i < col; i++) {
        for (int j = 0; j < row; j++) {
            if (isItInsideOfRooms(i, j, rooms) == false) {
                POINT point;
                point.x = i;
                point.y = j;
                remainedPoints.push_back(point);
            }
        }
    }

    return remainedPoints;
}

std::map<std::string, bool> prepareRemainedPointsMap(std::vector<POINT> remainedPoints) {
    std::map<std::string, bool> remainedPointsMap;
    for (int i = 0; i < remainedPoints.size(); i++) {
        POINT point = remainedPoints.at(i);
        std::string xyKey = std::to_string(point.x) + ":" + std::to_string(point.y);

        remainedPointsMap[xyKey] = true;
    }

    return remainedPointsMap;
}

void prepareMazeSpaces(elementVector elements, spacesMap *spaces) {
    for (elementVector::iterator it = elements.begin(); it != elements.end(); ++it) {
        std::string xyKey = std::to_string(it->x) + ":" + std::to_string(it->y);
        std::string ztKey = std::to_string(it->z) + ":" + std::to_string(it->t);
        int xyDirection = it->direction;
        int ztDirection = ~it->direction;

        (*spaces)[xyKey][xyDirection] = true;
        (*spaces)[ztKey][ztDirection] = true;
    }
}

void prepareRoomSpaces(roomVector rooms, spacesMap *spaces) {
    for (roomVector::iterator it = rooms.begin(); it != rooms.end(); ++it) {
        int x = it->point.x;
        int y = it->point.y;
        int height = it->height;
        int width = it->width;

        elementVector roomPassages;

        for (int i = x; i < x + width; i++) {
            for (int j = y; j < y + height; j++) {
                std::string xyKey = std::to_string(i) + ":" + std::to_string(j);

                if (i > x && i < x + width - 1 && j > y && j < y + height - 1) {
                    (*spaces)[xyKey][UP] = true;
                    (*spaces)[xyKey][DOWN] = true;
                    (*spaces)[xyKey][RIGHT] = true;
                    (*spaces)[xyKey][LEFT] = true;
                } else if (i == x && j == y) {
                    (*spaces)[xyKey][DOWN] = true;
                    (*spaces)[xyKey][RIGHT] = true;

                    ELEMENT element1;
                    element1.x = i;
                    element1.y = j;
                    element1.direction = LEFT;
                    element1.z = i - 1;
                    element1.t = j;

                    ELEMENT element2;
                    element2.x = i;
                    element2.y = j;
                    element2.direction = UP;
                    element2.z = i;
                    element2.t = j - 1;

                    roomPassages.push_back(element1);
                    roomPassages.push_back(element2);
                } else if (i == (x + width - 1) && j == (y + height - 1)) {
                    (*spaces)[xyKey][UP] = true;
                    (*spaces)[xyKey][LEFT] = true;

                    ELEMENT element1;
                    element1.x = i;
                    element1.y = j;
                    element1.direction = RIGHT;
                    element1.z = i + 1;
                    element1.t = j;

                    ELEMENT element2;
                    element2.x = i;
                    element2.y = j;
                    element2.direction = DOWN;
                    element2.z = i;
                    element2.t = j + 1;

                    roomPassages.push_back(element1);
                    roomPassages.push_back(element2);
                } else if (i == x && j == (y + height - 1)) {
                    (*spaces)[xyKey][UP] = true;
                    (*spaces)[xyKey][RIGHT] = true;

                    ELEMENT element1;
                    element1.x = i;
                    element1.y = j;
                    element1.direction = LEFT;
                    element1.z = i - 1;
                    element1.t = j;

                    ELEMENT element2;
                    element2.x = i;
                    element2.y = j;
                    element2.direction = DOWN;
                    element2.z = i;
                    element2.t = j + 1;

                    roomPassages.push_back(element1);
                    roomPassages.push_back(element2);
                } else if (i == (x + width - 1) && j == y) {
                    (*spaces)[xyKey][DOWN] = true;
                    (*spaces)[xyKey][LEFT] = true;

                    ELEMENT element1;
                    element1.x = i;
                    element1.y = j;
                    element1.direction = RIGHT;
                    element1.z = i + 1;
                    element1.t = j;

                    ELEMENT element2;
                    element2.x = i;
                    element2.y = j;
                    element2.direction = UP;
                    element2.z = i;
                    element2.t = j - 1;

                    roomPassages.push_back(element1);
                    roomPassages.push_back(element2);
                } else if (i > x && i < x + width - 1 && j == y) {
                    (*spaces)[xyKey][LEFT] = true;
                    (*spaces)[xyKey][RIGHT] = true;
                    (*spaces)[xyKey][DOWN] = true;

                    ELEMENT element1;
                    element1.x = i;
                    element1.y = j;
                    element1.direction = UP;
                    element1.z = i;
                    element1.t = j - 1;

                    roomPassages.push_back(element1);
                } else if (j > y && j < y + height - 1 && i == x) {
                    (*spaces)[xyKey][UP] = true;
                    (*spaces)[xyKey][DOWN] = true;
                    (*spaces)[xyKey][RIGHT] = true;

                    ELEMENT element1;
                    element1.x = i;
                    element1.y = j;
                    element1.direction = LEFT;
                    element1.z = i - 1;
                    element1.t = j;

                    roomPassages.push_back(element1);
                } else if (j == y + height - 1 && i > x && i < x + width - 1) {
                    (*spaces)[xyKey][UP] = true;
                    (*spaces)[xyKey][RIGHT] = true;
                    (*spaces)[xyKey][LEFT] = true;

                    ELEMENT element1;
                    element1.x = i;
                    element1.y = j;
                    element1.direction = DOWN;
                    element1.z = i;
                    element1.t = j + 1;

                    roomPassages.push_back(element1);
                } else if (i == x + width - 1 && j > y && j < y + height - 1) {
                    (*spaces)[xyKey][UP] = true;
                    (*spaces)[xyKey][DOWN] = true;
                    (*spaces)[xyKey][LEFT] = true;

                    ELEMENT element1;
                    element1.x = i;
                    element1.y = j;
                    element1.direction = RIGHT;
                    element1.z = i + 1;
                    element1.t = j;

                    roomPassages.push_back(element1);
                }
            }
        }

        int randomPassageIndex = random(roomPassages.size());

        if (randomPassageIndex > -1) {
            ELEMENT chosenPassage = roomPassages.at(randomPassageIndex);
        

            std::string xyKey = std::to_string(chosenPassage.x) + ":" + std::to_string(chosenPassage.y);
            std::string ztKey = std::to_string(chosenPassage.z) + ":" + std::to_string(chosenPassage.t);

            (*spaces)[xyKey][chosenPassage.direction] = true;
            (*spaces)[ztKey][~chosenPassage.direction] = true;
        }
    }
}
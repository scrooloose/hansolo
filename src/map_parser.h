#ifndef MAP_PARSER_H
#define MAP_PARSER_H

#include <string>
#include <iostream>
#include <fstream>
#include "map.h"
#include "tile.h"
#include "floor.h"
#include "wall.h"
#include "path.h"
#include "stairs.h"
#include "door.h"
#include "empty_tile.h"
#include "map_parsing_exception.h"

#include "mobile.h"
#include "zombie.h"

#include "item.h"

using namespace std;

class MapParser {
    private:
        string* fname;

        ifstream* open_map_file();

    public:
        MapParser(string* fname);
        ~MapParser();

        static Map* parse_file(string* fname);

        Map* parse();
        Tile* tile_for(char c, Position* p);
        Positionable* mobile_for(char c, Position* p, Map* m);
        int find_width();
        int find_height();


};

#endif

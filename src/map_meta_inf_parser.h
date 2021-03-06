#ifndef MAP_META_INF_PARSER_H
#define MAP_META_INF_PARSER_H

#include <fstream>
#include <vector>
#include <cstdio>
#include <boost/filesystem.hpp>

#include "message_log.h"
#include "map_parsing_exception.h"
#include "map.h"
#include "position.h"

#include "json/json.h"

#include "item.h"
#include "key.h"
#include "tiles/door.h"
#include "cheese.h"

#include "weapon_factory.h"

#include "mobile.h"
#include "mobile_factory.h"

#include "cutscene.h"

using namespace std;

class MapMetaInfParser {
    private:
        string fname;
        string map_dir;
        Map* map;
        string get_file_contents();

        vector<Mobile*> mobiles;
        string map_name;
        Position* start_position;
        vector<Item*> items;
        vector<Door*> locked_doors;
        vector<Cutscene*> cutscenes;

        void parse_mobiles(Json::Value root);
        void parse_map_name(Json::Value root);
        void parse_start_position(Json::Value root);
        void parse_items(Json::Value root);
        void parse_keys(Json::Value root);
        void parse_locked_doors(Json::Value root);
        void parse_cutscenes(Json::Value root);
        vector<string*> parse_cutscene_pages(Json::Value cutscene_root);

        Mobile* mob_for(string type, int x, int y);
        Item* item_for(string type, int quantity, int xpos, int ypos);

    public:
        MapMetaInfParser(string fname, Map* map);
        ~MapMetaInfParser();
        void parse();

        vector<Mobile*> get_mobiles();
        vector<Item*> get_items();
        vector<Door*> get_locked_doors();
        string get_map_name();
        const Position& get_start_position();

        vector<Cutscene*> get_cutscenes();
};

#endif

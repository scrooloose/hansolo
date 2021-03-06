#include "map_meta_inf_parser.h"

MapMetaInfParser::MapMetaInfParser(string fname, Map* map) {
    this->fname = fname;
    this->map = map;
    boost::filesystem::path p1(fname);
    this->map_dir = p1.parent_path().native();
}

MapMetaInfParser::~MapMetaInfParser() {
    delete start_position;
}

string MapMetaInfParser::get_file_contents() {
    ifstream inf_file(fname.c_str());

    if (!inf_file.is_open()) {
        throw MapParsingException("Meta inf file: '" + fname + "' doesn't exist.");
    }

    string file_contents, tmp;
    while (!inf_file.eof()) {
        getline(inf_file, tmp);
        file_contents += tmp;
        file_contents += "\n";
    }

    return file_contents;
}

void MapMetaInfParser::parse() {
    Json::Value root;
    Json::Reader reader;
    bool success = reader.parse(get_file_contents(), root);
    if (!success) {
        throw MapParsingException("Failed to parse '" + fname + "' as json");
    }

    parse_mobiles(root);
    parse_map_name(root);
    parse_start_position(root);
    parse_items(root);
    parse_keys(root);
    parse_locked_doors(root);
    parse_cutscenes(root);
}

void MapMetaInfParser::parse_mobiles(Json::Value root) {
    Json::Value mobiles = root["mobiles"];

    for (unsigned i = 0; i < mobiles.size(); i++) {
        Json::Value current = mobiles[i];

        string type = current["type"].asString();
        int xpos  = current["x"].asInt();
        int ypos  = current["y"].asInt();

        this->mobiles.push_back(mob_for(type, xpos, ypos));
    }
}

Mobile* MapMetaInfParser::mob_for(string type, int xpos, int ypos) {
    if (type == "zombie")
        return MobileFactory::build_zombie(map, Position(xpos, ypos));
    else if (type == "giant_spider")
        return MobileFactory::build_giant_spider(map, Position(xpos, ypos));
    else if (type == "rat")
        return MobileFactory::build_rat(map, Position(xpos, ypos));
    else
        throw MapParsingException("Unrecognised mobile type:" + type);
}

void MapMetaInfParser::parse_map_name(Json::Value root) {
    Json::Value val = root["name"];
    this->map_name = val.asString();
    if (map_name.empty()) {
        throw MapParsingException("Map name missing in: '" + fname + "'");
    }
}

void MapMetaInfParser::parse_start_position(Json::Value root) {
    Json::Value pos = root["start_position"];
    if (pos["x"].empty() || pos["y"].empty()) {
        throw MapParsingException("Invalid start_position in: '" + fname + "'");
    }

    this->start_position = new Position(pos["x"].asInt(), pos["y"].asInt());
}

void MapMetaInfParser::parse_items(Json::Value root) {
    Json::Value items = root["items"];

    for (unsigned i = 0; i < items.size(); i++) {
        Json::Value current = items[i];

        string type = current["type"].asString();
        int quantity  = current["quantity"].asInt();
        int xpos  = current["x"].asInt();
        int ypos  = current["y"].asInt();

        this->items.push_back(item_for(type, quantity, xpos, ypos));
    }
}

void MapMetaInfParser::parse_keys(Json::Value root) {
    Json::Value keys = root["keys"];

    for (unsigned i = 0; i < keys.size(); i++) {
        Json::Value current = keys[i];

        int door_id  = current["door_id"].asInt();
        string name = current["name"].asString();
        int xpos  = current["x"].asInt();
        int ypos  = current["y"].asInt();

        this->items.push_back(new Key(door_id, name, Position(xpos, ypos)));
    }
}

void MapMetaInfParser::parse_locked_doors(Json::Value root) {
    Json::Value doors = root["locked-doors"];
    for (unsigned i = 0; i < doors.size(); i++) {
        Json::Value current = doors[i];

        int door_id  = current["door_id"].asInt();
        string name = current["name"].asString();
        int xpos  = current["x"].asInt();
        int ypos  = current["y"].asInt();

        this->locked_doors.push_back(new Door(Position(xpos, ypos), door_id, name));
    }
}

void MapMetaInfParser::parse_cutscenes(Json::Value root) {
    Json::Value cutscenes = root["cutscenes"];

    for (unsigned i = 0; i < cutscenes.size(); i++) {
        Json::Value current = cutscenes[i];

        int xpos  = current["x"].asInt();
        int ypos  = current["y"].asInt();

        Cutscene* next_cutscene = new Cutscene(
            Position(xpos, ypos),
            parse_cutscene_pages(current)
        );

        this->cutscenes.push_back(next_cutscene);
    }
}

vector<string*> MapMetaInfParser::parse_cutscene_pages(Json::Value cutscene_root) {
    vector<string*> result;

    Json::Value pages = cutscene_root["pages"];

    for (unsigned i = 0; i < pages.size(); i++) {
        string fname = pages[i].asString();
        ifstream message_file(this->map_dir + "/" + fname);

        if (!message_file.is_open()) {
            throw MapParsingException("Couldnt open story file:" + fname);
        }

        string line;
        string page;
        while (!message_file.eof()) {
            getline(message_file, line);
            page += line;
            page += "\n";
        }

        result.push_back(new string(page));
    }

    return result;
}

Item* MapMetaInfParser::item_for(string type, int quantity, int xpos, int ypos) {
    Position pos(xpos, ypos);

    if (type == "9mm_round")
        return new Item("9mm round", type, quantity, "=", pos);
    else if (type == "shotgun_shell")
        return new Item("Shotgun Shell", type, quantity, "=", pos);
    else if (type == "cheese")
        return new Cheese(pos);
    else if (type == "pistol")
        return WeaponFactory::build_pistol(map, pos);
    else if (type == "shotgun")
        return WeaponFactory::build_shotgun(map, pos);
    else if (type == "p90")
        return WeaponFactory::build_p90(map, pos);
    else if (type == "drawing_pin")
        return WeaponFactory::build_drawing_pin(map, pos);
    else
        throw MapParsingException("Unrecognised item type:" + type);

}

string MapMetaInfParser::get_map_name() {
    return map_name;
}

vector<Mobile*> MapMetaInfParser::get_mobiles() {
    return mobiles;
}

vector<Item*> MapMetaInfParser::get_items() {
    return items;
}

vector<Door*> MapMetaInfParser::get_locked_doors() {
    return locked_doors;
}

const Position& MapMetaInfParser::get_start_position() {
    return *start_position;
}

vector<Cutscene*> MapMetaInfParser::get_cutscenes() {
    return cutscenes;
}

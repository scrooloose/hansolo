#ifndef TILE_H
#define TILE_H

#include "position.h"
#include "renderable.h"
#include <string>

using namespace std;

class Tile : public Renderable {
    private:
        Position position;
        bool visible;

    public:
        Tile(Position p);
        ~Tile();

        const Position& get_pos();
        virtual bool is_walkable() = 0;
        virtual bool is_descendable();

        virtual bool is_openable();
        virtual void open();

        bool is_visible();
        void set_visiblity(bool visibile);

        virtual bool is_transparent();
};

#endif

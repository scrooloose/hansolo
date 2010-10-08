#include "lead_pipe.h"

LeadPipe::LeadPipe(Positionable* player, Map* map) : MeleeWeapon(player, map) {
}

int LeadPipe::get_dmg_dice() {
    return 3;
}

int LeadPipe::get_dmg_dice_sides() {
    return 3;
}

int LeadPipe::get_dmg_modifier() {
    return 1;
}

string LeadPipe::get_ammo_type() {
    return "";
}

string LeadPipe::get_name() {
    return "Lead pipe";
}


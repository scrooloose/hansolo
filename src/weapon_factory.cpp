#include "weapon_factory.h"

MeleeWeapon* WeaponFactory::build_lead_pipe(Map* m, const Position& p) {
    SimpleDamageStrategy* dmg_strategy = new SimpleDamageStrategy(m);
    dmg_strategy->set_dmg_dice(3, 3, 1);

    MeleeWeapon* lead_pipe = new MeleeWeapon(m, "Lead pipe", "lead_pipe", p);
    lead_pipe->set_dmg_strategy(dmg_strategy);
    return lead_pipe;
}

RangedWeapon* WeaponFactory::build_pistol(Map* m, const Position& p) {
    SimpleDamageStrategy* dmg_strategy = new SimpleDamageStrategy(m);
    dmg_strategy->set_dmg_dice(3, 5, 2);

    RangedWeapon* pistol = new RangedWeapon(m, "Pistol", "pistol", p);
    pistol->set_min_range(2);
    pistol->set_max_range(8);
    pistol->set_ammo_type("9mm_round");
    pistol->set_ammo_per_turn(1);
    pistol->set_dmg_strategy(dmg_strategy);
    return pistol;
}

RangedWeapon* WeaponFactory::build_p90(Map* m, const Position& p) {
    SimpleDamageStrategy* dmg_strategy = new SimpleDamageStrategy(m);
    dmg_strategy->set_dmg_dice(7, 4, 2);

    RangedWeapon* p90 = new RangedWeapon(m, "P90", "p90", p);
    p90->set_min_range(2);
    p90->set_max_range(7);
    p90->set_ammo_type("9mm_round");
    p90->set_ammo_per_turn(2);
    p90->set_dmg_strategy(dmg_strategy);
    return p90;
}

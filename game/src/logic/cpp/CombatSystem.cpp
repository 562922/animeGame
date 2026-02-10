#include "CombatSystem.h"

int CombatSystem::calculateDamage(const Player &attacker, const Enemy &defender) {
  int base = attacker.level * 2 + (attacker.maxHP / 50) + attacker.attack;
  int def = defender.defense;
  int dmg = base - def;
  if (dmg < 1) dmg = 1;
  return dmg;
}

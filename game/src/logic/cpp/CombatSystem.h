#pragma once
#include "Player.h"
#include "Enemy.h"

class CombatSystem {
public:
  static int calculateDamage(const Player &attacker, const Enemy &defender);
};

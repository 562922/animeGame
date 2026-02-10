#pragma once
#include <string>

struct NPC {
  int npcID = 0;
  std::string npcName;
  std::string npcType;
  float X = 0;
  float Y = 0;
  int mapID = 0;
};

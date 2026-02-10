#pragma once
#include <string>
#include <vector>

struct Enemy {
  std::string enemyID;
  std::string enemyName;
  int level = 1;
  int maxHP = 100;
  int currentHP = 100;
  int attack = 10;
  int defense = 5;
  float experienceReward = 100.0f;
  bool isBoss = false;
  float posX = 0.0f;
  float posY = 0.0f;
  std::vector<std::string> lootDrops;
};

struct NPC {
  std::string npcID;
  std::string npcName;
  std::string npcType;  // "Merchant", "Quest Giver", "Information Broker", etc.
  float posX = 0.0f;
  float posY = 0.0f;
  std::string currentMap = "town_of_beginnings";
};

struct Map {
  std::string mapID;
  std::string mapName;
  std::string description;
  float width = 1024.0f;
  float height = 1024.0f;
  std::vector<std::string> connectedMaps;
};

#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <map>

// Item struct
struct Item {
  std::string itemID;
  std::string name;
  std::string description;
  int rarity; // 1=common, 2=uncommon, 3=rare, 4=epic, 5=legendary
  int level;
  int quantity = 1;
  
  // Combat stats
  int damageBonus = 0;
  int defenseBonus = 0;
  int hpBonus = 0;
  int manaBonus = 0;
  
  // Item type
  std::string type; // weapon, armor, accessory, consumable, quest
  std::string slot; // right_hand, left_hand, head, chest, legs, feet, accessory1, accessory2
};

// Equipment slot manager
struct EquipmentSlots {
  Item* rightHand = nullptr;
  Item* leftHand = nullptr;
  Item* head = nullptr;
  Item* chest = nullptr;
  Item* legs = nullptr;
  Item* feet = nullptr;
  Item* accessory1 = nullptr;
  Item* accessory2 = nullptr;
};

// Quest objective
struct QuestObjective {
  std::string objectiveID;
  std::string description;
  int targetCount = 1;
  int currentCount = 0;
  bool completed = false;
};

// Quest definition
struct QuestData {
  std::string questID;
  std::string title;
  std::string description;
  int rewardXP = 0;
  int rewardGold = 0;
  std::vector<Item> rewardItems;
  int minLevel = 1;
  std::vector<QuestObjective> objectives;
  std::string giver; // NPC ID
};

// Player quest progress
struct PlayerQuest {
  std::string questID;
  int status; // 0=not_started, 1=in_progress, 2=completed, 3=failed
  std::vector<QuestObjective> objectives;
  long long acceptedAt = 0;
  long long completedAt = 0;
};

// Complete Player struct
struct Player {
  // Identity
  std::string playerID;
  std::string displayName;
  int accountLevel = 1;
  
  // Stats
  int level = 1;
  int experience = 0;
  int nextLevelEXP = 1000;
  
  // Health & Mana
  int maxHP = 100;
  int currentHP = 100;
  int maxMana = 50;
  int currentMana = 50;
  int maxStamina = 100;
  int currentStamina = 100;
  
  // Attributes
  int strength = 10;
  int agility = 10;
  int intelligence = 10;
  int vitality = 10;
  int dexterity = 10;
  
  // Derived stats
  int attack = 20;
  int defense = 10;
  int magicAttack = 15;
  int magicDefense = 10;
  int critChance = 5; // percentage
  int evasion = 5; // percentage
  
  // Position
  double posX = 0;
  double posY = 0;
  std::string currentMap = "town_of_beginnings";
  
  // Inventory
  int gold = 0;
  int maxInventorySlots = 30;
  std::vector<Item> inventory;
  EquipmentSlots equipment;
  
  // Quests
  std::vector<PlayerQuest> activeQuests;
  std::vector<PlayerQuest> completedQuests;
  
  // Skills/abilities
  std::vector<std::string> learnedSkills;
  std::map<std::string, long long> skillCooldowns; // skillID -> timestamp
  
  // Status effects
  std::vector<std::string> statusEffects;
  std::map<std::string, int> statusDurations;
  
  // Combat state
  bool inCombat = false;
  std::string currentTargetID;
  
  // Timestamps
  long long createdAt = 0;
  long long lastSavedAt = 0;
  long long lastLogoutAt = 0;
};

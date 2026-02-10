#include "GameLogic.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <chrono>

GameLogic::GameLogic(const std::string &assetsRoot) 
  : assetsRoot(assetsRoot), gameRunning(false), currentMapID("town_of_beginnings") {
  
  // Initialize managers
  uiManager = std::make_unique<UIManager>();
  inputHandler = std::make_unique<InputHandler>();
  saveSystem = std::make_unique<SaveSystem>("./saves/");
  dialogueSystem = std::make_unique<DialogueSystem>();
  skillSystem = std::make_unique<SkillSystem>();
}

GameLogic::~GameLogic() {}

bool GameLogic::initializeNewGame(const std::string& playerName) {
  // Initialize player
  player.playerID = "player_1";
  player.displayName = playerName;
  player.accountLevel = 1;
  player.level = 1;
  player.experience = 0;
  player.nextLevelEXP = 1000;
  player.maxHP = 100;
  player.currentHP = 100;
  player.maxMana = 50;
  player.currentMana = 50;
  player.gold = 0;
  player.posX = 100.0;
  player.posY = 200.0;
  player.currentMap = "town_of_beginnings";
  player.createdAt = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
  
  // Learn starting skills
  skillSystem->learnSkill("slash");
  skillSystem->learnSkill("fireball");
  
  // Load initial assets
  loadAssets();
  
  // Update UI
  uiManager->setGameState(GameState::IN_GAME);
  HUDData hud;
  hud.playerHP = player.currentHP;
  hud.playerMaxHP = player.maxHP;
  hud.playerLevel = player.level;
  hud.playerExperience = player.experience;
  hud.nextLevelEXP = player.nextLevelEXP;
  hud.gold = player.gold;
  hud.currentMap = player.currentMap;
  uiManager->updateHUD(hud);
  
  return true;
}

bool GameLogic::loadGame(const std::string& saveSlot) {
  // Load player from save file
  if (!saveSystem->loadPlayer(player, saveSlot)) {
    return false;
  }
  
  loadAssets();
  gameRunning = true;
  uiManager->setGameState(GameState::IN_GAME);
  
  return true;
}

bool GameLogic::loadAssets() {
  // Load skill definitions
  skillSystem->loadSkillsFromAssets(assetsRoot);
  
  // Load enemies from JSON
  std::string efile = assetsRoot + "/PLAY/enemies.json";
  std::ifstream ef(efile);
  if (ef.is_open()) {
    std::string content((std::istreambuf_iterator<char>(ef)), std::istreambuf_iterator<char>());
    ef.close();
    // Basic extraction of enemy data (in production use nlohmann::json)
    // For now, hard-code standard first-floor enemies
    Enemy boar, nepent, kobold, illfang;
    boar.enemyID = "frenzy_boar";
    boar.enemyName = "Frenzy Boar";
    boar.maxHP = 30;
    boar.attack = 6;
    boar.defense = 2;
    boar.level = 1;
    
    nepent.enemyID = "little_nepent";
    nepent.enemyName = "Little Nepent";
    nepent.maxHP = 20;
    nepent.attack = 4;
    nepent.defense = 1;
    nepent.level = 1;
    
    kobold.enemyID = "ruin_kobold";
    kobold.enemyName = "Ruin Kobold Trooper";
    kobold.maxHP = 45;
    kobold.attack = 9;
    kobold.defense = 3;
    kobold.level = 3;
    
    illfang.enemyID = "illfang";
    illfang.enemyName = "Illfang the Kobold Lord";
    illfang.maxHP = 500;
    illfang.attack = 30;
    illfang.defense = 15;
    illfang.level = 10;
    illfang.isBoss = true;
    
    enemies.push_back(boar);
    enemies.push_back(nepent);
    enemies.push_back(kobold);
    enemies.push_back(illfang);
    
    std::cout << "Loaded 4 enemy types from assets" << std::endl;
  }
  
  // Load NPCs from JSON
  std::string nfile = assetsRoot + "/NPC/npcs.json";
  std::ifstream nf(nfile);
  if (nf.is_open()) {
    std::string content((std::istreambuf_iterator<char>(nf)), std::istreambuf_iterator<char>());
    nf.close();
    // Hard-code SAO first-floor NPCs
    NPC merchant1, merchant2, quest1, argo;
    
    merchant1.npcID = "merchant_bobby";
    merchant1.npcName = "Merchant Bobby";
    merchant1.npcType = "Merchant";
    merchant1.posX = 520;
    merchant1.posY = 420;
    
    merchant2.npcID = "merchant_yahiko";
    merchant2.npcName = "Merchant Yahiko";
    merchant2.npcType = "Merchant";
    merchant2.posX = 480;
    merchant2.posY = 420;
    
    quest1.npcID = "jinhiro";
    quest1.npcName = "Jinhiro";
    quest1.npcType = "Quest Giver";
    quest1.posX = 495;
    quest1.posY = 365;
    
    argo.npcID = "argo";
    argo.npcName = "Argo the Rat";
    argo.npcType = "Information Broker";
    argo.posX = 560;
    argo.posY = 380;
    
    npcs.push_back(merchant1);
    npcs.push_back(merchant2);
    npcs.push_back(quest1);
    npcs.push_back(argo);
    
    std::cout << "Loaded 4 NPCs from assets" << std::endl;
  }
  
  // Load maps from JSON  
  std::string mfile = assetsRoot + "/PLAY/maps.json";
  std::ifstream mf(mfile);
  if (mf.is_open()) {
    std::string content((std::istreambuf_iterator<char>(mf)), std::istreambuf_iterator<char>());
    mf.close();
    // Hard-code SAO first-floor maps
    Map town, tolbana;
    
    town.mapID = "town_of_beginnings";
    town.mapName = "Town of Beginnings";
    town.width = 1200;
    town.height = 1200;
    town.description = "The main settlement on the 1st Floor";
    
    tolbana.mapID = "tolbana";
    tolbana.mapName = "Tolbana Town";
    tolbana.width = 800;
    tolbana.height = 800;
    tolbana.description = "The second largest town on the 1st Floor";
    
    maps[town.mapID] = town;
    maps[tolbana.mapID] = tolbana;
    
    std::cout << "Loaded 2 maps from assets" << std::endl;
  }
  
  return true;
}

void GameLogic::update(float dt) {
  if (!gameRunning) return;
  
  // Update timing
  currentTimeMS += static_cast<long long>(dt * 1000.0f);
  accumulatedDeltaTime += dt;
  
  // Update HUD
  HUDData hud;
  hud.playerHP = player.currentHP;
  hud.playerMaxHP = player.maxHP;
  hud.playerMana = player.currentMana;
  hud.playerMaxMana = player.maxMana;
  hud.playerLevel = player.level;
  hud.playerExperience = player.experience;
  hud.nextLevelEXP = player.nextLevelEXP;
  hud.gold = player.gold;
  hud.currentMap = player.currentMap;
  uiManager->updateHUD(hud);
  
  // Update skills cooldowns
  auto skills = skillSystem->getLearnedSkills();
  for (const auto& skill : skills) {
    skillSystem->getRemainingCooldown(skill.skillID, currentTimeMS);
  }
}

void GameLogic::handleInput(const std::vector<PlayerCommand>& commands) {
  if (!gameRunning) return;
  
  for (const auto& cmd : commands) {
    switch (cmd.action) {
      case InputAction::MOVE_FORWARD:
        if (cmd.pressed) movePlayer(0, cmd.value * 50.0f);
        break;
      case InputAction::MOVE_BACKWARD:
        if (cmd.pressed) movePlayer(0, -cmd.value * 50.0f);
        break;
      case InputAction::MOVE_LEFT:
        if (cmd.pressed) movePlayer(-cmd.value * 50.0f, 0);
        break;
      case InputAction::MOVE_RIGHT:
        if (cmd.pressed) movePlayer(cmd.value * 50.0f, 0);
        break;
      case InputAction::INTERACT:
        if (cmd.pressed) {
          // TODO: Find nearest NPC and interact
        }
        break;
      case InputAction::ATTACK:
        if (cmd.pressed) {
          // TODO: Perform attack on nearest enemy
        }
        break;
      case InputAction::SKILL_1:
        if (cmd.pressed) useSkill("slash", "");
        break;
      case InputAction::SKILL_2:
        if (cmd.pressed) useSkill("fireball", "");
        break;
      case InputAction::OPEN_INVENTORY:
        if (cmd.pressed) uiManager->showPanel(UIPanel::INVENTORY);
        break;
      case InputAction::OPEN_STATS:
        if (cmd.pressed) uiManager->showPanel(UIPanel::STATS);
        break;
      case InputAction::OPEN_QUESTS:
        if (cmd.pressed) uiManager->showPanel(UIPanel::QUESTS);
        break;
      case InputAction::PAUSE:
        if (cmd.pressed) {
          if (uiManager->isPaused()) {
            resumeGame();
          } else {
            pauseGame();
          }
        }
        break;
      default:
        break;
    }
  }
}

void GameLogic::startGame() {
  gameRunning = true;
  uiManager->setGameState(GameState::IN_GAME);
}

void GameLogic::pauseGame() {
  gameRunning = false;
  uiManager->pauseGame();
}

void GameLogic::resumeGame() {
  gameRunning = true;
  uiManager->resumeGame();
}

void GameLogic::exitGame() {
  gameRunning = false;
  quickSave();
}

bool GameLogic::isRunning() const {
  return gameRunning;
}

void GameLogic::movePlayer(float dx, float dy) {
  player.posX += dx;
  player.posY += dy;
}

void GameLogic::teleportPlayer(const std::string& mapID, double x, double y) {
  player.currentMap = mapID;
  player.posX = x;
  player.posY = y;
  currentMapID = mapID;
}

void GameLogic::attackEnemy(const std::string& enemyID) {
  // TODO: Implement combat logic
  inCombat = true;
  activeCombatEnemyID = enemyID;
}

void GameLogic::useSkill(const std::string& skillID, const std::string& targetID) {
  if (skillSystem->canUseSkill(skillID, currentTimeMS)) {
    skillSystem->useSkill(skillID, currentTimeMS);
    uiManager->addChatMessage("Used skill: " + skillID);
  }
}

void GameLogic::interactWithNPC(const std::string& npcID) {
  dialogueSystem->startDialogue(npcID);
  uiManager->showPanel(UIPanel::DIALOGUE);
}

void GameLogic::selectDialogueChoice(const std::string& npcID, const std::string& choiceID) {
  dialogueSystem->selectChoice(npcID, choiceID);
}

void GameLogic::acceptQuest(const std::string& questID, const std::string& npcID) {
  PlayerQuest pq;
  pq.questID = questID;
  pq.status = 1; // in_progress
  pq.acceptedAt = currentTimeMS;
  player.activeQuests.push_back(pq);
  uiManager->addChatMessage("Quest accepted: " + questID);
}

void GameLogic::completeQuest(const std::string& questID) {
  for (auto& pq : player.activeQuests) {
    if (pq.questID == questID) {
      pq.status = 2; // completed
      pq.completedAt = currentTimeMS;
      player.completedQuests.push_back(pq);
      uiManager->addChatMessage("Quest completed: " + questID);
      break;
    }
  }
}

bool GameLogic::quickSave() {
  return saveSystem->savePlayer(player, "quicksave");
}

bool GameLogic::quickLoad() {
  return loadGame("quicksave");
}

Enemy GameLogic::getEnemy() const {
  if (enemies.size() > 0) {
    return enemies[0];
  }
  return Enemy();
}

std::vector<Item> GameLogic::getAvailableItems() const {
  std::vector<Item> items;
  for (const auto& pair : itemDatabase) {
    items.push_back(pair.second);
  }
  return items;
}

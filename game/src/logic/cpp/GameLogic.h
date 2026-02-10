#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Player.h"
#include "Enemy.h"
#include "UIManager.h"
#include "InputHandler.h"
#include "SaveSystem.h"
#include "DialogueSystem.h"
#include "SkillSystem.h"

class GameLogic {
public:
  GameLogic(const std::string &assetsRoot);
  ~GameLogic();
  
  // Initialization
  bool loadAssets();
  bool initializeNewGame(const std::string& playerName);
  bool loadGame(const std::string& saveSlot);
  
  // Main game loop
  void update(float dt);
  void handleInput(const std::vector<PlayerCommand>& commands);
  
  // Game state
  void startGame();
  void pauseGame();
  void resumeGame();
  void exitGame();
  bool isRunning() const;
  
  // Player management
  Player getPlayer() const { return player; }
  void movePlayer(float dx, float dy);
  void teleportPlayer(const std::string& mapID, double x, double y);
  
  // Combat
  void attackEnemy(const std::string& enemyID);
  void useSkill(const std::string& skillID, const std::string& targetID);
  
  // Interaction
  void interactWithNPC(const std::string& npcID);
  void selectDialogueChoice(const std::string& npcID, const std::string& choiceID);
  void acceptQuest(const std::string& questID, const std::string& npcID);
  void completeQuest(const std::string& questID);
  
  // UI
  UIManager* getUIManager() { return uiManager.get(); }
  
  // Save/Load
  bool quickSave();
  bool quickLoad();
  
  // Data accessors
  Enemy getEnemy() const;
  const std::vector<NPC>& getNPCs() const { return npcs; }
  const std::map<std::string, Map>& getMaps() const { return maps; }
  std::vector<Item> getAvailableItems() const;
  
private:
  std::string assetsRoot;
  bool gameRunning = false;
  
  // Game state
  Player player;
  std::vector<Enemy> enemies;
  std::vector<NPC> npcs;
  std::map<std::string, Map> maps;
  std::map<std::string, QuestData> questDatabase;
  std::map<std::string, Item> itemDatabase;
  
  // Managers
  std::unique_ptr<UIManager> uiManager;
  std::unique_ptr<InputHandler> inputHandler;
  std::unique_ptr<SaveSystem> saveSystem;
  std::unique_ptr<DialogueSystem> dialogueSystem;
  std::unique_ptr<SkillSystem> skillSystem;
  
  // Timing
  float accumulatedDeltaTime = 0.0f;
  long long currentTimeMS = 0;
  
  // World state
  std::string currentMapID;
  bool inCombat = false;
  std::string activeCombatEnemyID;
};

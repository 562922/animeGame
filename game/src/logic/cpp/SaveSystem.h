#pragma once
#include <string>
#include <vector>
#include "Player.h"

struct SaveFile {
  std::string saveID;
  std::string playerName;
  int playerLevel;
  std::string lastMap;
  long long savedAt;
  std::string version;
};

class SaveSystem {
public:
  SaveSystem(const std::string& savePath = "./saves/");
  
  // Save/Load
  bool savePlayer(const Player& player, const std::string& saveSlot = "autosave");
  bool loadPlayer(Player& player, const std::string& saveSlot);
  bool deletePlayerSave(const std::string& saveSlot);
  
  // List saves
  std::vector<SaveFile> listSaves();
  
  // World state
  bool saveWorldState(const std::string& mapID, const std::string& stateData);
  bool loadWorldState(const std::string& mapID, std::string& stateData);
  
  // Settings
  bool saveSettings(const std::string& settingsJSON);
  bool loadSettings(std::string& settingsJSON);
  
  // Validation
  bool isValidSaveFile(const std::string& saveSlot);
  
private:
  std::string savePath;
  std::string getSavePath(const std::string& saveSlot);
  std::string sanitizeSlotName(const std::string& slotName);
};

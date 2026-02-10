#include "SaveSystem.h"
#include <fstream>
#include <sstream>

SaveSystem::SaveSystem(const std::string& savePath) : savePath(savePath) {
  // Ensure save directory exists
  #ifdef _WIN32
    system(("mkdir " + savePath).c_str());
  #else
    system(("mkdir -p " + savePath).c_str());
  #endif
}

std::string SaveSystem::getSavePath(const std::string& saveSlot) {
  return savePath + sanitizeSlotName(saveSlot) + ".sav";
}

std::string SaveSystem::sanitizeSlotName(const std::string& slotName) {
  std::string result = slotName;
  for (char& c : result) {
    if (!std::isalnum(c)) c = '_';
  }
  return result;
}

bool SaveSystem::savePlayer(const Player& player, const std::string& saveSlot) {
  std::string path = getSavePath(saveSlot);
  std::ofstream file(path);
  if (!file.is_open()) {
    return false;
  }
  
  // Simple JSON-like format
  file << "{\n";
  file << "  \"playerID\": \"" << player.playerID << "\",\n";
  file << "  \"displayName\": \"" << player.displayName << "\",\n";
  file << "  \"level\": " << player.level << ",\n";
  file << "  \"experience\": " << player.experience << ",\n";
  file << "  \"maxHP\": " << player.maxHP << ",\n";
  file << "  \"currentHP\": " << player.currentHP << ",\n";
  file << "  \"gold\": " << player.gold << ",\n";
  file << "  \"posX\": " << player.posX << ",\n";
  file << "  \"posY\": " << player.posY << ",\n";
  file << "  \"currentMap\": \"" << player.currentMap << "\",\n";
  file << "  \"savedAt\": " << player.lastSavedAt << "\n";
  file << "}\n";
  file.close();
  
  return true;
}

bool SaveSystem::loadPlayer(Player& player, const std::string& saveSlot) {
  std::string path = getSavePath(saveSlot);
  std::ifstream file(path);
  if (!file.is_open()) {
    return false;
  }
  
  // Simple parsing (in production, use nlohmann/json)
  std::string line;
  while (std::getline(file, line)) {
    // Very basic parsing - just to demonstrate
    if (line.find("displayName") != std::string::npos) {
      size_t start = line.find("\"") + 1;
      size_t end = line.rfind("\"");
      if (start < end) {
        player.displayName = line.substr(start, end - start);
      }
    }
  }
  file.close();
  
  return true;
}

bool SaveSystem::deletePlayerSave(const std::string& saveSlot) {
  std::string path = getSavePath(saveSlot);
  return std::remove(path.c_str()) == 0;
}

std::vector<SaveFile> SaveSystem::listSaves() {
  std::vector<SaveFile> saves;
  // Implementation would list .sav files in savePath
  return saves;
}

bool SaveSystem::saveWorldState(const std::string& mapID, const std::string& stateData) {
  std::string path = savePath + "world_" + sanitizeSlotName(mapID) + ".sav";
  std::ofstream file(path);
  if (!file.is_open()) return false;
  file << stateData;
  file.close();
  return true;
}

bool SaveSystem::loadWorldState(const std::string& mapID, std::string& stateData) {
  std::string path = savePath + "world_" + sanitizeSlotName(mapID) + ".sav";
  std::ifstream file(path);
  if (!file.is_open()) return false;
  std::stringstream buffer;
  buffer << file.rdbuf();
  stateData = buffer.str();
  file.close();
  return true;
}

bool SaveSystem::saveSettings(const std::string& settingsJSON) {
  std::string path = savePath + "settings.sav";
  std::ofstream file(path);
  if (!file.is_open()) return false;
  file << settingsJSON;
  file.close();
  return true;
}

bool SaveSystem::loadSettings(std::string& settingsJSON) {
  std::string path = savePath + "settings.sav";
  std::ifstream file(path);
  if (!file.is_open()) return false;
  std::stringstream buffer;
  buffer << file.rdbuf();
  settingsJSON = buffer.str();
  file.close();
  return true;
}

bool SaveSystem::isValidSaveFile(const std::string& saveSlot) {
  std::string path = getSavePath(saveSlot);
  std::ifstream file(path);
  return file.good();
}

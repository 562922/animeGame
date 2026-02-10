#pragma once
#include <string>
#include <vector>
#include <map>

enum class GameState {
  LOGIN_MENU = 0,
  CHARACTER_CREATION = 1,
  MAIN_MENU = 2,
  IN_GAME = 3,
  PAUSE_MENU = 4,
  INVENTORY = 5,
  CHARACTER_STATS = 6,
  QUEST_LOG = 7,
  MAP = 8,
  SHOP = 9,
  LOADING = 10,
  CUTSCENE = 11
};

enum class UIPanel {
  NONE = 0,
  HUD = 1,
  INVENTORY = 2,
  STATS = 3,
  QUESTS = 4,
  MAP = 5,
  SHOP = 6,
  DIALOGUE = 7,
  SETTINGS = 8,
  PAUSE = 9,
  LOADING = 10
};

// UI Element base
struct UIElement {
  std::string elementID;
  bool visible = false;
  bool active = false;
  int posX = 0;
  int posY = 0;
  int width = 0;
  int height = 0;
};

// HUD display info
struct HUDData {
  int playerHP;
  int playerMaxHP;
  int playerMana;
  int playerMaxMana;
  int playerLevel;
  int playerExperience;
  int nextLevelEXP;
  int gold;
  std::string currentMap;
  std::vector<std::string> equippedSkills; // Quick bar
};

// Menu state
struct MenuState {
  GameState currentState;
  UIPanel activePanel;
  std::string selectedMenuItem;
  bool isPaused = false;
  bool showSettings = false;
  float uiScale = 1.0f;
  bool mouseoverMenu = false;
};

// UI Manager
class UIManager {
public:
  UIManager();
  
  // State management
  void setGameState(GameState newState);
  GameState getGameState() const { return menuState.currentState; }
  
  // Panel management
  void showPanel(UIPanel panel);
  void hidePanel(UIPanel panel);
  UIPanel getActivePanel() const { return menuState.activePanel; }
  
  // HUD updates
  void updateHUD(const HUDData& hudData);
  HUDData getHUDData() const { return hudData; }
  
  // Menu mouse/input
  void setMenuMouseover(bool hover) { menuState.mouseoverMenu = hover; }
  bool isMenuActive() const;
  
  // Messages
  void addChatMessage(const std::string& message);
  std::vector<std::string> getChatMessages(int count = 5) const;
  
  // Notifications
  void showNotification(const std::string& title, const std::string& message);
  
  // Pause
  void pauseGame();
  void resumeGame();
  bool isPaused() const { return menuState.isPaused; }
  
private:
  MenuState menuState;
  HUDData hudData;
  std::vector<std::string> chatLog;
  std::map<UIPanel, UIElement> panels;
};

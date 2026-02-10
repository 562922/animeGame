#include "UIManager.h"
#include <ctime>

UIManager::UIManager() {
  menuState.currentState = GameState::LOGIN_MENU;
  menuState.activePanel = UIPanel::NONE;
  menuState.isPaused = false;
}

void UIManager::setGameState(GameState newState) {
  menuState.currentState = newState;
  if (newState == GameState::PAUSE_MENU) {
    menuState.isPaused = true;
  } else if (newState == GameState::IN_GAME) {
    menuState.isPaused = false;
  }
}

void UIManager::showPanel(UIPanel panel) {
  menuState.activePanel = panel;
  if (panels.find(panel) != panels.end()) {
    panels[panel].visible = true;
    panels[panel].active = true;
  }
}

void UIManager::hidePanel(UIPanel panel) {
  if (panels.find(panel) != panels.end()) {
    panels[panel].visible = false;
    panels[panel].active = false;
  }
  if (menuState.activePanel == panel) {
    menuState.activePanel = UIPanel::NONE;
  }
}

void UIManager::updateHUD(const HUDData& newHudData) {
  hudData = newHudData;
}

bool UIManager::isMenuActive() const {
  return menuState.currentState == GameState::LOGIN_MENU ||
         menuState.currentState == GameState::CHARACTER_CREATION ||
         menuState.currentState == GameState::MAIN_MENU ||
         menuState.currentState == GameState::PAUSE_MENU;
}

void UIManager::addChatMessage(const std::string& message) {
  chatLog.push_back(message);
  if (chatLog.size() > 100) {
    chatLog.erase(chatLog.begin());
  }
}

std::vector<std::string> UIManager::getChatMessages(int count) const {
  std::vector<std::string> result;
  int start = static_cast<int>(chatLog.size()) - count;
  if (start < 0) start = 0;
  for (int i = start; i < static_cast<int>(chatLog.size()); ++i) {
    result.push_back(chatLog[i]);
  }
  return result;
}

void UIManager::showNotification(const std::string& title, const std::string& message) {
  addChatMessage(title + ": " + message);
}

void UIManager::pauseGame() {
  menuState.isPaused = true;
  setGameState(GameState::PAUSE_MENU);
}

void UIManager::resumeGame() {
  menuState.isPaused = false;
  setGameState(GameState::IN_GAME);
}

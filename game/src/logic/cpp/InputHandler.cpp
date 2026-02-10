#include "InputHandler.h"
#include <algorithm>
#include <chrono>

InputHandler::InputHandler() {
  // Default key bindings for WASD + mouse + UI keys
  bindKey(InputAction::MOVE_FORWARD, "W");
  bindKey(InputAction::MOVE_BACKWARD, "S");
  bindKey(InputAction::MOVE_LEFT, "A");
  bindKey(InputAction::MOVE_RIGHT, "D");
  bindKey(InputAction::JUMP, "SPACE");
  bindKey(InputAction::INTERACT, "E");
  bindKey(InputAction::ATTACK, "MOUSE_LEFT");
  bindKey(InputAction::SKILL_1, "1");
  bindKey(InputAction::SKILL_2, "2");
  bindKey(InputAction::SKILL_3, "3");
  bindKey(InputAction::SKILL_4, "4");
  bindKey(InputAction::OPEN_INVENTORY, "I");
  bindKey(InputAction::OPEN_STATS, "C");
  bindKey(InputAction::OPEN_QUESTS, "Q");
  bindKey(InputAction::OPEN_MAP, "M");
  bindKey(InputAction::PAUSE, "ESC");
  bindKey(InputAction::CONFIRM, "RETURN");
  bindKey(InputAction::CANCEL, "ESC");
}

void InputHandler::bindKey(InputAction action, const std::string& keyCode) {
  // Remove old binding if exists
  auto it = bindings.find(action);
  if (it != bindings.end() && !it->second.keyCode.empty()) {
    keyToAction.erase(it->second.keyCode);
  }
  
  // Add new binding
  InputBinding binding;
  binding.action = action;
  binding.keyCode = keyCode;
  bindings[action] = binding;
  keyToAction[keyCode] = action;
}

void InputHandler::unbindKey(InputAction action) {
  auto it = bindings.find(action);
  if (it != bindings.end()) {
    keyToAction.erase(it->second.keyCode);
    bindings.erase(it);
  }
}

std::string InputHandler::getKeyBinding(InputAction action) const {
  auto it = bindings.find(action);
  if (it != bindings.end()) {
    return it->second.keyCode;
  }
  return "";
}

void InputHandler::onKeyDown(const std::string& keyCode) {
  auto it = keyToAction.find(keyCode);
  if (it != keyToAction.end()) {
    InputAction action = it->second;
    auto bindIt = bindings.find(action);
    if (bindIt != bindings.end()) {
      bindIt->second.isPressed = true;
      bindIt->second.lastPressTime = std::chrono::system_clock::now().time_since_epoch().count() / 1000000; // ms
      
      PlayerCommand cmd;
      cmd.action = action;
      cmd.pressed = true;
      cmd.value = 1.0f;
      commandQueue.push_back(cmd);
    }
  }
}

void InputHandler::onKeyUp(const std::string& keyCode) {
  auto it = keyToAction.find(keyCode);
  if (it != keyToAction.end()) {
    InputAction action = it->second;
    auto bindIt = bindings.find(action);
    if (bindIt != bindings.end()) {
      bindIt->second.isPressed = false;
      
      PlayerCommand cmd;
      cmd.action = action;
      cmd.pressed = false;
      cmd.value = 0.0f;
      commandQueue.push_back(cmd);
    }
  }
}

void InputHandler::onMouseMove(int x, int y) {
  int dx = x - mouseX;
  int dy = y - mouseY;
  mouseX = x;
  mouseY = y;
}

void InputHandler::onMouseDown(int button) {
  std::string mouseKey = (button == 0) ? "MOUSE_LEFT" : (button == 1) ? "MOUSE_RIGHT" : "MOUSE_MIDDLE";
  onKeyDown(mouseKey);
}

void InputHandler::onMouseUp(int button) {
  std::string mouseKey = (button == 0) ? "MOUSE_LEFT" : (button == 1) ? "MOUSE_RIGHT" : "MOUSE_MIDDLE";
  onKeyUp(mouseKey);
}

void InputHandler::onAnalogInput(InputAction action, float value) {
  auto it = bindings.find(action);
  if (it != bindings.end()) {
    it->second.isPressed = (value > 0.1f);
    
    PlayerCommand cmd;
    cmd.action = action;
    cmd.value = value;
    cmd.pressed = it->second.isPressed;
    commandQueue.push_back(cmd);
  }
}

bool InputHandler::isActionPressed(InputAction action) const {
  auto it = bindings.find(action);
  if (it != bindings.end()) {
    return it->second.isPressed;
  }
  return false;
}

float InputHandler::getAnalogValue(InputAction action) const {
  auto it = bindings.find(action);
  if (it != bindings.end()) {
    return it->second.isPressed ? 1.0f : 0.0f;
  }
  return 0.0f;
}

std::vector<PlayerCommand> InputHandler::getPendingCommands() {
  return commandQueue;
}

void InputHandler::clearFrame() {
  commandQueue.clear();
}

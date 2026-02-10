#pragma once
#include <string>
#include <map>
#include <vector>

enum class InputAction {
  MOVE_FORWARD = 0,
  MOVE_BACKWARD = 1,
  MOVE_LEFT = 2,
  MOVE_RIGHT = 3,
  JUMP = 4,
  INTERACT = 5,
  ATTACK = 6,
  SKILL_1 = 7,
  SKILL_2 = 8,
  SKILL_3 = 9,
  SKILL_4 = 10,
  OPEN_INVENTORY = 11,
  OPEN_STATS = 12,
  OPEN_QUESTS = 13,
  OPEN_MAP = 14,
  PAUSE = 15,
  CONFIRM = 16,
  CANCEL = 17,
  TAB = 18,
  CAMERA_UP = 19,
  CAMERA_DOWN = 20,
  CAMERA_LEFT = 21,
  CAMERA_RIGHT = 22
};

// Input binding
struct InputBinding {
  InputAction action;
  std::string keyCode; // e.g., "W", "MOUSE_LEFT", "SPACE"
  bool isPressed = false;
  long long lastPressTime = 0;
};

// Player command (input -> action)
struct PlayerCommand {
  InputAction action;
  float value; // 0-1 for analog input
  bool pressed = false;
};

// Input Manager
class InputHandler {
public:
  InputHandler();
  
  // Key binding
  void bindKey(InputAction action, const std::string& keyCode);
  void unbindKey(InputAction action);
  std::string getKeyBinding(InputAction action) const;
  
  // Input events (called by engine/Unreal)
  void onKeyDown(const std::string& keyCode);
  void onKeyUp(const std::string& keyCode);
  void onMouseMove(int x, int y);
  void onMouseDown(int button); // 0=left, 1=right, 2=middle
  void onMouseUp(int button);
  void onAnalogInput(InputAction action, float value); // -1 to 1
  
  // Poll input state
  bool isActionPressed(InputAction action) const;
  float getAnalogValue(InputAction action) const;
  std::vector<PlayerCommand> getPendingCommands();
  
  // Clear frame input
  void clearFrame();
  
  // Settings
  void setSensitivity(float sens) { mouseSensitivity = sens; }
  void setInverted(bool invert) { invertY = invert; }
  
private:
  std::map<InputAction, InputBinding> bindings;
  std::map<std::string, InputAction> keyToAction;
  std::vector<PlayerCommand> commandQueue;
  float mouseSensitivity = 1.0f;
  bool invertY = false;
  int mouseX = 0, mouseY = 0;
};

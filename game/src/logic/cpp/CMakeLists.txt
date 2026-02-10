cmake_minimum_required(VERSION 3.10)
project(AnimeGameCPP)

set(CMAKE_CXX_STANDARD 17)

add_library(GameLogic STATIC
  GameLogic.cpp
  Player.cpp
  Enemy.cpp
  CombatSystem.cpp
  ItemManager.cpp
  UIManager.cpp
  InputHandler.cpp
  SaveSystem.cpp
  DialogueSystem.cpp
  SkillSystem.cpp
)

target_include_directories(GameLogic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

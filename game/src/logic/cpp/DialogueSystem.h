#pragma once
#include <string>
#include <vector>
#include <map>
#include "Enemy.h"  // For NPC struct

// Dialogue choice
struct DialogueChoice {
  std::string choiceID;
  std::string text;
  std::string nextNodeID;
  bool endsDialogue = false;
};

// Dialogue node
struct DialogueNode {
  std::string nodeID;
  std::string npcID;
  std::string text;
  std::vector<DialogueChoice> choices;
  bool endsDialogue = false;
  std::string questID; // If this dialogue triggers a quest
};

// Dialogue tree
struct DialogueTree {
  std::string npcID;
  std::string startNodeID;
  std::map<std::string, DialogueNode> nodes;
};

// Dialogue Manager
class DialogueSystem {
public:
  DialogueSystem();
  
  // Load dialogue trees
  bool loadDialogueTree(const NPC& npc);
  
  // Get current dialogue
  DialogueNode getCurrentNode(const std::string& npcID) const;
  std::vector<DialogueChoice> getCurrentChoices(const std::string& npcID) const;
  
  // Progress dialogue
  void selectChoice(const std::string& npcID, const std::string& choiceID);
  void startDialogue(const std::string& npcID);
  void endDialogue(const std::string& npcID);
  
  // State
  bool isDialogueActive(const std::string& npcID) const;
  std::string getCurrentNodeID(const std::string& npcID) const;
  
private:
  std::map<std::string, DialogueTree> dialogueTrees;
  std::map<std::string, std::string> currentNodes; // npcID -> current nodeID
};

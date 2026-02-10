#include "DialogueSystem.h"

DialogueSystem::DialogueSystem() {}

bool DialogueSystem::loadDialogueTree(const NPC& npc) {
  // Implementation would load from JSON
  // For now, create a basic tree
  DialogueTree tree;
  tree.npcID = npc.npcID;
  tree.startNodeID = "greeting";
  
  DialogueNode greetNode;
  greetNode.nodeID = "greeting";
  greetNode.npcID = npc.npcID;
  greetNode.text = "Hello, adventurer!";
  
  DialogueChoice choice1, choice2;
  choice1.choiceID = "ask_quest";
  choice1.text = "I'm looking for a quest.";
  choice1.nextNodeID = "quest_offer";
  
  choice2.choiceID = "goodbye";
  choice2.text = "Goodbye.";
  choice2.nextNodeID = "end";
  choice2.endsDialogue = true;
  
  greetNode.choices.push_back(choice1);
  greetNode.choices.push_back(choice2);
  
  tree.nodes[greetNode.nodeID] = greetNode;
  
  DialogueNode endNode;
  endNode.nodeID = "end";
  endNode.npcID = npc.npcID;
  endNode.text = "Farewell!";
  endNode.endsDialogue = true;
  tree.nodes[endNode.nodeID] = endNode;
  
  dialogueTrees[npc.npcID] = tree;
  currentNodes[npc.npcID] = tree.startNodeID;
  
  return true;
}

DialogueNode DialogueSystem::getCurrentNode(const std::string& npcID) const {
  auto treeIt = dialogueTrees.find(npcID);
  if (treeIt == dialogueTrees.end()) {
    return DialogueNode();
  }
  
  auto nodeIt = treeIt->second.nodes.find(currentNodes.at(npcID));
  if (nodeIt != treeIt->second.nodes.end()) {
    return nodeIt->second;
  }
  
  return DialogueNode();
}

std::vector<DialogueChoice> DialogueSystem::getCurrentChoices(const std::string& npcID) const {
  DialogueNode node = getCurrentNode(npcID);
  return node.choices;
}

void DialogueSystem::selectChoice(const std::string& npcID, const std::string& choiceID) {
  DialogueNode node = getCurrentNode(npcID);
  for (const auto& choice : node.choices) {
    if (choice.choiceID == choiceID) {
      currentNodes[npcID] = choice.nextNodeID;
      return;
    }
  }
}

void DialogueSystem::startDialogue(const std::string& npcID) {
  auto treeIt = dialogueTrees.find(npcID);
  if (treeIt != dialogueTrees.end()) {
    currentNodes[npcID] = treeIt->second.startNodeID;
  }
}

void DialogueSystem::endDialogue(const std::string& npcID) {
  auto it = currentNodes.find(npcID);
  if (it != currentNodes.end()) {
    currentNodes.erase(it);
  }
}

bool DialogueSystem::isDialogueActive(const std::string& npcID) const {
  return currentNodes.find(npcID) != currentNodes.end();
}

std::string DialogueSystem::getCurrentNodeID(const std::string& npcID) const {
  auto it = currentNodes.find(npcID);
  if (it != currentNodes.end()) {
    return it->second;
  }
  return "";
}

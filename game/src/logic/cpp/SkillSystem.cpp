#include "SkillSystem.h"
#include <chrono>

SkillSystem::SkillSystem() {}

bool SkillSystem::learnSkill(const std::string& skillID) {
  auto it = skillDatabase.find(skillID);
  if (it == skillDatabase.end()) return false;
  
  PlayerSkill pskill;
  pskill.skillData = it->second;
  pskill.lastUsedTime = 0;
  
  learnedSkills[skillID] = pskill;
  return true;
}

bool SkillSystem::forgetSkill(const std::string& skillID) {
  auto it = learnedSkills.find(skillID);
  if (it == learnedSkills.end()) return false;
  
  learnedSkills.erase(it);
  return true;
}

bool SkillSystem::canUseSkill(const std::string& skillID, long long currentTimeMS) const {
  auto it = learnedSkills.find(skillID);
  if (it == learnedSkills.end()) return false;
  
  return !it->second.isOnCooldown(currentTimeMS) && it->second.skillData.isActive;
}

bool SkillSystem::useSkill(const std::string& skillID, long long currentTimeMS) {
  if (!canUseSkill(skillID, currentTimeMS)) return false;
  
  auto it = learnedSkills.find(skillID);
  if (it != learnedSkills.end()) {
    it->second.lastUsedTime = currentTimeMS;
    return true;
  }
  return false;
}

Skill SkillSystem::getSkillData(const std::string& skillID) const {
  auto it = learnedSkills.find(skillID);
  if (it != learnedSkills.end()) {
    return it->second.skillData;
  }
  return Skill();
}

std::vector<Skill> SkillSystem::getLearnedSkills() const {
  std::vector<Skill> result;
  for (const auto& pair : learnedSkills) {
    result.push_back(pair.second.skillData);
  }
  return result;
}

float SkillSystem::getRemainingCooldown(const std::string& skillID, long long currentTimeMS) const {
  auto it = learnedSkills.find(skillID);
  if (it == learnedSkills.end()) return 0.0f;
  
  return it->second.getRemainingCooldown(currentTimeMS);
}

void SkillSystem::resetCooldown(const std::string& skillID) {
  auto it = learnedSkills.find(skillID);
  if (it != learnedSkills.end()) {
    it->second.lastUsedTime = 0;
  }
}

bool SkillSystem::loadSkillsFromAssets(const std::string& assetsPath) {
  // Implementation would load from JSON file
  // For now, create some basic skills
  
  Skill slash;
  slash.skillID = "slash";
  slash.name = "Slash";
  slash.description = "A basic sword slash.";
  slash.baseDamage = 20;
  slash.cooldownSeconds = 1.0f;
  slash.manaCost = 0;
  skillDatabase["slash"] = slash;
  
  Skill fireball;
  fireball.skillID = "fireball";
  fireball.name = "Fireball";
  fireball.description = "Unleash a ball of fire.";
  fireball.baseDamage = 40;
  fireball.cooldownSeconds = 3.0f;
  fireball.manaCost = 30;
  fireball.skillType = "magical";
  skillDatabase["fireball"] = fireball;
  
  return true;
}

bool PlayerSkill::isOnCooldown(long long currentTimeMS) const {
  if (lastUsedTime == 0) return false;
  
  long long elapsedMS = currentTimeMS - lastUsedTime;
  long long cooldownMS = static_cast<long long>(skillData.cooldownSeconds * 1000.0f);
  
  return elapsedMS < cooldownMS;
}

float PlayerSkill::getRemainingCooldown(long long currentTimeMS) const {
  if (lastUsedTime == 0) return 0.0f;
  
  long long elapsedMS = currentTimeMS - lastUsedTime;
  long long cooldownMS = static_cast<long long>(skillData.cooldownSeconds * 1000.0f);
  
  if (elapsedMS >= cooldownMS) return 0.0f;
  
  return (cooldownMS - elapsedMS) / 1000.0f;
}

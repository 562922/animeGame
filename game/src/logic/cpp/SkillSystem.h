#pragma once
#include <string>
#include <vector>
#include <map>

// Skill definition
struct Skill {
  std::string skillID;
  std::string name;
  std::string description;
  int level = 1;
  
  // Cost
  int manaCost = 0;
  int staminaCost = 0;
  
  // Cooldown
  float cooldownSeconds = 0.0f;
  
  // Damage
  int baseDamage = 0;
  float damageMultiplier = 1.0f;
  
  // Range
  float range = 10.0f;
  
  // Activation
  float castTime = 0.0f; // seconds
  
  // Flags
  bool isActive = true;
  bool requiresTarget = true;
  std::string skillType; // physical, magical, heal, buff, debuff
};

// Player skill instance with cooldown tracking
struct PlayerSkill {
  Skill skillData;
  long long lastUsedTime = 0;
  
  bool isOnCooldown(long long currentTimeMS) const;
  float getRemainingCooldown(long long currentTimeMS) const;
};

// Skill System
class SkillSystem {
public:
  SkillSystem();
  
  // Learn skill
  bool learnSkill(const std::string& skillID);
  bool forgetSkill(const std::string& skillID);
  
  // Use skill
  bool canUseSkill(const std::string& skillID, long long currentTimeMS) const;
  bool useSkill(const std::string& skillID, long long currentTimeMS);
  
  // Get skill info
  Skill getSkillData(const std::string& skillID) const;
  std::vector<Skill> getLearnedSkills() const;
  
  // Cooldown management
  float getRemainingCooldown(const std::string& skillID, long long currentTimeMS) const;
  void resetCooldown(const std::string& skillID);
  
  // Load skills from assets
  bool loadSkillsFromAssets(const std::string& assetsPath);
  
private:
  std::map<std::string, PlayerSkill> learnedSkills;
  std::map<std::string, Skill> skillDatabase;
};

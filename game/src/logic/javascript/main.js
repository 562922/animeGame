import Items from './items.json' assert { type: 'json' };
import Skills from './skills.json' assert { type: 'json' };
import Enemies from './enemies.json' assert { type: 'json' };
import NPCs from './npcs.json' assert { type: 'json' };
import Quests from './quests.json' assert { type: 'json' };
import Maps from './maps.json' assert { type: 'json' };
import Mods from './mods.json' assert { type: 'json' };
import { addNewPlayer } from '../player/player.js';

//consts/databases
const saoStats = [
    "HP",          // Hit Points
    "SP",          // Skill Points / Mana
    "STA",     // Determines running, dodging, heavy swings
    "STR",         // Strength – Physical attack power
    "DEX",         // Dexterity – Accuracy and finesse damage
    "AGI",         // Agility – Attack & movement speed, evasion
    "VIT",         // Vitality – Max HP, damage reduction
    "INT",         // Intelligence – Skill success, crafting, etc.
    "MIND",        // Mental/Willpower – Status resistance, skill learning
    "LUK",         // Luck – Critical hits, item drops, randomness
    "ATKPOW",      // Attack Power
    "DEF",      // Defense
    "CRT", // Critical Hit Rate
    "EVA", // Evasion Rate
    "PROF", // Profession/Skill Proficiency
    "LVL", // Level
    "EXP"  // Experience Points
];

let player = {
    "PlayerID": 1,
    "DisplayName": "Hero",
    "Username": "HeroPlayer",
    "Password": "securepassword123",
    "stats": {
        "HP": getStat(),  // Hit Points
        "MP": getStat(),  // Mana Points
        "STA": getStat(), // Stamina
        "STR": getStat(), // Strength
        "DEX": getStat(), // Dexterity
        "AGI": getStat(), // Agility
        "VIT": getStat(), // Vitality
        "INT": getStat(), // Intelligence
        "MIND": getStat(),    // Mental/Willpower
        "LUK": getStat(), // Luck
        "ATKPOW": getStat(),   // Attack Power
        "DEF": getStat(),     // Defense
        "CRT": getStat(), // Critical Hit Rate
        "EVA": getStat(), // Evasion Rate
        "LVL": 1, // Level
        "EXP": 0  // Experience Points
    },
    "inventory": {
        "equipment": {
            "weapon": null,
            "armor": null,
            "shield": null,
            "accessories": []
        },
        "items": {
            "item1": count1,
            "item2": count2,
            // ...
        },
        "gold": 0
    },
    "skills": [
        {
            "skillName": "One-Handed Sword",
            "type": "Weapon Skill",
            "levelRequirement": 1,
            "maxLevel": 1000,
            "skillProficiency": 1,
            "effectBonus": {
                "ATK": stats.ATK * 1.05,
            },
            "passiveActive": "Passive",
            "weaponRestriction": "One-handed swords",
            "modsAvailable": true,
            "modSlots": 3,
            "mods": [],
            "compositeEffects": {
                "Dual Blades": "Sword Skill"
            },
            "description": "Increases attack power and allows one-handed sword techniques."
        }
    ],
    "quests": [
        {
            "questName": "The Beginning",
            "status": "In Progress",
            "objectives": [
                "Talk to the village elder",
                "Collect 5 healing herbs"
            ],
            "rewards": {
                "EXP": 100,
                "Items": [
                    "Health Potion x2"
                ]
            },
            "description": "Start your journey by helping the village elder."
        }
    ],
    "position": {
        "mapID": 1,
        "X": 0,
        "Y": 0,
        "Z": 0
    }
};

//actions
function performAttack() { }
function performSkill() { }
function useItem() { }
function equipItem() { }
function unequipItem() { }
function interactWithNPC() { }
function movePlayer() { }
function openInventory() { }
function openSkillTree() { }
function openMap() { }
function saveProgress() { }
function loadProgress() { }

//input handlers
function handleKeyboardInput() { }
function handleMouseInput() { }
function handleGamepadInput() { }

//event handlers
function onNewPlayer() {
    const filePath = 'src/player/players.json';
    addNewPlayer(filePath, player);
}
function onEnemyDefeated() {
    const e = enemy.loot;
    let rewards = {};
    rewards.EXP = e.EXP;
    rewards.items = e.items;
    rewards.gold = e.gold;
    rewards.equipment = e.equipment;
}
function onLevelUp() { }
function onItemAcquired() { }
function onQuestCompleted() { }
function onPlayerDamaged() { }
function onSkillUsed() { }
function onNPCInteraction() { }
function onAreaEntered() { }
function onGameSaved() { }
function onGameLoaded() { }
function onNewGameStarted() { }
function onAreaEntered() { }
function onPartyFormed() { }
function onPartyDesolved() { }

//player
function getStat() { }
function getEquipment() { }
function getInventory() { }
function getSkills() { }

//helpers
function calcDamage() { }
function calcHealing() { }
function calcShield() { }
function gainExperience() { }
function levelUp() { }
function saveGame() { }
function loadGame() { }
function startNewGame() { }

export {
    onNewPlayer,
    onEnemyDefeated,
    onLevelUp,
    onItemAcquired,
    onQuestCompleted,
    onPlayerDamaged,
    onSkillUsed,
    onNPCInteraction,
    onAreaEntered,
    onGameSaved,
    onGameLoaded,
    onNewGameStarted,
    onAreaEntered,
    onPartyFormed,
    onPartyDesolved
};
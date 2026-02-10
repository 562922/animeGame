import path from "path";
import { loadJSON, saveJSON } from "../../utils.js";
import { loadPlayer, savePlayer } from "./player.js";
import { addItemToInventory } from "./item.js";
import { applyExperience } from "./player.js";

const QUESTS_FILE = path.join(process.cwd(), "game", "assets", "PLAY", "quests.json");

export function loadQuest(questID) {
	const quests = loadJSON(QUESTS_FILE) || [];
	if (typeof questID === "number") return quests.find((q) => q.questID === questID) || null;
	return quests.find((q) => q.questName === questID) || null;
}

export function trackQuestProgress(playerID, questID) {
	const player = loadPlayer(playerID);
	const quest = loadQuest(questID);
	if (!player || !quest) return false;
	player.quests = player.quests || { main: {}, side: {}, daily: {} };
	const qKey = `q_${quest.questID}`;
	player.quests.side = player.quests.side || {};
	player.quests.side[qKey] = player.quests.side[qKey] || { progress: 0, completed: false };
	player.quests.side[qKey].progress += 1;
	savePlayer(player);
	return player.quests.side[qKey];
}

export function completeQuest(playerID, questID) {
	const player = loadPlayer(playerID);
	const quest = loadQuest(questID);
	if (!player || !quest) return false;
	// mark as completed on player
	player.quests = player.quests || { main: {}, side: {}, daily: {} };
	const qKey = `q_${quest.questID}`;
	player.quests.side[qKey] = player.quests.side[qKey] || {};
	player.quests.side[qKey].completed = true;
	// apply rewards
	rewardQuest(playerID, questID);
	savePlayer(player);
	return true;
}

export function rewardQuest(playerID, questID) {
	const player = loadPlayer(playerID);
	const quest = loadQuest(questID);
	if (!player || !quest) return false;
	if (quest.rewards) {
		if (quest.rewards.EXP) applyExperience(playerID, quest.rewards.EXP);
		if (Array.isArray(quest.rewards.items)) {
			quest.rewards.items.forEach((name) => addItemToInventory(playerID, name, 1));
		}
	}
	return true;
}

// Event handlers (placeholders)
export function onQuestObjectiveUpdate() {}
export function onQuestFail() {}
export function onDailyReset() {}

export function getQuestObjectives(questID) {
	const q = loadQuest(questID);
	return q ? q.objectives || [] : [];
}

export function validateQuestState(playerID, questID) {
	const player = loadPlayer(playerID);
	if (!player) return false;
	const qKey = `q_${questID}`;
	return !!(player.quests && (player.quests.main || player.quests.side || player.quests.daily) && (player.quests.side && player.quests.side[qKey]));
}
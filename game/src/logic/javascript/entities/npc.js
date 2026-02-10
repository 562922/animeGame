import path from "path";
import { loadJSON } from "../utils.js";
import { loadPlayer, savePlayer } from "../player/player.js";
import { loadQuest, rewardQuest } from "../player/quest.js";

const NPCS_FILE = path.join(process.cwd(), "game", "assets", "NPC", "npcs.json");
const DIALOGUE_FILE = path.join(process.cwd(), "game", "assets", "NPC", "dialogue.json");

export function startDialogue(npcID, playerID) {
	const npcs = loadJSON(NPCS_FILE) || [];
	const npc = npcs.find((n) => n.npcID === npcID) || null;
	const tree = getDialogueTree(npcID);
	return { npc, tree };
}

export function advanceDialogue(dialogueID, choice) {
	// for now just return the node if present
	const dialog = loadJSON(DIALOGUE_FILE) || {};
	// dialogueID may be name.key
	const parts = String(dialogueID).split(".");
	let node = dialog;
	for (const p of parts) {
		if (node[p] === undefined) return null;
		node = node[p];
	}
	return node;
}

export function endDialogue() {
	return true;
}

export function assignQuest(npcID, playerID) {
	const questList = loadQuest ? loadQuest() : null;
	const player = loadPlayer(playerID);
	if (!player) return false;
	player.quests = player.quests || { main: {}, side: {}, daily: {} };
	// naive: assign first quest from NPC's related quests in quests.json
	// We'll search quests.json for relatedNPCs containing the NPC name
	const quests = loadJSON(path.join(process.cwd(), "game", "assets", "PLAY", "quests.json")) || [];
	const npcs = loadJSON(NPCS_FILE) || [];
	const npc = npcs.find((n) => n.npcID === npcID);
	if (!npc) return false;
	const name = npc.npcName;
	const first = quests.find((q) => Array.isArray(q.relatedNPCs) && q.relatedNPCs.includes(name));
	if (!first) return false;
	const key = `q_${first.questID}`;
	player.quests.side[key] = { progress: 0, completed: false };
	savePlayer(player);
	return true;
}

//Event handlers
export function onDialogueStart() {}
export function onDialogueChoice() {}
export function onQuestAccepted() {}
export function onQuestCompleted() {}

export function getDialogueTree(npcID) {
	const dialog = loadJSON(DIALOGUE_FILE) || {};
	const npcs = loadJSON(NPCS_FILE) || [];
	const npc = npcs.find((n) => n.npcID === npcID);
	if (!npc) return null;
	return dialog[npc.npcName] || null;
}

export function checkQuestCompletion(playerID, questID) {
	const player = loadPlayer(playerID);
	if (!player) return false;
	const key = `q_${questID}`;
	const has = (player.quests && player.quests.side && player.quests.side[key]) || null;
	return has && has.completed;
}
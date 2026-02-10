import path from "path";
import { loadJSON, saveJSON, deepClone, randomInt, clamp } from "../utils.js";

const PLAYERS_DIR = () => path.join(process.cwd(), "game", "assets", "PLAY", "players");

export function loadPlayer(playerID) {
	const file = path.join(PLAYERS_DIR(), `player_${String(playerID).padStart(4, "0")}.json`);
	return loadJSON(file);
}

export function savePlayer(playerData) {
	const file = path.join(PLAYERS_DIR(), `player_${String(playerData.playerID).padStart(4, "0")}.json`);
	return saveJSON(file, playerData);
}

export function updatePlayerStats(playerID) {
	const player = loadPlayer(playerID);
	if (!player) return null;
	const derived = calculateDerivedStats(player.stats, player.inventory && player.inventory.equipment);
	player.derivedStats = derived;
	savePlayer(player);
	return player;
}

export function applyExperience(playerID, exp) {
	const player = loadPlayer(playerID);
	if (!player) return false;
	player.EXP = (player.EXP || 0) + exp;
	let leveled = false;
	const threshold = (lvl) => lvl * 100;
	while (player.EXP >= threshold(player.LVL)) {
		player.EXP -= threshold(player.LVL);
		player.LVL += 1;
		levelUpPlayer(player);
		leveled = true;
	}
	savePlayer(player);
	return leveled;
}

export function levelUpPlayer(playerDataOrID) {
	let player = typeof playerDataOrID === "number" ? loadPlayer(playerDataOrID) : playerDataOrID;
	if (!player) return null;
	// simple stat increases per level
	player.LVL = (player.LVL || 1) + 0; // ensure exists
	player.stats.HP = (player.stats.HP || 100) + 10;
	player.stats.STR = (player.stats.STR || 1) + 2;
	player.stats.DEX = (player.stats.DEX || 1) + 1;
	player.stats.CON = (player.stats.CON || 1) + 1;
	player.stats.STA = (player.stats.STA || 1) + 1;
	if (typeof playerDataOrID === "number") savePlayer(player);
	return player;
}

export function updatePlayerPosition(playerID, position) {
	const player = loadPlayer(playerID);
	if (!player) return false;
	player.position = { ...player.position, ...position };
	savePlayer(player);
	return true;
}

export function respawnPlayer(playerID) {
	const player = loadPlayer(playerID);
	if (!player) return false;
	player.stats.HP = Math.max(player.stats.HP, 1);
	// move to spawn if defined
	player.position = player.position || { mapID: 0, X: 0, Y: 0, Z: 0 };
	savePlayer(player);
	return true;
}

// Event handlers (placeholders for game engine hooks)
export function onPlayerMove() {}
export function onPlayerJump() {}
export function onPlayerAttack() {}
export function onPlayerDeath() {}
export function onPlayerRespawn() {}
export function onPlayerLevelUp() {}
export function onPlayerEquipItem() {}
export function onPlayerUnequipItem() {}

export function calculateDerivedStats(baseStats = {}, equipment = {}) {
	const itemsFile = path.join(process.cwd(), "game", "assets", "ITEM", "items.json");
	const items = loadJSON(itemsFile) || [];
	const agg = { ...baseStats };
	// equipment structure is arbitrary; walk nested and add stats
	function addStatsFromItem(obj) {
		if (!obj) return;
		if (obj.itemID !== undefined) {
			const it = items.find((i) => i.itemID === obj.itemID);
			if (it && it.itemStats) {
				Object.entries(it.itemStats).forEach(([k, v]) => {
					agg[k] = (agg[k] || 0) + v;
				});
			}
		} else if (typeof obj === "object") {
			Object.values(obj).forEach(addStatsFromItem);
		}
	}
	addStatsFromItem(equipment);
	return agg;
}

export function validatePlayerState(playerData) {
	if (!playerData) return false;
	if (typeof playerData.playerID !== "number") return false;
	if (!playerData.stats) return false;
	return true;
}

export function isPlayerAlive(playerID) {
	const p = loadPlayer(playerID);
	return !!(p && p.stats && p.stats.HP > 0);
}
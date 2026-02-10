import path from "path";
import { loadJSON, generateUUID as genUUID, deepClone } from "./utils.js";
import { loadPlayer } from "./player/player.js";

const ASSETS = {};

export function initGame() {
	loadAssets();
	return true;
}

export function loadAssets() {
	const base = path.join(process.cwd(), "game", "assets");
	const files = {
		items: path.join(base, "ITEM", "items.json"),
		skills: path.join(base, "ITEM", "skills.json"),
		mods: path.join(base, "ITEM", "mods.json"),
		npcs: path.join(base, "NPC", "npcs.json"),
		dialogue: path.join(base, "NPC", "dialogue.json"),
		enemies: path.join(base, "PLAY", "enemies.json"),
		maps: path.join(base, "PLAY", "maps.json"),
		quests: path.join(base, "PLAY", "quests.json"),
		dungeon: path.join(base, "ROOMS", "dungeon.json")
	};
	Object.entries(files).forEach(([k, v]) => {
		ASSETS[k] = loadJSON(v);
	});
	return ASSETS;
}

export function connectToServer() {
	// placeholder for networking
	return true;
}

export function spawnPlayer(playerID) {
	const p = loadPlayer(playerID);
	if (!p) return null;
	p.instanceID = generateUUID();
	return p;
}

export function enterWorld(mapID) {
	// placeholder to set map-level state
	return true;
}

export function gameTick(deltaTime = 16) {
	// called periodically by engine
	return { tick: Date.now(), dt: deltaTime };
}

export function shutdownGame() {
	return true;
}

// Event handlers
export function onGameStart() {}
export function onPlayerJoin() {}
export function onPlayerLeave() {}
export function onServerDisconnect() {}
export function onWorldSync() {}

// Helpers
export function validateAssetSchemas() {
	return !!ASSETS.items;
}

export function deepCloneGameObject(obj) {
	return deepClone(obj);
}

export function generateUUID() {
	return genUUID();
}
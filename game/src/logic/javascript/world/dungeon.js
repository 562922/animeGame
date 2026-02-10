import path from "path";
import { loadJSON, generateUUID, randomInt } from "../utils.js";

const DUNGEON_FILE = path.join(process.cwd(), "game", "assets", "ROOMS", "dungeon.json");
const instances = new Map();

export function generateDungeon(dungeonID = 0, seed = null) {
	const all = loadJSON(DUNGEON_FILE) || [];
	const def = all.find((d) => d.dungeonID === dungeonID) || all[0];
	if (!def) return null;
	const layout = JSON.parse(JSON.stringify(def));
	if (!seed) seed = generateDungeonSeed();
	layout.seed = seed;
	layout.instanceID = generateUUID();
	layout.rooms = randomizeRoomOrder(layout.rooms || []);
	instances.set(layout.instanceID, layout);
	return layout;
}

export function regenerateDungeon(dungeonID) {
	// remove existing instances for dungeonID
	for (const [k, v] of instances.entries()) {
		if (v.dungeonID === dungeonID) instances.delete(k);
	}
	return generateDungeon(dungeonID);
}

export function spawnDungeonRooms(dungeonLayout) {
	if (!dungeonLayout) return [];
	// spawn room instances
	return (dungeonLayout.rooms || []).map((r) => ({ ...r, spawnedAt: Date.now(), roomInstanceID: generateUUID() }));
}

export function resetDungeonInstance(instanceID) {
	if (!instances.has(instanceID)) return false;
	const def = instances.get(instanceID);
	def.rooms = randomizeRoomOrder(def.rooms || []);
	instances.set(instanceID, def);
	return true;
}

// Event handlers
export function onDungeonEnter() {}
export function onDungeonClear() {}
export function onDungeonResetTimer() {}

export function randomizeRoomOrder(rooms) {
	const copy = (rooms || []).slice();
	for (let i = copy.length - 1; i > 0; i--) {
		const j = randomInt(0, i);
		[copy[i], copy[j]] = [copy[j], copy[i]];
	}
	return copy;
}

export function validateDungeonLayout(layout) {
	return layout && Array.isArray(layout.rooms) && layout.rooms.length > 0;
}

export function generateDungeonSeed() {
	return Math.floor(Math.random() * 1e9);
}
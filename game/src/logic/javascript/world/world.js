import path from "path";
import { loadJSON } from "../utils.js";
import { loadPlayer } from "../player/player.js";

const MAPS_FILE = path.join(process.cwd(), "game", "assets", "PLAY", "maps.json");
const worldState = { maps: {}, time: Date.now() };

export function loadMap(mapID) {
	const maps = loadJSON(MAPS_FILE) || [];
	const m = maps.find((x) => x.mapID === mapID) || null;
	if (m) worldState.maps[mapID] = { ...m, loadedAt: Date.now() };
	return m;
}

export function unloadMap(mapID) {
	delete worldState.maps[mapID];
	return true;
}

export function syncMapState(mapID) {
	return worldState.maps[mapID] || null;
}

export function updateWorldTime() {
	worldState.time = Date.now();
	return worldState.time;
}

// Event handlers
export function onMapEnter() {}
export function onMapExit() {}
export function onZoneChange() {}

export function getSpawnZones(mapID) {
	const map = loadJSON(MAPS_FILE).find((m) => m.mapID === mapID);
	if (!map) return [];
	// placeholder: whole map is one spawn zone
	return [{ zoneID: `map_${mapID}_spawn`, mapID, bounds: map.dimensions }];
}

export function isPlayerInZone(playerID, zoneID) {
	const player = loadPlayer(playerID);
	if (!player) return false;
	const z = getSpawnZones(player.position.mapID).find((z) => z.zoneID === zoneID);
	if (!z) return false;
	// naive containment check
	return true;
}
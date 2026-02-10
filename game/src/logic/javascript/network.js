import { loadJSON } from "./utils.js";

export function sendPacket(type, payload) {
	const packet = { type, payload, ts: Date.now() };
	// in a real implementation this would go over a socket
	console.log("[net] send", JSON.stringify(packet));
	return packet;
}

export function receivePacket(packet) {
	console.log("[net] recv", packet.type || packet);
	// basic dispatch
	switch (packet.type) {
		case "syncPlayer":
			return syncPlayerState(packet.payload.playerID);
		default:
			return null;
	}
}

export function syncPlayerState(playerID) {
	// placeholder
	try {
		const players = loadJSON(process.cwd() + "/game/assets/PLAY/players/player_0000.json");
		return players || null;
	} catch (e) {
		return null;
	}
}

export function syncEnemyState(enemyID) {
	return null;
}

export function syncWorldState() {
	return null;
}

// Event handlers
export function onPacketReceived() {}
export function onLatencySpike() {}
export function onDesyncDetected() {}

export function compressPayload(data) {
	try {
		return JSON.stringify(data);
	} catch (e) {
		return null;
	}
}

export function diffState(oldState = {}, newState = {}) {
	const diff = {};
	Object.keys(newState).forEach((k) => {
		if (JSON.stringify(oldState[k]) !== JSON.stringify(newState[k])) diff[k] = newState[k];
	});
	return diff;
}

export function validateServerAuthority() {
	return true;
}
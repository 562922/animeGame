import path from "path";
import { loadJSON, generateUUID, randomInt } from "../utils.js";
import { loadPlayer, savePlayer } from "../player/player.js";
import { calculateDamage } from "../player/combat.js";

const ENEMIES_FILE = path.join(process.cwd(), "game", "assets", "PLAY", "enemies.json");
const PLAYERS_DIR = path.join(process.cwd(), "game", "assets", "PLAY", "players");

const instances = new Map();

export function spawnEnemy(enemyID, location = {}) {
	const list = loadJSON(ENEMIES_FILE) || [];
	const base = list.find((e) => e.enemyID === enemyID);
	if (!base) return null;
	const instanceID = generateUUID();
	const instance = JSON.parse(JSON.stringify(base));
	instance.instanceID = instanceID;
	instance.position = { ...instance.position, ...location };
	instance.state = "idle";
	instance.targetPlayer = null;
	instances.set(instanceID, instance);
	return instance;
}

export function despawnEnemy(instanceID) {
	return instances.delete(instanceID);
}

export function updateEnemyAI(instanceID) {
	const enemy = instances.get(instanceID);
	if (!enemy) return null;
	enemy.stats = enemy.stats || {};
	// find nearest player
	const player = findNearestPlayer(enemy);
	if (!player) {
		enemy.state = "idle";
		return enemy.state;
	}
	enemy.targetPlayer = player.playerID;
	// decide action
	if ((enemy.stats.HP || 0) <= 0) {
		enemy.state = "dead";
		onEnemyDeath(enemy);
		return enemy.state;
	}
	// if low HP, 30% chance to flee
	const hpPct = (enemy.stats.HP || 0) / Math.max(1, enemy.stats.HP || 1);
	if (hpPct < 0.3 && randomInt(1, 100) <= 30) {
		enemy.state = "flee";
		return enemy.state;
	}
	// Otherwise attempt attack
	enemy.state = "engage";
	// try to use special skill if available
	try {
		// calculate enemy damage against player and apply
		const dmg = calculateDamage(enemy, player, {});
		player.stats.HP = Math.max(0, (player.stats.HP || 0) - dmg);
		savePlayer(player);
	} catch (e) {
		// fallback: reduce some HP
		player.stats.HP = Math.max(0, (player.stats.HP || 0) - (enemy.stats.STR || 1));
		savePlayer(player);
	}
	return enemy.state;
}

export function enemyTakeDamage(instanceID, amount) {
	const enemy = instances.get(instanceID);
	if (!enemy) return null;
	enemy.stats = enemy.stats || {};
	enemy.stats.HP = Math.max(0, (enemy.stats.HP || 0) - amount);
	if (enemy.stats.HP <= 0) {
		onEnemyDeath(enemy);
	}
	return enemy.stats.HP;
}

export function enemyDropLoot(instanceID) {
	const enemy = instances.get(instanceID);
	if (!enemy) return [];
	const drops = enemy.deathDrops || { items: [] };
	const out = [];
	if (Array.isArray(drops.items)) out.push(...drops.items);
	return out;
}

// Event handlers
export function onEnemyAggro() {}
export function onEnemyAttack() {}
export function onEnemyDeath(enemy) {
	if (!enemy) return;
	instances.forEach((v, k) => {
		if (v === enemy || v.instanceID === (enemy.instanceID || enemy)) instances.delete(k);
	});
}
export function onEnemyRespawn() {}

// Helpers
export function selectAIState(enemy) {
	if (!enemy) return "idle";
	if ((enemy.stats && enemy.stats.HP) < 20) return "flee";
	return "patrol";
}

export function findNearestPlayer(enemy) {
	try {
		const fs = require("fs");
		const files = fs.readdirSync(PLAYERS_DIR).filter((f) => f.endsWith('.json'));
		if (!files.length) return null;
		const p = JSON.parse(fs.readFileSync(path.join(PLAYERS_DIR, files[0]), 'utf-8'));
		return p;
	} catch (e) {
		return null;
	}
}

export function rollLootTable(enemyID) {
	const list = loadJSON(ENEMIES_FILE) || [];
	const base = list.find((e) => e.enemyID === enemyID);
	if (!base) return [];
	return (base.deathDrops && base.deathDrops.items) || [];
}
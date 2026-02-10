import path from "path";
import { randomInt, clamp, loadJSON } from "../utils.js";
import { loadPlayer, savePlayer } from "./player.js";
import { getItemStats } from "./item.js";

const SKILLS_FILE = path.join(process.cwd(), "game", "assets", "ITEM", "skills.json");

export function useSkill(playerID, skillID, targetID) {
	const player = loadPlayer(playerID);
	if (!player) return null;
	const skill = getSkillData(skillID);
	if (!skill) return null;

	player.cooldowns = player.cooldowns || {};
	const name = skill.skillName || String(skillID);
	const now = Date.now();
	if (player.cooldowns[name] && player.cooldowns[name] > now) return { error: "on_cooldown" };

	// cost handling placeholder: active skills consume stamina
	if (skill.passiveActive === "Active") {
		const cost = skill.staminaCost || 5;
		if ((player.stats.STA || 0) < cost) return { error: "no_stamina" };
		player.stats.STA -= cost;
	}

	// resolve target (players only in this beta)
	let target = null;
	try { target = loadPlayer(targetID); } catch (e) { target = null; }

	const dmg = calculateDamage(player, target || {}, skill);
	if (target) {
		target.stats.HP = Math.max(0, (target.stats.HP || 0) - dmg);
		savePlayer(target);
	}

	// set cooldown
	const cd = calculateCooldown(skillID, player.stats || {});
	player.cooldowns[name] = now + Math.floor(cd * 1000);
	savePlayer(player);

	return { damage: dmg, targetID };
}

export function calculateDamage(attacker = {}, defender = {}, skill = {}) {
	const weaponATK = (attacker.derivedStats && attacker.derivedStats.ATKPOW) || 0;
	const base = (attacker.stats && attacker.stats.STR) || 0;
	let multiplier = 1;
	if (skill && skill.effectBonus && typeof skill.effectBonus === "string") {
		const m = skill.effectBonus.match(/([0-9]+)%/);
		if (m) multiplier = 1 + parseInt(m[1], 10) / 100;
		const raw = skill.effectBonus.match(/([0-9]+)x|([0-9]+)\s*normal/);
		if (raw && raw[1]) multiplier = parseInt(raw[1], 10);
	}
	const def = (defender.stats && defender.stats.DEF) || 0;
	let dmg = Math.max(0, Math.round((base + weaponATK) * multiplier) - def);
	// variance
	const variance = randomInt(-Math.floor(Math.max(1, dmg) * 0.1), Math.floor(Math.max(1, dmg) * 0.1));
	dmg = Math.max(0, dmg + variance);
	// crit
	if (rollCritChance(attacker.stats || {})) dmg = Math.round(dmg * 1.5);
	return dmg;
}

export function applyStatusEffect(targetID, effect) {
	const target = loadPlayer(targetID);
	if (!target) return false;
	target.statusEffects = target.statusEffects || [];
	target.statusEffects.push({ ...effect, appliedAt: Date.now() });
	savePlayer(target);
	return true;
}

export function resolveCombat(attackerID, defenderID) {
	const attacker = loadPlayer(attackerID);
	const defender = loadPlayer(defenderID);
	if (!attacker || !defender) return null;
	const dmg = calculateDamage(attacker, defender, {});
	defender.stats.HP = Math.max(0, (defender.stats.HP || 0) - dmg);
	savePlayer(defender);
	return { attackerID, defenderID, damage: dmg, defenderHP: defender.stats.HP };
}

// tick helper to decay cooldowns and status effects
export function tickPlayerTimers(playerID, deltaMS = 1000) {
	const p = loadPlayer(playerID);
	if (!p) return null;
	// status effects: remove expired ones (simple 10s lifetime default)
	p.statusEffects = (p.statusEffects || []).filter((s) => {
		const lifetime = s.durationMS || 10000;
		return (Date.now() - (s.appliedAt || 0)) < lifetime;
	});
	// no-op for cooldowns since we store absolute timestamps
	savePlayer(p);
	return p;
}

// Event handlers
export function onSkillCast() {}
export function onSkillCooldownEnd() {}
export function onCriticalHit() {}
export function onEnemyDefeated() {}

export function rollCritChance(stats = {}) {
	const crit = stats.CRT || stats.CRC || 0;
	const roll = randomInt(0, 100);
	return roll <= Math.min(50, crit);
}

export function getSkillData(skillID) {
	const skills = loadJSON ? loadJSON(SKILLS_FILE) : null;
	if (!skills) return null;
	if (typeof skillID === "number") return skills[skillID] || null;
	return skills.find((s) => s.skillName === skillID) || null;
}

export function calculateCooldown(skillID, stats = {}) {
	const base = 5; // seconds default
	const cdr = stats.CDR || 0;
	return Math.max(0.1, base * (1 - cdr / 100));
}
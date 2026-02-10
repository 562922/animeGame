import path from "path";
import { loadJSON, saveJSON } from "../utils.js";
import { loadPlayer, savePlayer } from "./player.js";

const ITEMS_FILE = path.join(process.cwd(), "game", "assets", "ITEM", "items.json");

export function loadItem(itemID) {
	const items = loadJSON(ITEMS_FILE) || [];
	if (typeof itemID === "number") return items.find((i) => i.itemID === itemID) || null;
	return items.find((i) => i.itemName === itemID) || null;
}

export function addItemToInventory(playerID, itemID, qty = 1) {
	const player = loadPlayer(playerID);
	if (!player) return false;
	player.inventory = player.inventory || {};
	player.inventory.items = player.inventory.items || {};
	const key = String(itemID);
	player.inventory.items[key] = (player.inventory.items[key] || 0) + qty;
	savePlayer(player);
	return true;
}

export function removeItemFromInventory(playerID, itemID, qty = 1) {
	const player = loadPlayer(playerID);
	if (!player || !player.inventory || !player.inventory.items) return false;
	const key = String(itemID);
	const have = player.inventory.items[key] || 0;
	if (have < qty) return false;
	player.inventory.items[key] = have - qty;
	if (player.inventory.items[key] <= 0) delete player.inventory.items[key];
	savePlayer(player);
	return true;
}

export function equipItem(playerID, itemID, slot) {
	const player = loadPlayer(playerID);
	if (!player) return false;
	player.inventory = player.inventory || {};
	player.inventory.equipment = player.inventory.equipment || {};
	const item = loadItem(itemID);
	if (!item) return false;
	// ensure removed from items list if present
	removeItemFromInventory(playerID, itemID, 1);
	player.inventory.equipment[slot] = item;
	savePlayer(player);
	return true;
}

export function unequipItem(playerID, slot) {
	const player = loadPlayer(playerID);
	if (!player || !player.inventory || !player.inventory.equipment) return false;
	const item = player.inventory.equipment[slot];
	if (!item) return false;
	// add back to items
	player.inventory.items = player.inventory.items || {};
	const key = String(item.itemID || item.itemName);
	player.inventory.items[key] = (player.inventory.items[key] || 0) + 1;
	delete player.inventory.equipment[slot];
	savePlayer(player);
	return true;
}

//Event handlers
export function onItemPickup() {}
export function onItemDrop() {}
export function onItemEquip() {}
export function onItemBreak() {}
export function onItemUpgrade() {}

export function getItemStats(itemID) {
	const it = loadItem(itemID);
	return it ? it.itemStats || {} : {};
}

export function applyItemModifiers(baseStats = {}, mods = {}) {
	const out = { ...baseStats };
	Object.entries(mods || {}).forEach(([k, v]) => {
		out[k] = (out[k] || 0) + v;
	});
	return out;
}

export function checkItemRequirements(playerID, itemID) {
	const player = loadPlayer(playerID);
	const item = loadItem(itemID);
	if (!player || !item) return false;
	const reqs = item.itemReqs || {};
	if (reqs.level && player.LVL < reqs.level) return false;
	return true;
}
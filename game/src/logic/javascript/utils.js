//imports
import fs from "fs";
import path from "path";

//Primary
export function loadJSON(filePath) {
  try {
    const raw = fs.readFileSync(filePath, "utf-8");
    return JSON.parse(raw);
  } catch (err) {
    console.error(`[JS] Failed to load JSON: ${filePath}`, err);
    return null;
  }
}
export function saveJSON(filePath, data) {
  try {
    const dir = path.dirname(filePath);
    fs.mkdirSync(dir, { recursive: true });

    fs.writeFileSync(
      filePath,
      JSON.stringify(data, null, 2),
      "utf-8"
    );
    return true;
  } catch (err) {
    console.error(`[JS] Failed to save JSON: ${filePath}`, err);
    return false;
  }
}


//Helpers
export function getTimeMS() {
  return Date.now();
}
export function generateUUID() {
  return crypto.randomUUID();
}
export function randomInt(min, max) {
  return Math.floor(Math.random() * (max - min + 1)) + min;
}
export function clamp(value, min, max) {
  return Math.max(min, Math.min(max, value));
}
export function deepClone(obj) {
  return JSON.parse(JSON.stringify(obj));
}

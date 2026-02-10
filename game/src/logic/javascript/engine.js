import { initGame, loadAssets, spawnPlayer } from "./main.js";
import { spawnEnemy, updateEnemyAI, enemyTakeDamage } from "./entities/enemy.js";
import { loadPlayer, savePlayer } from "./player/player.js";
import { calculateDamage, tickPlayerTimers } from "./player/combat.js";
import { loadJSON } from "./utils.js";

async function runSimulation() {
  initGame();
  loadAssets();

  const player = spawnPlayer(0);
  if (!player) {
    console.error('No player found to spawn');
    return;
  }
  console.log('Spawned player:', player.displayName || player.username);

  // spawn all enemies defined in assets
  const enemiesAsset = loadJSON(process.cwd() + "/game/assets/PLAY/enemies.json") || [];
  const spawnedEnemies = [];
  for (const e of enemiesAsset) {
    const coords = (e.position && (e.position.coords || { X: (e.position.X||0), Y: (e.position.Y||0) })) || { X: 0, Y: 0 };
    const loc = { position: { mapID: e.position ? (e.position.mapID||1) : 1, X: coords.X || (player.position.X+50), Y: coords.Y || player.position.Y } };
    const inst = spawnEnemy(e.enemyID, loc);
    if (inst) spawnedEnemies.push(inst);
    console.log('Spawned enemy:', e.enemyName, 'at', loc.position.X, loc.position.Y);
  }

  // simple tick loop
  let tick = 0;
  while (tick < 200) {
    // player auto-attack cycle: target nearest spawned enemy
    try {
      const p = loadPlayer(player.playerID);
      if (!p) break;
      // find an alive enemy
      for (const en of spawnedEnemies.slice()) {
        if (!en || !en.instanceID) continue;
        const dmg = calculateDamage(p, en, {});
        enemyTakeDamage(en.instanceID, dmg);
        break; // one attack per tick
      }
    } catch (e) {}

    // enemy AI performs attacks on player
    for (const en of spawnedEnemies.slice()) {
      if (en && en.instanceID) updateEnemyAI(en.instanceID);
    }

    // tick timers for player
    tickPlayerTimers(player.playerID, 1000);

    // reload player to get updated HP
    const pReload = loadPlayer(player.playerID);
    const pHP = pReload && pReload.stats ? pReload.stats.HP : 0;
    if (pHP <= 0) {
      console.log('Player has died.');
      break;
    }

    // sleep 50ms
    await new Promise((r) => setTimeout(r, 50));
    tick++;
  }
  console.log('Simulation finished. ticks:', tick);
}

runSimulation().catch((e) => console.error(e));

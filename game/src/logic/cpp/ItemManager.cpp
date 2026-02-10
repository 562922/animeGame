#include "ItemManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

static std::string readFileSimple(const std::string &path) {
  std::ifstream in(path);
  if (!in) return "";
  std::ostringstream ss; ss << in.rdbuf();
  return ss.str();
}

ItemManager::ItemManager(const std::string &assetsRoot) : assetsRoot(assetsRoot) {}

std::vector<ItemInfo> ItemManager::listItems() const {
  std::vector<ItemInfo> out;
  std::string f = assetsRoot + "/ITEM/items.json";
  auto data = readFileSimple(f);
  // naive parsing: find "itemName"
  size_t pos = 0;
  while (true) {
    pos = data.find("\"itemName\"", pos);
    if (pos == std::string::npos) break;
    auto colon = data.find(':', pos);
    auto start = data.find('"', colon+1);
    if (start == std::string::npos) break;
    auto end = data.find('"', start+1);
    if (end == std::string::npos) break;
    std::string name = data.substr(start+1, end-start-1);
    out.push_back({0, name});
    pos = end+1;
  }
  return out;
}

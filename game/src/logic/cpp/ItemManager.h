#pragma once
#include <string>
#include <vector>

struct ItemInfo { int itemID; std::string itemName; };

class ItemManager {
public:
  ItemManager(const std::string &assetsRoot);
  std::vector<ItemInfo> listItems() const;
private:
  std::string assetsRoot;
};

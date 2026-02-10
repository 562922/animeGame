# Unreal Engine 5 Integration Guide

This guide shows how to integrate the AnimeGame C++ engine into an Unreal Engine 5 project as a reusable game module.

## Quick Start

### 1. Copy GameLogic to Your Unreal Project

```bash
# In your Unreal project root (e.g., ~/Projects/MyGame)
mkdir -p Source/ThirdParty/AnimeGame/GameLogic
cp -r /path/to/animeGame/game/src/logic/cpp/* Source/ThirdParty/AnimeGame/GameLogic/
cp -r /path/to/animeGame/game/assets Source/ThirdParty/AnimeGame/
```

### 2. Create a Module Build File

**YourGame/Source/AnimeGameLogic/AnimeGameLogic.Build.cs:**

```csharp
using UnrealBuildTool;
using System.IO;

public class AnimeGameLogicTarget : ModuleRules {
  public AnimeGameLogicTarget(ReadOnlyTargetRules Target) : base(Target) {
    Type = ModuleType.External;
    
    // Include paths for GameLogic headers
    PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "..", "ThirdParty", "AnimeGame", "GameLogic"));
    
    // For Windows: add compiled .lib
    // For Mac/Linux: add compiled .a
    string LibPath = Path.Combine(ModuleDirectory, "..", "ThirdParty", "AnimeGame", "Lib");
    if (Target.Platform == UnrealTargetPlatform.Win64) {
      PublicAdditionalLibraries.Add(Path.Combine(LibPath, "GameLogic.lib"));
    } else if (Target.Platform == UnrealTargetPlatform.Mac) {
      PublicAdditionalLibraries.Add(Path.Combine(LibPath, "libGameLogic.a"));
    } else if (Target.Platform == UnrealTargetPlatform.Linux) {
      PublicAdditionalLibraries.Add(Path.Combine(LibPath, "libGameLogic.a"));
    }
  }
}
```

Then in **YourGame/Source/YourGame/YourGame.Build.cs**:

```csharp
PublicDependencies.AddRange(new string[] {
  "Core",
  "CoreUObject",
  "Engine",
  "AnimeGameLogic"
});
```

### 3. Create a Wrapper Actor

**YourGame/Source/YourGame/Public/AnimeGameManager.h:**

```cpp
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameLogic.h"
#include "AnimeGameManager.generated.h"

UCLASS()
class YOURGAME_API AAnimeGameManager : public AActor {
  GENERATED_BODY()

public:
  AAnimeGameManager();
  virtual void BeginPlay() override;
  virtual void Tick(float DeltaTime) override;

  UFUNCTION(BlueprintCallable, Category = "AnimeGame")
  void InitializeGame();

  UFUNCTION(BlueprintCallable, Category = "AnimeGame")
  FVector GetPlayerPosition() const;

  UFUNCTION(BlueprintCallable, Category = "AnimeGame")
  FVector GetEnemyPosition() const;

  UFUNCTION(BlueprintCallable, Category = "AnimeGame")
  int32 GetPlayerHealth() const;

  UFUNCTION(BlueprintCallable, Category = "AnimeGame")
  int32 GetEnemyHealth() const;

private:
  GameLogic* GameEngine;
  bool bGameInitialized;
};
```

**YourGame/Source/YourGame/Private/AnimeGameManager.cpp:**

```cpp
#include "AnimeGameManager.h"
#include "Kismet/GameplayStatics.h"

AAnimeGameManager::AAnimeGameManager() {
  PrimaryActorTick.bCanEverTick = true;
  bGameInitialized = false;
  GameEngine = nullptr;
}

void AAnimeGameManager::BeginPlay() {
  Super::BeginPlay();
  InitializeGame();
}

void AAnimeGameManager::InitializeGame() {
  if (bGameInitialized) return;

  // Get the project's base directory and construct asset path
  FString AssetsPath = FPaths::ProjectDir() + TEXT("Source/ThirdParty/AnimeGame/assets");
  std::string AssetPathStr(TCHAR_TO_UTF8(*AssetsPath));

  // Create and initialize game engine
  GameEngine = new GameLogic(AssetPathStr);
  if (GameEngine->loadAssets()) {
    UE_LOG(LogTemp, Warning, TEXT("AnimeGame initialized successfully"));
    bGameInitialized = true;
  } else {
    UE_LOG(LogTemp, Error, TEXT("Failed to load AnimeGame assets"));
  }
}

void AAnimeGameManager::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  if (GameEngine && bGameInitialized) {
    GameEngine->update(DeltaTime);
  }
}

FVector AAnimeGameManager::GetPlayerPosition() const {
  if (!GameEngine) return FVector::ZeroVector;
  
  Player p = GameEngine->getPlayer();
  return FVector(p.position.X, 0, p.position.Y);  // Adjust Z mapping as needed
}

FVector AAnimeGameManager::GetEnemyPosition() const {
  if (!GameEngine) return FVector::ZeroVector;
  
  Enemy e = GameEngine->getEnemy();
  return FVector(e.position.X, 0, e.position.Y);
}

int32 AAnimeGameManager::GetPlayerHealth() const {
  if (!GameEngine) return 0;
  return GameEngine->getPlayer().stats.HP;
}

int32 AAnimeGameManager::GetEnemyHealth() const {
  if (!GameEngine) return 0;
  return GameEngine->getEnemy().stats.HP;
}
```

### 4. Use in a Blueprint or Pawn

**Create a Character Blueprint:**

1. Right-click in Content Browser → Create Blueprint Class → Character
2. Set the parent class to your ANimeGameManager (or use the wrapper actor)
3. In the viewport, place the actor in your level
4. In the Blueprint Details, you can now call the AnimeGame functions

**Or use in a GameMode:**

```cpp
// In your GameMode.cpp
void AYourGameMode::BeginPlay() {
  Super::BeginPlay();
  
  // Spawn the game manager
  AAnimeGameManager* GameMgr = GetWorld()->SpawnActor<AAnimeGameManager>();
  GameMgr->InitializeGame();
}
```

## Integrating with Unreal's Pawn/Character System

To fully integrate with Unreal's character system, you can override movement and animation in a Character class:

```cpp
void AMyCharacter::BeginPlay() {
  Super::BeginPlay();
  
  // Get the game manager (find in world, or cache a reference)
  AAnimeGameManager* GameMgr = Cast<AAnimeGameManager>(
    UGameplayStatics::GetActorOfClass(GetWorld(), AAnimeGameManager::StaticClass())
  );
  
  if (GameMgr) {
    cached_GameMgr = GameMgr;
  }
}

void AMyCharacter::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
  
  if (cached_GameMgr) {
    // Update this character's position from game engine
    FVector PlayerPos = cached_GameMgr->GetPlayerPosition();
    SetActorLocation(PlayerPos);
  }
}
```

## Building for Different Platforms

### Windows (MSVC)
```bash
cd /path/to/animeGame/build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
# Output: game/src/logic/cpp/Release/GameLogic.lib
```

### macOS
```bash
cd /path/to/animeGame/build
cmake .. -G "Xcode"
cmake --build . --config Release
# Output: game/src/logic/cpp/libGameLogic.a
```

### Linux
```bash
cd /path/to/animeGame/build
cmake ..
make -j$(nproc)
# Output: game/src/logic/cpp/libGameLogic.a
```

## Asset Management

Place the `game/assets/` folder in your Unreal project:
```
YourProject/
├── Source/
├── Content/
├── Binaries/
└── Assets/                    # Copy game/assets here
    ├── ITEM/
    ├── NPC/
    ├── PLAY/
    └── ROOMS/
```

Update the asset path in `AnimeGameManager.cpp`:
```cpp
FString AssetsPath = FPaths::ProjectDir() + TEXT("Assets");
```

## Performance Considerations

1. **Load assets once** in BeginPlay or at game startup, not every frame.
2. **Cache player/enemy references** instead of calling getPlayer/getEnemy every frame.
3. **Use sync vs. async loading**: For large asset sets, consider async file I/O.
4. **Memory pooling**: Pre-allocate enemy instances to avoid runtime allocations during combat.

## Troubleshooting

### Compiler Errors: "Undefined reference to GameLogic::"
- Ensure the `.lib` / `.a` file path in `.Build.cs` is correct
- Rebuild the GameLogic library before rebuilding Unreal project
- Check that the library was built for the same platform

### Runtime Errors: "Cannot find assets"
- Verify the asset path string in `InitializeGame()` is correct
- Check that `game/assets/` is in the right location
- Use `FPaths` helpers to construct platform-independent paths

### Blueprint Not Finding Functions
- Ensure the wrapper class is declared with `UCLASS()` / `UFUNCTION(BlueprintCallable)`
- Recompile the module and refresh the Blueprint

## Example: Full Combat Loop

```cpp
// In your character's update
if (GameMgr) {
  // Get positions
  FVector PlayerPos = GameMgr->GetPlayerPosition();
  FVector EnemyPos = GameMgr->GetEnemyPosition();
  
  // Calculate distance
  float Distance = FVector::Dist(PlayerPos, EnemyPos);
  
  // Update UI
  if (HealthProgressBar) {
    float HealthPercent = (float)GameMgr->GetPlayerHealth() / 100.0f;
    HealthProgressBar->SetPercent(HealthPercent);
  }
  
  // Move character
  SetActorLocation(PlayerPos);
  
  // Rotate toward enemy
  FVector Direction = (EnemyPos - PlayerPos).GetSafeNormal();
  SetActorRotation(Direction.Rotation());
}
```

## Next Steps

- **Extend GameLogic** with more features (shops, skill trees, etc.)
- **Add animations** to character meshes using Unreal's Skeletal Mesh system
- **Implement networking** using Unreal's replication system
- **Create UI** using Unreal's UMG Slate widgets

Happy developing in Unreal! 🎮

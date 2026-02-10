# Unreal Engine Integration Guide

This guide provides detailed instructions for integrating the AnimeGame C++ game logic library into Unreal Engine 5.x.

## Project Structure Overview

The AnimeGame library provides:

- **GameLogic**: Core orchestrator managing all game systems
- **UIManager**: Game state machine and UI panel management
- **InputHandler**: Input binding and command queue system
- **SaveSystem**: Player/world state persistence
- **DialogueSystem**: NPC dialogue tree system
- **SkillSystem**: Skill database and cooldown management
- **Player**: Complete player data structure with inventory, quests, skills
- **Enemy/Combat**: Combat system and enemy data structures
- **Asset Loading**: JSON-based game data loading

## Prerequisites

- Unreal Engine 5.0 or later
- Visual Studio 2022 (Windows) or Xcode (macOS)
- C++17 standard support

## Integration Steps

### 1. Copy Game Logic to Unreal Project

```bash
# Copy the C++ source files to your Unreal project
cp -r /workspaces/animeGame/game/src/logic/cpp/ YourProject/Source/AnimeGame/Logic/

# Copy asset files to the project
cp -r /workspaces/animeGame/game/assets/ YourProject/Content/AnimeGame/Assets/
```

### 2. Update Project Module (.Build.cs)

Create or update `YourProject/Source/AnimeGame/AnimeGame.Build.cs`:

```csharp
using UnrealBuildTool;

public class AnimeGame : ModuleRules
{
    public AnimeGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "Slate",
            "SlateCore"
        });
        
        // Add game logic include paths
        PublicIncludePaths.AddRange(new string[] {
            System.IO.Path.Combine(ModuleDirectory, "Logic"),
            System.IO.Path.Combine(ModuleDirectory, "Logic/cpp")
        });
        
        // Add source files
        PublicSourcePaths.Add(System.IO.Path.Combine(ModuleDirectory, "Logic/cpp"));
    }
}
```

### 3. Create Wrapper Actor

Create `YourProject/Source/AnimeGame/Public/AnimeGameManager.h`:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameLogic.h"
#include "InputActionValue.h"
#include "AnimeGameManager.generated.h"

class UInputMappingContext;
class UInputAction;
class APlayerCharacter;

UCLASS()
class ANIMEGAME_API AAnimeGameManager : public AActor
{
    GENERATED_BODY()

public:
    AAnimeGameManager();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Game initialization
    UFUNCTION(BlueprintCallable, Category = "AnimeGame")
    void InitializeGame(const FString& PlayerName);

    UFUNCTION(BlueprintCallable, Category = "AnimeGame")
    void LoadGame(const FString& SaveSlot);

    UFUNCTION(BlueprintCallable, Category = "AnimeGame")
    void SaveGame(const FString& SaveSlot);

    // Game state
    UFUNCTION(BlueprintCallable, Category = "AnimeGame")
    void StartGame();

    UFUNCTION(BlueprintCallable, Category = "AnimeGame")
    void PauseGame();

    UFUNCTION(BlueprintCallable, Category = "AnimeGame")
    void ResumeGame();

    // Player interaction
    UFUNCTION(BlueprintCallable, Category = "AnimeGame")
    void MovePlayer(float DX, float DY);

    UFUNCTION(BlueprintCallable, Category = "AnimeGame")
    void UseSkill(const FString& SkillID);

    UFUNCTION(BlueprintCallable, Category = "AnimeGame")
    void InteractWithNPC(const FString& NPCID);

    UFUNCTION(BlueprintCallable, Category = "AnimeGame")
    void AcceptQuest(const FString& QuestID);

    // Data accessors
    UFUNCTION(BlueprintCallable, Category = "AnimeGame")
    FString GetPlayerName() const;

    UFUNCTION(BlueprintCallable, Category = "AnimeGame")
    int32 GetPlayerLevel() const;

    UFUNCTION(BlueprintCallable, Category = "AnimeGame")
    int32 GetPlayerHP() const;

    UFUNCTION(BlueprintCallable, Category = "AnimeGame")
    int32 GetPlayerMaxHP() const;

    UFUNCTION(BlueprintCallable, Category = "AnimeGame")
    int32 GetPlayerMana() const;

    UFUNCTION(BlueprintCallable, Category = "AnimeGame")
    int32 GetPlayerMaxMana() const;

    UFUNCTION(BlueprintCallable, Category = "AnimeGame")
    bool IsGameRunning() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimeGame")
    FString AssetsPath;

    UPROPERTY()
    class UInputMappingContext* DefaultMappingContext;

    UPROPERTY()
    class UInputAction* MoveAction;

    UPROPERTY()
    class UInputAction* LookAction;

private:
    std::unique_ptr<GameLogic> GameLogicInstance;
    float AccumulatedDeltaTime;

    void SetupInput();
    void HandleInput(const FInputActionValue& Value);
};
```

Create `YourProject/Source/AnimeGame/Private/AnimeGameManager.cpp`:

```cpp
#include "AnimeGameManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

AAnimeGameManager::AAnimeGameManager()
    : AccumulatedDeltaTime(0.0f)
{
    PrimaryActorTick.TickInterval = 0.016f;  // 60 FPS
    PrimaryActorTick.bCanEverTick = true;

    // Get assets path
    AssetsPath = FPaths::ProjectContentDir() + TEXT("AnimeGame/Assets");
}

void AAnimeGameManager::BeginPlay()
{
    Super::BeginPlay();

    // Initialize game logic
    std::string AssetsCStr = std::string(TCHAR_TO_UTF8(*AssetsPath));
    GameLogicInstance = std::make_unique<GameLogic>(AssetsCStr);

    SetupInput();
}

void AAnimeGameManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (GameLogicInstance && GameLogicInstance->isRunning())
    {
        AccumulatedDeltaTime += DeltaTime;

        // Update game logic
        GameLogicInstance->update(DeltaTime);

        // Sync Unreal world state with game logic
        // (Update actor positions, animations, etc.)
    }
}

void AAnimeGameManager::InitializeGame(const FString& PlayerName)
{
    if (GameLogicInstance)
    {
        std::string NameCStr = std::string(TCHAR_TO_UTF8(*PlayerName));
        GameLogicInstance->initializeNewGame(NameCStr);
        GameLogicInstance->startGame();
    }
}

void AAnimeGameManager::LoadGame(const FString& SaveSlot)
{
    if (GameLogicInstance)
    {
        std::string SlotCStr = std::string(TCHAR_TO_UTF8(*SaveSlot));
        GameLogicInstance->loadGame(SlotCStr);
    }
}

void AAnimeGameManager::SaveGame(const FString& SaveSlot)
{
    if (GameLogicInstance)
    {
        GameLogicInstance->quickSave();
    }
}

void AAnimeGameManager::StartGame()
{
    if (GameLogicInstance)
    {
        GameLogicInstance->startGame();
    }
}

void AAnimeGameManager::PauseGame()
{
    if (GameLogicInstance)
    {
        GameLogicInstance->pauseGame();
    }
}

void AAnimeGameManager::ResumeGame()
{
    if (GameLogicInstance)
    {
        GameLogicInstance->resumeGame();
    }
}

void AAnimeGameManager::MovePlayer(float DX, float DY)
{
    if (GameLogicInstance)
    {
        GameLogicInstance->movePlayer(DX, DY);
    }
}

void AAnimeGameManager::UseSkill(const FString& SkillID)
{
    if (GameLogicInstance)
    {
        std::string SkillCStr = std::string(TCHAR_TO_UTF8(*SkillID));
        GameLogicInstance->useSkill(SkillCStr, "");
    }
}

void AAnimeGameManager::InteractWithNPC(const FString& NPCID)
{
    if (GameLogicInstance)
    {
        std::string NPCCStr = std::string(TCHAR_TO_UTF8(*NPCID));
        GameLogicInstance->interactWithNPC(NPCCStr);
    }
}

void AAnimeGameManager::AcceptQuest(const FString& QuestID)
{
    if (GameLogicInstance)
    {
        std::string QuestCStr = std::string(TCHAR_TO_UTF8(*QuestID));
        GameLogicInstance->acceptQuest(QuestCStr, "");
    }
}

FString AAnimeGameManager::GetPlayerName() const
{
    if (GameLogicInstance)
    {
        Player player = GameLogicInstance->getPlayer();
        return FString(player.displayName.c_str());
    }
    return FString();
}

int32 AAnimeGameManager::GetPlayerLevel() const
{
    if (GameLogicInstance)
    {
        return GameLogicInstance->getPlayer().level;
    }
    return 0;
}

int32 AAnimeGameManager::GetPlayerHP() const
{
    if (GameLogicInstance)
    {
        return GameLogicInstance->getPlayer().currentHP;
    }
    return 0;
}

int32 AAnimeGameManager::GetPlayerMaxHP() const
{
    if (GameLogicInstance)
    {
        return GameLogicInstance->getPlayer().maxHP;
    }
    return 100;
}

int32 AAnimeGameManager::GetPlayerMana() const
{
    if (GameLogicInstance)
    {
        return GameLogicInstance->getPlayer().currentMana;
    }
    return 0;
}

int32 AAnimeGameManager::GetPlayerMaxMana() const
{
    if (GameLogicInstance)
    {
        return GameLogicInstance->getPlayer().maxMana;
    }
    return 50;
}

bool AAnimeGameManager::IsGameRunning() const
{
    if (GameLogicInstance)
    {
        return GameLogicInstance->isRunning();
    }
    return false;
}

void AAnimeGameManager::SetupInput()
{
    // Setup input mapping context
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void AAnimeGameManager::HandleInput(const FInputActionValue& Value)
{
    if (GameLogicInstance)
    {
        // Convert Unreal input to game logic commands
        // This will be called from Blueprint or player controller
    }
}
```

### 4. Blueprint Setup in Unreal Editor

1. **Create Level Blueprint**:
   - Open your level in Unreal Editor
   - Open the level Blueprint
   - Add an instance of AAnimeGameManager
   - Call "Initialize Game" with player name on BeginPlay

2. **Create Player Character Blueprint**:
   - Create a character blueprint inheriting from ACharacter
   - Add mesh and animation components
   - Bind input to AAnimeGameManager events

3. **Create UI Widgets**:
   - Create UMG widgets for HUD, inventory, dialogue, menus
   - Bind to GameLogic data through AAnimeGameManager accessors

### 5. Asset Configuration

Store game assets in: `YourProject/Content/AnimeGame/Assets/`

Folder structure:
```
Content/AnimeGame/Assets/
├── ITEM/
│   ├── items.json
│   ├── mods.json
│   └── skills.json
├── NPC/
│   ├── npcs.json
│   └── dialogue.json
├── PLAY/
│   ├── enemies.json
│   ├── maps.json
│   └── quests.json
└── ROOMS/
    └── dungeon.json
```

## Key Integration Points

### 1. Game Loop Integration

```cpp
void AAnimeGameManager::Tick(float DeltaTime)
{
    // Update game logic
    GameLogicInstance->update(DeltaTime);
    
    // Sync with Unreal world
    SyncPlayerPosition();
    UpdateEnemyPositions();
    UpdateNPCPositions();
}
```

### 2. Input Handling

```cpp
void ACharacterController::SetupInput()
{
    // Bind Unreal input to game logic
    if (AAnimeGameManager* GameManager = GetGameManager())
    {
        InputComponent->BindAction("MoveForward", IE_Pressed, 
            GameManager, &AAnimeGameManager::MovePlayer, 
            1.0f, 0.0f);
    }
}
```

### 3. UI Synchronization

```cpp
void APlayerHUD::Tick(float DeltaTime)
{
    if (AAnimeGameManager* GameManager = GetGameManager())
    {
        HPBar->SetPercent(GameManager->GetPlayerHP() / static_cast<float>(GameManager->GetPlayerMaxHP()));
        LevelText->SetText(FText::AsNumber(GameManager->GetPlayerLevel()));
    }
}
```

## Performance Considerations

1. **Asset Loading**: Cache JSON parsing results in native Unreal data structures
2. **Dialogue System**: Pre-compile dialogue trees into optimized format
3. **Combat Calculations**: Consider moving heavy calculations to dedicated compute shaders
4. **Save System**: Use Unreal's save system (USaveGame) instead of plain file I/O

## Data Serialization Recommendations

### For Production Use:

1. **Replace naive JSON parsing with nlohmann::json**:
   ```bash
   # Add to CMakeLists.txt dependencies
   find_package(nlohmann_json REQUIRED)
   target_link_libraries(GameLogic PUBLIC nlohmann_json::nlohmann_json)
   ```

2. **Update Parsing**:
   ```cpp
   #include <nlohmann/json.hpp>
   using json = nlohmann::json;
   
   std::ifstream file(filepath);
   auto data = json::parse(file);
   // Access data["field"]["subfield"]
   ```

3. **Create Unreal-compatible serialization**:
   ```cpp
   // Serialize to FStructs
   void SerializeToUnreal(const Player& InPlayer, FPlayerData& OutData)
   {
       OutData.PlayerName = FString(InPlayer.displayName.c_str());
       OutData.Level = InPlayer.level;
       // ... map all fields
   }
   ```

## Debugging

### Enable Logging

```cpp
// Add to GameLogic.cpp
#if UE_BUILD_DEBUG
    UE_LOG(LogAnimeGame, Warning, TEXT("Asset loading started"));
#endif
```

### Verify Integration

1. Check `Output Log` for asset loading messages
2. Use `stat unit` console command to monitor frame time
3. Debug NPC positions and dialogue trees in Editor viewport

## Next Steps

1. Implement player animation controller with game logic
2. Create dynamic NPC spawning from asset data
3. Build full dialogue UI with branching support
4. Implement inventory system UI
5. Add combat VFX and animations
6. Create dungeon instance management

## Support Resources

- [Unreal Engine C++ Documentation](https://docs.unrealengine.com/5.0/en-US/programming-in-cplusplus/)
- [Enhanced Input System](https://docs.unrealengine.com/5.0/en-US/enhanced-input-user-guide/)
- [UMG UI Framework](https://docs.unrealengine.com/5.0/en-US/umg-ui-designer-users-guide/)
- [Save System](https://docs.unrealengine.com/5.0/en-US/save-system-in-unreal-engine/)

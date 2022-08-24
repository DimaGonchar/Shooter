// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "STUCoreTypes.h"
#include "STUSaveGame.generated.h"

/**
 * 
 */

struct FLevelData;

UCLASS(BlueprintType, Blueprintable)
class SHOOTTHEMUP_API USTUSaveGame : public USaveGame
{
	GENERATED_BODY()

protected:

    UPROPERTY(EditAnywhere)
    FString SloteName;
    
    UFUNCTION(BlueprintCallable)
    static USTUSaveGame* GetInstance();

private:
    static USTUSaveGame* CreateInstance();

    UPROPERTY()
    FLevelData LevelDate;
    
    UPROPERTY()
    FWeaponData WeaponData;
    
    UPROPERTY()
    FWeaponUIData WeaponUIData;
    
    UPROPERTY()
    FVector PlayerLocation;

    UPROPERTY()
    FRotator PlayerRotation;

    UPROPERTY()
    uint32 CountDeadEnemies;

    UPROPERTY()
    float PlayerHelth = 0.f;

    UPROPERTY()
    uint32 CountAmmo = 0;

    UPROPERTY()
    float TotalMoney = 0.f;
    
    static USTUSaveGame* Self;
};

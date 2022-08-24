// Shoot Them Up Game, All Rights Reserved.


#include "STUSaveGame.h"

USTUSaveGame* USTUSaveGame::Self = nullptr;

USTUSaveGame* USTUSaveGame:: GetInstance()
{
    if(!Self)
    {
        return CreateInstance();
    }
    return Self;
}

USTUSaveGame* USTUSaveGame:: CreateInstance()
{
    if(!Self)
    {
        Self = NewObject<USTUSaveGame>(nullptr, USTUSaveGame::StaticClass(), NAME_None, RF_NoFlags, USTUSaveGame::StaticClass());
        return  Self;
    }
    return  Self;
}
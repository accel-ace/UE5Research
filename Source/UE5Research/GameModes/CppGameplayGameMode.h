// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"

#include "CppGameplayGameMode.generated.h"

class ACharacter;

USTRUCT(BlueprintType)
struct FGameResult
{
    GENERATED_BODY()
    
public:
    UPROPERTY(BlueprintReadOnly)
    int32 DefeatedEnemyCount = 0;
    
    UPROPERTY(BlueprintReadOnly)
    float SurvivalTime = 0.0f;
    
    UPROPERTY(BlueprintReadOnly)
    int32 Score = 0;
};

/**
 *
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOverStarted);

UCLASS()
class UE5RESEARCH_API ACppGameplayGameMode : public AGameModeBase
{
    GENERATED_BODY()
    
public:
    ACppGameplayGameMode();
    
    virtual void PostLogin(APlayerController* NewPlayer) override;

    UFUNCTION(BlueprintCallable)
    void HandlePlayerDeath(ACharacter* DeadPlayer);
    
    UPROPERTY(BlueprintAssignable, Category = "Game Over")
    FOnGameOverStarted OnGameOverStarted;
    
    UFUNCTION(BlueprintCallable, Category = "Game")
    void StartGameOver(ACharacter* DeadPlayer);
    
    UFUNCTION(BlueprintPure, Category = "Game Result")
    const FGameResult& GetGameResult() const;
    
    UFUNCTION(BlueprintCallable, Category = "Game Result")
    void AddDefeatedEnemyCount();
    
    void RestartCurrentGame();
    
    bool IsMatchFinished() const
    {
        return bMatchFinished;
    }

protected:
    virtual void BeginPlay() override;
    
    void UpdateGameResult();
    
    UPROPERTY(BlueprintReadOnly, Category = "Game Result")
    FGameResult GameResult;
    
    UPROPERTY(BlueprintReadOnly, Category = "Game")
    bool bIsGameOver = false;
    
    float GameStartTime = 0.0f;

    void ResetGameResult();

    void CalculateScore();    

private:
    bool bRestartRequested = false;
    
    bool bMatchFinished = false;
};

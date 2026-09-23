// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ResearchPlayerState.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FOnGameResultUpdated,
    int32,
    DefeatedEnemyCount,
    float,
    SurvivalTime
);

UCLASS()
class UE5RESEARCH_API AResearchPlayerState : public APlayerState
{
    GENERATED_BODY()
    
public:
    AResearchPlayerState();
    
    void AddDefeatedEnemyCount();
    
    void AddScoreValue(int32 Amount);
    
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    
    UFUNCTION(BlueprintCallable, Category = "Game Result")
    void SetDefeatedEnemyCount(int32 NewCount);
    
    UFUNCTION(BlueprintPure, Category = "Game Result")
    int32 GetDefeatedEnemyCount() const
    {
        return DefeatedEnemyCount;
    }
    
    UFUNCTION(BlueprintCallable, Category = "Game Result")
    void SetSurvivalTime(float NewSurvivalTime);
    
    UFUNCTION(BlueprintPure, Category = "Game Result")
    float GetSurvivalTime() const
    {
        return SurvivalTime;
    }
    
    UFUNCTION(BlueprintPure, Category = "Research|Score")
    int32 GetPlayerScore() const
    {
        return PlayerScore;
    }

    UPROPERTY(BlueprintAssignable, Category = "Game Result")
    FOnGameResultUpdated OnGameResultUpdated;
    
protected:
    UPROPERTY(ReplicatedUsing = OnRep_DefeatedEnemyCount, BlueprintReadOnly, Category = "Player Result")
    int32 DefeatedEnemyCount = 0;
    
    UPROPERTY(ReplicatedUsing = OnRep_SurvivalTime, BlueprintReadOnly, Category = "Game Result")
    float SurvivalTime = 0.0f;
    
    UPROPERTY(ReplicatedUsing = OnRep_PlayerScore, BlueprintReadOnly, Category = "Research|Score")
    int32 PlayerScore = 0;

    UFUNCTION()
    void OnRep_DefeatedEnemyCount();
    
    UFUNCTION()
    void OnRep_SurvivalTime();

    UFUNCTION()
    void OnRep_PlayerScore();
};

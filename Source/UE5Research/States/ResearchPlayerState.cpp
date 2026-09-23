// Fill out your copyright notice in the Description page of Project Settings.


#include "States/ResearchPlayerState.h"
#include "Net/UnrealNetwork.h"

AResearchPlayerState::AResearchPlayerState()
{
    bReplicates = true;
    DefeatedEnemyCount = 0;
    SurvivalTime = 0.0f;
}

void AResearchPlayerState::AddDefeatedEnemyCount()
{
    if (!HasAuthority())
    {
        return;
    }
    
    ++DefeatedEnemyCount;
    OnRep_DefeatedEnemyCount();
}

void AResearchPlayerState::AddScoreValue(const int32 Amount)
{
    if (!HasAuthority())
    {
        UE_LOG(
            LogTemp,
            Log,
            TEXT(
                "AddScoreValue ignored | PlayerState: %s | No server authority"
            ),
            *GetName()
        );

        return;
    }

    const int32 PreviousScore = PlayerScore;

    PlayerScore = FMath::Max(
        0,
        PlayerScore + Amount
    );

    UE_LOG(
        LogTemp,
        Log,
        TEXT(
            "Score changed on server | PlayerState: %s | Previous: %d | Current: %d | NetMode: %d"
        ),
        *GetName(),
        PreviousScore,
        PlayerScore,
        static_cast<int32>(GetNetMode())
    );
}

void AResearchPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(
        AResearchPlayerState,
        DefeatedEnemyCount
    );

    DOREPLIFETIME(
        AResearchPlayerState,
        SurvivalTime
    );
}

void AResearchPlayerState::SetDefeatedEnemyCount(int32 NewCount)
{
    if (!HasAuthority())
    {
        return;
    }

    DefeatedEnemyCount = NewCount;
    OnRep_DefeatedEnemyCount();
}

void AResearchPlayerState::SetSurvivalTime(float NewSurvivalTime)
{
    if (!HasAuthority())
    {
        return;
    }

    SurvivalTime = FMath::Max(0.0f, NewSurvivalTime);
    OnRep_SurvivalTime();
}

void AResearchPlayerState::OnRep_DefeatedEnemyCount()
{
    OnGameResultUpdated.Broadcast(
        DefeatedEnemyCount,
        SurvivalTime
    );
}

void AResearchPlayerState::OnRep_SurvivalTime()
{
    OnGameResultUpdated.Broadcast(
        DefeatedEnemyCount,
        SurvivalTime
    );
}

void AResearchPlayerState::OnRep_PlayerScore()
{
    UE_LOG(
        LogTemp,
        Log,
        TEXT(
            "OnRep_PlayerScore | PlayerState: %s | Score: %d | NetMode: %d"
        ),
        *GetName(),
        PlayerScore,
        static_cast<int32>(GetNetMode())
    );
}

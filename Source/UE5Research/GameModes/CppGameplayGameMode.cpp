// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModes/CppGameplayGameMode.h"

#include "Engine/World.h"
#include "OnlineSubsystemUtils.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameStateBase.h"
#include "States/ResearchPlayerState.h"
#include "Controllers/ResearchPlayerController.h"

ACppGameplayGameMode::ACppGameplayGameMode()
{
    PlayerStateClass = AResearchPlayerState::StaticClass();

    bIsGameOver = false;
    GameStartTime = 0.0f;
}

void ACppGameplayGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    UE_LOG(
        LogTemp,
        Log,
        TEXT("Player joined | Controller: %s | Players: %d"),
        *GetNameSafe(NewPlayer),
        GetNumPlayers()
    );
}

void ACppGameplayGameMode::AddDefeatedEnemyCount()
{
    if (bIsGameOver)
    {
        return;
    }
    
    ++GameResult.DefeatedEnemyCount;
}

void ACppGameplayGameMode::BeginPlay()
{
    Super::BeginPlay();

    ResetGameResult();
}

void ACppGameplayGameMode::StartGameOver(ACharacter* DeadPlayer)
{
    // ゲームオーバー処理の多重実行を防ぐ
    if (bIsGameOver)
    {
        return;
    }
    
    bIsGameOver = true;
    
    GameResult.SurvivalTime =
        GetWorld()->GetTimeSeconds() - GameStartTime;

    CalculateScore();
    
    AResearchPlayerState* ResearchPlayerState =
    DeadPlayer->GetPlayerState<AResearchPlayerState>();
    
    if (ResearchPlayerState)
    {
        ResearchPlayerState->SetDefeatedEnemyCount(GameResult.DefeatedEnemyCount);
        
        const float CurrentTimeSeconds = GetWorld()->GetTimeSeconds();
        const float SurvivalTime =
        CurrentTimeSeconds - GameStartTime;
        
        ResearchPlayerState->SetSurvivalTime(SurvivalTime);
    }
    else
    {
        UE_LOG(
           LogTemp,
           Log,
           TEXT("ResearchPlayerState was not found on dead player.")
       );
    }
    
    for (FConstPlayerControllerIterator It =
             GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (AResearchPlayerController* Controller =
                Cast<AResearchPlayerController>(It->Get()))
        {
            UE_LOG(
                LogTemp,
                Log,
                TEXT("Sending GameOver | Controller: %s | LocalOnServer: %s"),
                *Controller->GetName(),
                Controller->IsLocalController()
                    ? TEXT("true")
                    : TEXT("false")
            );

            Controller->ClientShowGameOver();
        }
    }
    // Blueprint側へゲームオーバー開始を通知
    OnGameOverStarted.Broadcast();
}

const FGameResult& ACppGameplayGameMode::GetGameResult() const
{
    return GameResult;
}

void ACppGameplayGameMode::HandlePlayerDeath(ACharacter* DeadPlayer)
{
    if (!DeadPlayer)
    {
        UE_LOG(LogTemp, Log, TEXT("HandlePlayerDeath called, but DeadPlayer is null."));
        return;
    }
    
    if (!HasAuthority())
    {
        return;
    }
    
    if (!IsValid(DeadPlayer))
    {
        UE_LOG(
            LogTemp,
            Log,
            TEXT("HandlePlayerDeath rejected: DeadCharacter is invalid"));
        return;
    }
    
    if (bMatchFinished)
    {
        UE_LOG(
            LogTemp,
            Log,
            TEXT("HandlePlayerDeath skipped: match already finished"));
        return;
    }
    
    bMatchFinished = true;

    APlayerController* DeadController =
        Cast<APlayerController>(DeadPlayer->GetController());
    
    APlayerController* WinnerController = nullptr;
    
    
    for (FConstPlayerControllerIterator Iterator =
             GetWorld()->GetPlayerControllerIterator();
         Iterator;
         ++Iterator)
    {
        APlayerController* PlayerController = Iterator->Get();

        if (!IsValid(PlayerController))
        {
            continue;
        }

        if (PlayerController != DeadController)
        {
            WinnerController = PlayerController;
            break;
        }
    }

    UE_LOG(
        LogTemp,
        Log,
        TEXT("Match finished | Loser:%s | Winner:%s"),
        *GetNameSafe(DeadController),
        *GetNameSafe(WinnerController));
    
    GameResult.SurvivalTime = GetWorld()->GetTimeSeconds() - GameStartTime;

    // リザルト確定
    UpdateGameResult();
    
    StartGameOver(DeadPlayer);
    
    if (AResearchPlayerController* LoserController =
            Cast<AResearchPlayerController>(DeadController))
    {
        LoserController->ClientShowMatchResult(false);
    }

    if (AResearchPlayerController* WinnerResearchController =
            Cast<AResearchPlayerController>(WinnerController))
    {
        WinnerResearchController->ClientShowMatchResult(true);
    }

}

void ACppGameplayGameMode::UpdateGameResult()
{
    if (!GameState)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("UpdateGameResult: GameState is null")
        );
        return;
    }

    for (APlayerState* PlayerState : GameState->PlayerArray)
    {
        AResearchPlayerState* ResearchPlayerState =
            Cast<AResearchPlayerState>(PlayerState);

        if (!ResearchPlayerState)
        {
            continue;
        }

        ResearchPlayerState->SetDefeatedEnemyCount(
            GameResult.DefeatedEnemyCount
        );

        ResearchPlayerState->SetSurvivalTime(
            GameResult.SurvivalTime
        );
        
        ResearchPlayerState->ForceNetUpdate();

    }
}

void ACppGameplayGameMode::ResetGameResult()
{
    GameResult.DefeatedEnemyCount = 0;
    GameResult.SurvivalTime = 0.0f;

    if (UWorld* World = GetWorld())
    {
        GameStartTime = World->GetTimeSeconds();
    }
}

void ACppGameplayGameMode::CalculateScore()
{
    const int32 EnemyScore =
    GameResult.DefeatedEnemyCount * 100;
    
    const int32 SurvivalScore =
    FMath::FloorToInt(GameResult.SurvivalTime * 10.0f);
    
    GameResult.Score = EnemyScore + SurvivalScore;
}

void ACppGameplayGameMode::RestartCurrentGame()
{
    if (bRestartRequested)
    {
        return;
    }

    bRestartRequested = true;

    if (UWorld* World = GetWorld())
    {
        FString CurrentMapName = World->GetMapName();

#if WITH_EDITOR
        // PIEで付加される UEDPIE_0_ などを除去
        CurrentMapName.RemoveFromStart(World->StreamingLevelsPrefix);
#endif

        World->ServerTravel(CurrentMapName);
    }
}

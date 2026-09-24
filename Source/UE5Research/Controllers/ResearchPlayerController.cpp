// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/ResearchPlayerController.h"
#include "Engine/World.h"
#include "GameModes/CppGameplayGameMode.h"
#include "States/ResearchPlayerState.h"
#include "EnhancedInputSubsystems.h"
#include "Widgets/Input/SVirtualJoystick.h"
#include "Characters/PlayerCharacter.h"
#include "SubSystem/ResearchSessionSubsystem.h"

void AResearchPlayerController::ClientShowGameOver_Implementation()
{
    OnGameOverRequested();
}

void AResearchPlayerController::ServerRequestRetry_Implementation()
{
    if (ACppGameplayGameMode* GameMode =
            GetWorld()->GetAuthGameMode<ACppGameplayGameMode>())
    {
        GameMode->RestartCurrentGame();
    }
}

void AResearchPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (IsLocalPlayerController())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
        {
            for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
            {
                Subsystem->AddMappingContext(CurrentContext, 0);
            }

            if (!ShouldUseTouchControls())
            {
                for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
                {
                    Subsystem->AddMappingContext(CurrentContext, 0);
                }
            }
        }
    }
}

void AResearchPlayerController::HandleAddScoreInput()
{
    UE_LOG(
        LogTemp,
        Log,
        TEXT("AddScore input | Controller: %s | Local: %s"),
        *GetName(),
        IsLocalController() ? TEXT("true") : TEXT("false")
    );

    Server_AddScore();
}

void AResearchPlayerController::RequestAddScore()
{
    UE_LOG(
        LogTemp,
        Log,
        TEXT(
            "AddScore requested | "
            "Controller: %s | "
            "LocalController: %s | "
            "Authority: %s | "
            "NetMode: %d"
        ),
        *GetName(),
        IsLocalController() ? TEXT("true") : TEXT("false"),
        HasAuthority() ? TEXT("true") : TEXT("false"),
        static_cast<int32>(GetNetMode())
    );

    if (!IsLocalController())
    {
        UE_LOG(
            LogTemp,
            Log,
            TEXT(
                "RequestAddScore ignored because "
                "this is not a local PlayerController."
            )
        );

        return;
    }

    Server_AddScore();
}

void AResearchPlayerController::Server_AddScore_Implementation()
{
    UE_LOG(
        LogTemp,
        Log,
        TEXT(
            "Server_AddScore received | Controller: %s | Authority: %s | NetMode: %d"
        ),
        *GetName(),
        HasAuthority() ? TEXT("true") : TEXT("false"),
        static_cast<int32>(GetNetMode())
    );

    if (!HasAuthority())
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT(
                "Server_AddScore was executed without server authority."
            )
        );

        return;
    }

    AResearchPlayerState* ResearchPlayerState =
        GetPlayerState<AResearchPlayerState>();

    if (!IsValid(ResearchPlayerState))
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT(
                "Server_AddScore failed | ResearchPlayerState is null | Controller: %s"
            ),
            *GetName()
        );

        return;
    }

    ResearchPlayerState->AddScoreValue(1);
}

bool AResearchPlayerController::ShouldUseTouchControls() const
{
    return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}

void AResearchPlayerController::ClientShowMatchResult_Implementation(
    bool bIsWinner)
{
    SetIgnoreMoveInput(true);
    SetIgnoreLookInput(true);

    UE_LOG(
        LogTemp,
        Log,
        TEXT("ClientShowMatchResult | Controller:%s | Result:%s"),
        *GetNameSafe(this),
        bIsWinner ? TEXT("WIN") : TEXT("LOSE"));
    
    if (APlayerCharacter* PlayerCharacter =
            Cast<APlayerCharacter>(GetPawn()))
    {
        PlayerCharacter->SetCombatInputEnabled(false);
    }

    ShowMatchResult(bIsWinner);
}

void AResearchPlayerController::NotifyRemoteClientsToDestroySession()
{
    if (!HasAuthority())
    {
        UE_LOG(
            LogTemp,
            Log,
            TEXT("NotifyRemoteClientsToDestroySession skipped: no authority"));

        return;
    }

    UWorld* World = GetWorld();

    if (!IsValid(World))
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("NotifyRemoteClientsToDestroySession failed: invalid World"));

        return;
    }

    for (FConstPlayerControllerIterator Iterator =
             World->GetPlayerControllerIterator();
         Iterator;
         ++Iterator)
    {
        AResearchPlayerController* PlayerController =
            Cast<AResearchPlayerController>(Iterator->Get());

        if (!IsValid(PlayerController))
        {
            continue;
        }

        // Listen Server自身は除外する。
        if (PlayerController->IsLocalController())
        {
            continue;
        }

        UE_LOG(
            LogTemp,
            Log,
            TEXT("Requesting remote client session destroy | Controller:%s"),
            *GetNameSafe(PlayerController));

        PlayerController->Client_DestroyLocalSession();
    }
}

void AResearchPlayerController::Client_DestroyLocalSession_Implementation()
{
    UE_LOG(
        LogTemp,
        Log,
        TEXT("Client_DestroyLocalSession received | Controller:%s"),
        *GetNameSafe(this));

    UResearchSessionSubsystem* SessionSubsystem =
        GetGameInstance()->GetSubsystem<UResearchSessionSubsystem>();

    if (!IsValid(SessionSubsystem))
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("Client_DestroyLocalSession failed: invalid SessionSubsystem"));

        return;
    }

    SessionSubsystem->DestroySession();
}

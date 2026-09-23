// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"

#include "ResearchPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UE5RESEARCH_API AResearchPlayerController : public APlayerController
{
	GENERATED_BODY()
    
public:
    UFUNCTION(Client, Reliable)
    void ClientShowGameOver();

    UFUNCTION(BlueprintImplementableEvent, Category = "Game Over")
    void OnGameOverRequested();

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Game Over")
    void ServerRequestRetry();
    
    UFUNCTION(BlueprintCallable, Category = "Research|Score")
    void RequestAddScore();
    
    UFUNCTION(Client, Reliable)
    void ClientShowMatchResult(bool bIsWinner);

    UFUNCTION(BlueprintImplementableEvent, Category = "Match")
    void ShowMatchResult(bool bIsWinner);
    
    UFUNCTION(BlueprintCallable, Category = "Session")
    void NotifyRemoteClientsToDestroySession();

    UFUNCTION(Client, Reliable)
    void Client_DestroyLocalSession();
    
protected:
    virtual void SetupInputComponent() override;

    void HandleAddScoreInput();

    UFUNCTION(Server, Reliable)
    void Server_AddScore();

    UPROPERTY(EditAnywhere, Config, Category = "Input|Touch Controls")
    bool bForceTouchControls = false;
    
    /** Input Mapping Contexts */
    UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
    TArray<UInputMappingContext*> DefaultMappingContexts;

    /** Input Mapping Contexts */
    UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
    TArray<UInputMappingContext*> MobileExcludedMappingContexts;

    bool ShouldUseTouchControls() const;
};

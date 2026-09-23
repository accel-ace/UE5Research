// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "ResearchSessionSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FResearchSessionListItem
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Online Session")
    int32 SessionIndex = INDEX_NONE;

    UPROPERTY(BlueprintReadOnly, Category = "Online Session")
    FString HostName;

    UPROPERTY(BlueprintReadOnly, Category = "Online Session")
    int32 CurrentPlayers = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Online Session")
    int32 MaxPlayers = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Online Session")
    int32 PingInMs = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FOnFindSessionsFinishedBP,
    bool,
    bWasSuccessful,
    const TArray<FResearchSessionListItem>&,
    Results
);

UCLASS()
class UE5RESEARCH_API UResearchSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
    UResearchSessionSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    
    UFUNCTION(BlueprintCallable, Category = "Online Session")
    void CreateSession();

    UFUNCTION(BlueprintCallable, Category = "Online Session")
    void FindSessions();

    UFUNCTION(BlueprintCallable, Category = "Online Session")
    void JoinSessionByIndex(int32 SessionIndex);
    
    UPROPERTY(BlueprintAssignable, Category = "Online Session")
    FOnFindSessionsFinishedBP OnFindSessionsFinishedBP;
    
    UFUNCTION(BlueprintCallable, Category = "Online Session")
    void DestroySession();
    
    UFUNCTION(BlueprintCallable, Category = "Online Session")
    void LeaveSession();

private:
    void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

    void OnFindSessionsComplete(bool bWasSuccessful);
    
    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
    
    void HandleDestroySessionComplete(FName SessionName, bool bWasSuccessful);
    
    void OnLeaveSessionComplete(FName SessionName, bool bWasSuccessful);

    void ReturnToSessionBrowser();

    IOnlineSessionPtr SessionInterface;

    FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;

    FDelegateHandle CreateSessionCompleteDelegateHandle;

    FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;

    FDelegateHandle FindSessionsCompleteDelegateHandle;

    TSharedPtr<FOnlineSessionSearch> SessionSearch;

    FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;

    FDelegateHandle JoinSessionCompleteDelegateHandle;

    FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;

    FDelegateHandle DestroySessionCompleteDelegateHandle;
    
    FOnDestroySessionCompleteDelegate LeaveSessionCompleteDelegate;

    FDelegateHandle LeaveSessionDelegateHandle;
    
};

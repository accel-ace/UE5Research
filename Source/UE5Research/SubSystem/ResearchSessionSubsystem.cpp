// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/ResearchSessionSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

namespace ResearchSession
{
    static const FName Name(TEXT("ResearchSession"));
}

UResearchSessionSubsystem::UResearchSessionSubsystem()
    : CreateSessionCompleteDelegate(
        FOnCreateSessionCompleteDelegate::CreateUObject(
            this,
            &UResearchSessionSubsystem::OnCreateSessionComplete
        )
    )
    , FindSessionsCompleteDelegate(
        FOnFindSessionsCompleteDelegate::CreateUObject(
            this,
            &UResearchSessionSubsystem::OnFindSessionsComplete
        )
    )
    , JoinSessionCompleteDelegate(
        FOnJoinSessionCompleteDelegate::CreateUObject(
            this,
            &UResearchSessionSubsystem::OnJoinSessionComplete
        )
    )
    , DestroySessionCompleteDelegate(
        FOnDestroySessionCompleteDelegate::CreateUObject(
            this,
            &UResearchSessionSubsystem::HandleDestroySessionComplete
        )
    )
{
}

void UResearchSessionSubsystem::Initialize(
    FSubsystemCollectionBase& Collection
)
{
    Super::Initialize(Collection);

    IOnlineSubsystem* OnlineSubsystem =
        IOnlineSubsystem::Get();

    if (!OnlineSubsystem)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("OnlineSubsystem could not be obtained.")
        );
        return;
    }

    SessionInterface =
        OnlineSubsystem->GetSessionInterface();

    LeaveSessionCompleteDelegate =
        FOnDestroySessionCompleteDelegate::CreateUObject(
            this,
            &UResearchSessionSubsystem::OnLeaveSessionComplete);
}

void UResearchSessionSubsystem::CreateSession()
{
    UE_LOG(LogTemp, Log, TEXT("CreateSession called"));
    
    if (UWorld* World = GetWorld())
    {
        const FString LevelName = World->GetMapName();

        UE_LOG(
            LogTemp,
            Log,
            TEXT("Opening listen server | PID: %u | World: %p | Level: %s"),
            FPlatformProcess::GetCurrentProcessId(),
            GetWorld(),
            *LevelName
       );
    }
    
    if (!SessionInterface.IsValid())
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("CreateSession failed: SessionInterface is invalid.")
        );
        return;
    }
    
    const FNamedOnlineSession* ExistingSession =
        SessionInterface->GetNamedSession(ResearchSession::Name);
    
    const EOnlineSessionState::Type SessionState =
        SessionInterface->GetSessionState(ResearchSession::Name);

    if (ExistingSession != nullptr)
    {
        UE_LOG(
            LogTemp,
            Log,
            TEXT("CreateSession skipped: GameSession already exists.")
        );
        return;
    }

    FOnlineSessionSettings SessionSettings;
    static const FName RoomNameKey(TEXT("ROOM_NAME"));

    SessionSettings.Set(
        RoomNameKey,
        FString(TEXT("テストルーム")),
        EOnlineDataAdvertisementType::ViaOnlineServiceAndPing
    );
    SessionSettings.bIsLANMatch = true;
    SessionSettings.NumPublicConnections = 2;
    SessionSettings.bShouldAdvertise = true;
    SessionSettings.bAllowJoinInProgress = true;

    CreateSessionCompleteDelegateHandle =
        SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
            CreateSessionCompleteDelegate
        );

    const bool bRequestStarted = SessionInterface->CreateSession(
        0,
        ResearchSession::Name,
        SessionSettings
    );
    
    if (!bRequestStarted)
    {
        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(
            CreateSessionCompleteDelegateHandle
        );

        UE_LOG(
            LogTemp,
            Error,
            TEXT("CreateSession request could not be started.")
        );

        return;
    }

    UE_LOG(
        LogTemp,
        Log,
        TEXT("CreateSession request started.")
    );
}

void UResearchSessionSubsystem::FindSessions()
{
    UE_LOG(LogTemp, Log, TEXT("FindSessions called"));

    if (!SessionInterface.IsValid())
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("FindSessions aborted: SessionInterface is invalid.")
        );
        return;
    }

    SessionSearch = MakeShared<FOnlineSessionSearch>();
    SessionSearch->bIsLanQuery = true;
    SessionSearch->MaxSearchResults = 100;

    FindSessionsCompleteDelegateHandle =
        SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(
            FindSessionsCompleteDelegate
        );

    const bool bRequestStarted =
        SessionInterface->FindSessions(
            0,
            SessionSearch.ToSharedRef()
        );

    if (!bRequestStarted)
    {
        SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(
            FindSessionsCompleteDelegateHandle
        );

        UE_LOG(
            LogTemp,
            Error,
            TEXT("FindSessions request could not be started.")
        );

        return;
    }

    UE_LOG(
        LogTemp,
        Log,
        TEXT("FindSessions request accepted.")
    );
}

void UResearchSessionSubsystem::OnFindSessionsComplete(
    bool bWasSuccessful
)
{
    if (SessionSearch.IsValid())
    {
        for (int32 Index = 0;
             Index < SessionSearch->SearchResults.Num();
             ++Index)
        {
            const FOnlineSessionSearchResult& Result =
                SessionSearch->SearchResults[Index];

            UE_LOG(
                LogTemp,
                Log,
                TEXT(
                    "SearchResult[%d] | Valid:%s | Owner:%s | OpenConnections:%d | Ping:%d"),
                Index,
                Result.IsValid() ? TEXT("true") : TEXT("false"),
                *Result.Session.OwningUserName,
                Result.Session.NumOpenPublicConnections,
                Result.PingInMs);
        }
    }



    TArray<FResearchSessionListItem> DisplayResults;

    if (!bWasSuccessful || !SessionSearch.IsValid())
    {
        UE_LOG(
            LogTemp,
            Log,
            TEXT("FindSessions failed.")
        );

        OnFindSessionsFinishedBP.Broadcast(
            false,
            DisplayResults
        );

        return;
    }
    
    const TArray<FOnlineSessionSearchResult>& SearchResults =
        SessionSearch->SearchResults;

    DisplayResults.Reserve(SearchResults.Num());
    static const FName RoomNameKey(TEXT("ROOM_NAME"));
    
    for (int32 Index = 0; Index < SearchResults.Num(); ++Index)
    {
        const FOnlineSessionSearchResult& SearchResult =
            SearchResults[Index];

        FResearchSessionListItem ListItem;

        ListItem.SessionIndex = Index;
        
        FString DisplayRoomName;
        const bool bHasRoomName =
            SearchResult.Session.SessionSettings.Get(
                RoomNameKey,
                DisplayRoomName
            );

        if (!bHasRoomName || DisplayRoomName.IsEmpty())
        {
            // 古いセッションなど、ROOM_NAMEが入っていない場合の代替表示
            DisplayRoomName = SearchResult.Session.OwningUserName;
        }

        ListItem.HostName = DisplayRoomName;

        ListItem.MaxPlayers =
            SearchResult.Session.SessionSettings
                .NumPublicConnections;

        ListItem.CurrentPlayers = FMath::Max(
            0,
            ListItem.MaxPlayers -
                SearchResult.Session
                    .NumOpenPublicConnections
        );

        ListItem.PingInMs =
            SearchResult.PingInMs;

        DisplayResults.Add(ListItem);

        UE_LOG(
            LogTemp,
            Log,
            TEXT(
                "Session UI Result | Index: %d | Host: %s | Players: %d/%d | Ping: %d"
            ),
            ListItem.SessionIndex,
            *ListItem.HostName,
            ListItem.CurrentPlayers,
            ListItem.MaxPlayers,
            ListItem.PingInMs
        );
    }

    OnFindSessionsFinishedBP.Broadcast(
        true,
        DisplayResults
    );
}

void UResearchSessionSubsystem::JoinSessionByIndex(
    int32 SessionIndex)
{
    UE_LOG(LogTemp, Log, TEXT("JoinSession called"));

    if (!SessionSearch.IsValid())
    {
        UE_LOG(
            LogTemp,
            Log,
            TEXT("Join failed: SessionSearch is invalid.")
        );
        return;
    }

    if (!SessionSearch->SearchResults.IsValidIndex(
        SessionIndex))
    {
        UE_LOG(
            LogTemp,
            Log,
            TEXT("Join failed: Invalid index %d"),
            SessionIndex
        );
        return;
    }

    const FOnlineSessionSearchResult& SearchResult =
        SessionSearch->SearchResults[SessionIndex];

    JoinSessionCompleteDelegateHandle =
        SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(
            JoinSessionCompleteDelegate
        );

    const bool bRequestStarted = SessionInterface->JoinSession(
        0,
        ResearchSession::Name,
        SearchResult
    );

    if (!bRequestStarted)
    {
        SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(
            JoinSessionCompleteDelegateHandle
        );

        UE_LOG(
            LogTemp,
            Error,
            TEXT("JoinSession request could not be started.")
        );

        return;
    }

    UE_LOG(
        LogTemp,
        Log,
        TEXT("JoinSession request accepted.")
    );
}

void UResearchSessionSubsystem::OnCreateSessionComplete(
    FName SessionName,
    bool bWasSuccessful
)
{
    UE_LOG(
        LogTemp,
        Log,
        TEXT("CreateSession called | Map: %s"),
        *GetWorld()->GetMapName()
    );
    
    const FNamedOnlineSession* ExistingSession =
        SessionInterface->GetNamedSession(ResearchSession::Name);

    if (SessionInterface.IsValid())
    {
        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(
            CreateSessionCompleteDelegateHandle
        );
    }
    
    if (!bWasSuccessful)
    {
        return;
    }

    UWorld* World = GetWorld();

    if (!IsValid(World))
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("OpenLevel failed: World is invalid.")
        );
        return;
    }

    const FName LevelName(TEXT("/Game/Learning/PvP/Maps/L_PvPTest"));
    UGameplayStatics::OpenLevel(
        this,
        LevelName,
        true,
        TEXT("listen?Port=7777")
    );
}

void UResearchSessionSubsystem::OnJoinSessionComplete(
    FName SessionName,
    EOnJoinSessionCompleteResult::Type Result
)
{
    if (SessionInterface.IsValid())
    {
        SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(
            JoinSessionCompleteDelegateHandle
        );
    }

    UE_LOG(
        LogTemp,
        Log,
        TEXT("JoinSession completed | Name: %s | Result: %d"),
        *SessionName.ToString(),
        static_cast<int32>(Result)
    );
    
    if (Result != EOnJoinSessionCompleteResult::Success)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("JoinSession failed.")
        );
        return;
    }

    FString ConnectString;

    if (!SessionInterface->GetResolvedConnectString(
            ResearchSession::Name,
            ConnectString
        ))
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("Failed to resolve session connect string.")
        );
        return;
    }

    UWorld* World = GetWorld();

    if (!IsValid(World))
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("ClientTravel failed: World is invalid.")
        );
        return;
    }

    APlayerController* PlayerController =
        World->GetFirstPlayerController();

    if (!IsValid(PlayerController))
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("ClientTravel failed: PlayerController is invalid.")
        );
        return;
    }

    UE_LOG(
        LogTemp,
        Log,
        TEXT("ClientTravel | Address: %s"),
        *ConnectString
    );

    PlayerController->ClientTravel(
        ConnectString,
        TRAVEL_Absolute
    );
}

void UResearchSessionSubsystem::DestroySession()
{
    UE_LOG(LogTemp, Log, TEXT("DestroySession called"));

    const bool bSessionExists =
        SessionInterface.IsValid() &&
        SessionInterface->GetNamedSession(ResearchSession::Name) != nullptr;

    UE_LOG(
        LogTemp,
        Log,
        TEXT("DestroySession called | PID:%d | SessionExists:%s"),
        FPlatformProcess::GetCurrentProcessId(),
        bSessionExists ? TEXT("true") : TEXT("false"));

    if (!SessionInterface.IsValid())
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("DestroySession failed: SessionInterface is invalid.")
        );
        return;
    }

    const FNamedOnlineSession* ExistingSession =
        SessionInterface->GetNamedSession(ResearchSession::Name);

    if (ExistingSession == nullptr)
    {
        UE_LOG(
            LogTemp,
            Log,
            TEXT("DestroySession skipped: GameSession does not exist.")
        );
        return;
    }

    DestroySessionCompleteDelegateHandle =
        SessionInterface
            ->AddOnDestroySessionCompleteDelegate_Handle(
                DestroySessionCompleteDelegate
            );

    const bool bRequestStarted =
        SessionInterface->DestroySession(ResearchSession::Name);

    if (!bRequestStarted)
    {
        SessionInterface
            ->ClearOnDestroySessionCompleteDelegate_Handle(
                DestroySessionCompleteDelegateHandle
            );

        UE_LOG(
            LogTemp,
            Error,
            TEXT("DestroySession request could not be started.")
        );
    }
}

void UResearchSessionSubsystem::HandleDestroySessionComplete(
    FName SessionName,
    bool bWasSuccessful)
{
    UE_LOG(
        LogTemp,
        Log,
        TEXT("DestroySession completed | PID:%d | Name:%s | Success:%s"),
        FPlatformProcess::GetCurrentProcessId(),
        *SessionName.ToString(),
        bWasSuccessful ? TEXT("true") : TEXT("false"));

    if (SessionInterface.IsValid())
    {
        SessionInterface
            ->ClearOnDestroySessionCompleteDelegate_Handle(
                DestroySessionCompleteDelegateHandle
            );
    }

    if (!bWasSuccessful)
    {
        UE_LOG(LogTemp, Error, TEXT("DestroySession failed."));
        return;
    }

    UE_LOG(
        LogTemp,
        Log,
        TEXT("GameSession cleanup completed.")
    );
}

void UResearchSessionSubsystem::LeaveSession()
{
    UE_LOG(LogTemp, Log, TEXT("LeaveSession called"));

    if (!SessionInterface.IsValid())
    {
        UE_LOG(
            LogTemp,
            Log,
            TEXT("LeaveSession: SessionInterface is invalid"));

        ReturnToSessionBrowser();
        return;
    }

    const FNamedOnlineSession* ExistingSession =
        SessionInterface->GetNamedSession(ResearchSession::Name);

    if (ExistingSession == nullptr)
    {
        UE_LOG(
            LogTemp,
            Log,
            TEXT("LeaveSession: GameSession does not exist"));

        ReturnToSessionBrowser();
        return;
    }

    LeaveSessionDelegateHandle =
        SessionInterface
            ->AddOnDestroySessionCompleteDelegate_Handle(
                LeaveSessionCompleteDelegate);

    const bool bStarted =
        SessionInterface->DestroySession(ResearchSession::Name);

    UE_LOG(
        LogTemp,
        Log,
        TEXT("DestroySession started: %s"),
        bStarted ? TEXT("true") : TEXT("false"));

    if (!bStarted)
    {
        SessionInterface
            ->ClearOnDestroySessionCompleteDelegate_Handle(
                LeaveSessionDelegateHandle);

        ReturnToSessionBrowser();
    }
}

void UResearchSessionSubsystem::OnLeaveSessionComplete(
    FName SessionName,
    bool bWasSuccessful)
{
    UE_LOG(
        LogTemp,
        Log,
        TEXT("OnLeaveSessionComplete | Session:%s | Success:%s"),
        *SessionName.ToString(),
        bWasSuccessful ? TEXT("true") : TEXT("false"));

    if (SessionInterface.IsValid())
    {
        SessionInterface
            ->ClearOnDestroySessionCompleteDelegate_Handle(
                LeaveSessionDelegateHandle);
    }

    ReturnToSessionBrowser();
}

void UResearchSessionSubsystem::ReturnToSessionBrowser()
{
    UGameplayStatics::OpenLevel(
        this,
        FName(TEXT("L_SessionBrowser")));
}

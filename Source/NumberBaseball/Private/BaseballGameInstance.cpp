#include "BaseballGameInstance.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

static const FName SEARCH_PRESENCE(TEXT("SEARCH_PRESENCE"));


UBaseballGameInstance::UBaseballGameInstance()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		SessionInterface = OnlineSub->GetSessionInterface();
	}
}

void UBaseballGameInstance::CreateGameSession(FName SessionName)
{
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("세션 인터페이스가 유효하지 않음"));
		return;
	}

	SessionInterface->OnCreateSessionCompleteDelegates.RemoveAll(this);
	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UBaseballGameInstance::OnCreateSessionComplete);

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = false;
	SessionSettings.NumPublicConnections = 2;	// 최대 플레이어 수
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;

	// 세션 생성 (LocalUserNum 0 사용)
	SessionInterface->CreateSession(0, SessionName, SessionSettings);
}

void UBaseballGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		OnCreateSession.Broadcast(SessionName);
	}
}

void UBaseballGameInstance::FindGameSessions()
{
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("세션 인터페이스가 유효하지 않음"));
		return;
	}

	SessionInterface->OnFindSessionsCompleteDelegates.RemoveAll(this);
	SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UBaseballGameInstance::OnFindSessionsComplete);

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = false;
	SessionSearch->MaxSearchResults = 5;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UBaseballGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Log, TEXT("Find sessions complete: %d"), bWasSuccessful);
	if (bWasSuccessful && SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("세션 갯수 : %d"), SessionSearch->SearchResults.Num());
		for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++)
		{
			const FOnlineSessionSearchResult& Result = SessionSearch->SearchResults[i];

			FString OwningUser = Result.Session.OwningUserName;

			FString CustomSessionName;
			Result.Session.SessionSettings.Get(TEXT("SESSION_NAME"), CustomSessionName);

			int32 MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
			int32 CurrentPlayers = MaxPlayers - Result.Session.NumOpenPublicConnections;

			OnSessionFound.Broadcast(CustomSessionName, MaxPlayers, CurrentPlayers);
		}
	}
}

void UBaseballGameInstance::JoinGameSession(int32 SessionIndex)
{
	if (!SessionInterface.IsValid() || !SessionSearch.IsValid() || SessionSearch->SearchResults.Num() <= SessionIndex)
	{
		UE_LOG(LogTemp, Warning, TEXT("JoinGameSession failed due to invalid session info."));
		return;
	}

	SessionInterface->OnJoinSessionCompleteDelegates.RemoveAll(this);
	SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UBaseballGameInstance::OnJoinSessionComplete);

	SessionInterface->JoinSession(0, NAME_GameSession, SessionSearch->SearchResults[SessionIndex]);
}

void UBaseballGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Log, TEXT("Join session complete: %s, Result: %d"), *SessionName.ToString(), (int32)Result);

	// 성공 시 접속 URL을 얻어 클라이언트 맵 전환을 수행
	FString ConnectString;
	if (SessionInterface->GetResolvedConnectString(SessionName, ConnectString))
	{
		APlayerController* PlayerController = GetFirstLocalPlayerController();
		if (PlayerController)
		{
			PlayerController->ClientTravel(ConnectString, TRAVEL_Absolute);
		}
	}
}

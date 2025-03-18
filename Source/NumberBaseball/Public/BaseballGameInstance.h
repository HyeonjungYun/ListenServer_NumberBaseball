#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "BaseballGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnSessionFound, 
	const FString&, CustomSessionName, 
	int32, MaxPlayers, 
	int32, CurrentPlayers
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnCreateSession,
	const FName&, CustomSessionName
);

UCLASS()
class NUMBERBASEBALL_API UBaseballGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	FOnSessionFound OnSessionFound;
	FOnCreateSession OnCreateSession;

protected:
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	void OnFindSessionsComplete(bool bWasSuccessful);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

private:
	// �¶��� ���� �������̽�
	IOnlineSessionPtr SessionInterface;

	// ���� �˻� ���
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	
public:
	UBaseballGameInstance();

	UFUNCTION(BlueprintCallable)
	void CreateGameSession(FName SessionName);

	UFUNCTION(BlueprintCallable)
	void FindGameSessions();

	UFUNCTION(BlueprintCallable)
	void JoinGameSession(int32 SessionIndex);
};

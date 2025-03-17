#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BaseballGameState.generated.h"

UCLASS()
class NUMBERBASEBALL_API ABaseballGameState : public AGameState
{
	GENERATED_BODY()

/*----------PROPERTY----------*/
public:
	// 승부 판별 변수
	int VictoryUsers;

#pragma region 타이머 핸들 변수

	UPROPERTY()
	FTimerHandle GameStartTimer;

	UPROPERTY()
	FTimerHandle GamePlayingTimer;

	UPROPERTY()
	FTimerHandle SubmittionTurnTimer;

	UPROPERTY()
	FTimerHandle TurnOverTimer;

#pragma endregion 타이머 핸들 변수

	// 정답
	UPROPERTY(VisibleAnywhere, Category = "TargetNumber")
	FString RandNumber;

/*----------FUNCTION----------*/
private:

#pragma region 생성자, virtual 함수

	ABaseballGameState();

	virtual void BeginPlay() override;

#pragma endregion 생성자, virtual 함수

public:
	FString SetRandomNumber();
	void GameStart();
	void CountVictoryUsers();

	UFUNCTION(BlueprintCallable)
	void InitGame();

	UFUNCTION(Blueprintpure, Category = "Number")
	FString GetRandomNumber();

	UFUNCTION()
	void NotifyMatchResult();

#pragma region 리퀘스트 함수

	UFUNCTION(BlueprintCallable)
	void RequestActivateAllSubmitButton();

	UFUNCTION(BlueprintCallable)
	void RequestDeactivateAllSubmitButton();

	UFUNCTION(BlueprintCallable)
	void RequestDeactivateSubmitButton();

#pragma endregion 리퀘스트 함수

#pragma region 클라이언트 함수

	UFUNCTION(Client, Reliable)
	void Client_DeactiveSubmitButton();

	UFUNCTION(Client, Reliable)
	void Client_NotifyVictory();

	UFUNCTION(Client, Reliable)
	void Client_NotifyDefeat();

#pragma endregion 클라이언트 함수

#pragma region 멀티캐스트 함수

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReceiveMessage(APlayerState* SenderState, const FString& Message);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReceiveCheckResult(APlayerState* SenderState, const FString& Result);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_GameStart();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BeTimeToSubmit();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OverTurnToSubmit();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_NotifyTurnStart();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_NotifyTurnOver();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_NotifyDraw();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CheckMatchResult();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_RequestSendResult();

#pragma endregion 멀티캐스트 함수
};

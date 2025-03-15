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
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> ChatWidget;

	UPROPERTY()
	FTimerHandle GameStartTimer;

	UPROPERTY()
	FTimerHandle GamePlayingTimer;

	UPROPERTY()
	FTimerHandle SubmittionTurnTimer;

	UPROPERTY()
	FTimerHandle TurnOverTimer;

/*----------FUNCTION----------*/
private:
	ABaseballGameState();

	virtual void BeginPlay() override;

public:
	FString SetRandomNumber();
	void GameStart();

	UFUNCTION(BlueprintCallable)
	void InitGame();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReceiveMessage(APlayerState* SenderState, const FString& Message);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReceiveCheckResult(const FString& Result);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_GameStart();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BeTimeToSubmit();

	UFUNCTION(BlueprintCallable)
	void RequestActivateAllSubmitButton();

	UFUNCTION(BlueprintCallable)
	void RequestDeactivateAllSubmitButton();

	UFUNCTION(BlueprintCallable)
	void RequestDeactivateSubmitButton();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OverTurnToSubmit();

	UFUNCTION(Client, Reliable)
	void Client_DeactiveSubmitButton();
};

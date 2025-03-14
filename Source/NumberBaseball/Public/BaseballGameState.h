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

/*----------FUNCTION----------*/
private:
	ABaseballGameState();

	virtual void BeginPlay() override;

public:
	FString SetRandomNumber();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReceiveMessage(APlayerState* SenderState, const FString& Message);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReceiveCheckResult(const FString& Result);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_GameStart();
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BaseballPlayerState.generated.h"

UCLASS()
class NUMBERBASEBALL_API ABaseballPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	FString BaseballPlayerName;

	UFUNCTION(BlueprintPure)
	const FString GetBaseballPlayerName() const;

	UFUNCTION(BlueprintCallable)
	void SetBaseballPlayerName(const FString& PlayerName, bool IsHost);
};

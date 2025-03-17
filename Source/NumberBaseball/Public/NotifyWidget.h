#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotifyWidget.generated.h"

UCLASS()
class NUMBERBASEBALL_API UNotifyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void NotifyTurnStart();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyTurnOver();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyVictory();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyDefeat();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyDraw();
};

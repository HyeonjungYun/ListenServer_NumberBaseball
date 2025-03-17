#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseballPlayerController.h"
#include "ChatWindowWidget.generated.h"

UCLASS()
class NUMBERBASEBALL_API UChatWindowWidget : public UUserWidget
{
	GENERATED_BODY()
	
/*----------PROPERTY----------*/
public:

/*----------FUNCTION----------*/
public:
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveMyMessage(const FString& Message);

	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveOtherMessage(const FString& Message);

	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveMyResultMessage(const FString& Result);

	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveOtherResultMessage(const FString& Result);

	UFUNCTION(BlueprintImplementableEvent)
	void DeactivateSubmittionButton();

	UFUNCTION(BlueprintImplementableEvent)
	void ActivateSubmittionButton();
};

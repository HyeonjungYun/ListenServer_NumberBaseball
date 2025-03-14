#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseballPlayerController.h"
#include "ChatWindowWidget.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSendMessage, const FString&, Message);

UCLASS()
class NUMBERBASEBALL_API UChatWindowWidget : public UUserWidget
{
	GENERATED_BODY()
	
/*----------PROPERTY----------*/
public:
	/*UPROPERTY(BlueprintAssignable)
	FOnSendMessage OnSendMessage;*/

/*----------FUNCTION----------*/
public:
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveMyMessage(const FString& Message);

	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveOtherMessage(const FString& Message);

	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveResultMessage(const FString& Result);

	UFUNCTION(BlueprintImplementableEvent)
	void DeactivateSubmittionButton();

	UFUNCTION(BlueprintImplementableEvent)
	void ActivateSubmittionButton();
};

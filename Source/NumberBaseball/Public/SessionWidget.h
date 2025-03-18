#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionWidget.generated.h"

UCLASS()
class NUMBERBASEBALL_API USessionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void AddFoundSession(
		const FString& CustomSessionName, 
		int32 MaxPlayers, 
		int32 CurrentPlayers);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateSession(const FName& CustomSessionName);


protected:
	virtual void NativeConstruct() override;
};

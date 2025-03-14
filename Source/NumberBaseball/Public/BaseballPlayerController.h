#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseballPlayerController.generated.h"

class UChatWindowWidget;

UCLASS()
class NUMBERBASEBALL_API ABaseballPlayerController : public APlayerController
{
	GENERATED_BODY()
	
/*----------PROPERTY----------*/
public:
	UPROPERTY(VisibleAnywhere, Category = "RandNumber")
	FString RandNumber;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> ChatWindowWidget;

	UPROPERTY()
	UChatWindowWidget* ChatWindowWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> OpeningServerWidget;

	UPROPERTY()
	UUserWidget* OpeningServerWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> OpeningClientWidget;

	UPROPERTY()
	UUserWidget* OpeningClientWidgetInstance;

	UPROPERTY(Replicated)
	bool bIsReady;

/*----------FUNCTION----------*/
public:
	ABaseballPlayerController();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void AddMyChatWidget(const FString& Message);
	void AddOtherChatWidget(const FString& Message);
	void AddResultWidget(const FString Result);

	UFUNCTION(BlueprintPure)
	bool GetbIsReady();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SetIsReady();

	UFUNCTION(BlueprintCallable)
	void OpenOpeningServerWidget();

	UFUNCTION(BlueprintCallable)
	void OpenOpeningClientWidget();
	
	UFUNCTION(BlueprintCallable)
	void OpenChatWidget();

	UFUNCTION(Blueprintcallable)
	void CheckSubmittedNumber(const FString& SubmittedNumber);

	UFUNCTION(Server, Reliable)
	void Server_RequestReceiveResult(const FString& Result);

	UFUNCTION(Blueprintpure, Category = "Number")
	FString GetRandomNumber();

	UFUNCTION(BlueprintCallable, Category = "RequestChat")
	void RequestSendMessage(const FString& Message);

	UFUNCTION(BlueprintCallable, Category = "Ready")
	void RequestReadyCheck();

	UFUNCTION(Server, Reliable)
	void Server_SendChatMessage(APlayerState* SenderState, const FString& Message);

	UFUNCTION(Server, Reliable)
	void Server_CheckReady();
};

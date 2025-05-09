﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseballPlayerController.generated.h"

class UChatWindowWidget;
class UNotifyWidget;
class ULoginWidget;

UCLASS()
class NUMBERBASEBALL_API ABaseballPlayerController : public APlayerController
{
	GENERATED_BODY()
	
/*----------PROPERTY----------*/
public:
	bool bIsDraw;

#pragma region 위젯 관련 변수

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> LoginWidget;

	UPROPERTY()
	UUserWidget* LoginWidgetInstance;

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

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> NotifyWidget;

	UPROPERTY()
	UNotifyWidget* NotifyWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> FinishWidget;

	UPROPERTY()
	UUserWidget* FinishWidgetInstance;

#pragma endregion 위젯 관련 변수

#pragma region Replicated 변수

	UPROPERTY(Replicated)
	bool bIsVictory;

	UPROPERTY(Replicated)
	bool bIsReady;

	UPROPERTY(Replicated)
	FString TempResult;

	UPROPERTY(EditAnywhere, Replicated)
	bool bIsSubmitted;

#pragma endregion Replicated 변수

/*----------FUNCTION----------*/
public:
#pragma region 생성자, 퓨어 게임 시작 함수

	ABaseballPlayerController();

	UNotifyWidget* GetNotifyWidget();

	UFUNCTION(BlueprintPure)
	bool GetbIsReady();

	UFUNCTION(BlueprintPure)
	bool GetIsVictory();

	UFUNCTION(BlueprintPure)
	bool GetIsDraw();

	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintCallable)
	void CleanControllerInf();

#pragma endregion 생성자, 퓨어 게임 시작 함수

#pragma region virtual함수

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma endregion virtual함수

#pragma region 버튼 제어 함수

	void DeactiveSubmitButton();
	void ActiveSubmitButton();

#pragma endregion 버튼 제어 함수

#pragma region 위젯 관련 함수

	void AddMyChatWidget(const FString& Message);
	void AddOtherChatWidget(const FString& Message);
	void AddMyResultWidget(const FString& Result);
	void AddOtherResultWidget(const FString& Result);

	UFUNCTION(BlueprintCallable)
	void OpenLoginWidget();

	UFUNCTION(BlueprintCallable)
	void OpenOpeningServerWidget();

	UFUNCTION(BlueprintCallable)
	void OpenOpeningClientWidget();
	
	UFUNCTION(BlueprintCallable)
	void OpenChatWidget();

	UFUNCTION(BlueprintCallable)
	void OpenFinishWidget();

#pragma endregion 위젯 관련 함수

#pragma region 리퀘스트, 숫자 판별 함수

	UFUNCTION(Blueprintcallable)
	void CheckSubmittedNumber(const FString& SubmittedNumber);

	UFUNCTION(BlueprintCallable, Category = "RequestChat")
	void RequestSendMessage(const FString& Message);

	UFUNCTION(BlueprintCallable, Category = "Ready")
	void RequestReadyCheck();

	UFUNCTION()
	void RequestNotifyTurnStart();

	UFUNCTION()
	void RequestNotifyTurnOver();

	UFUNCTION()
	void RequestNotifyVictory();

	UFUNCTION()
	void RequestNotifyDefeat();

	UFUNCTION()
	void RequestNotifyDraw();

#pragma endregion 리퀘스트, 숫자 판별 함수

#pragma region 서버RPC함수

	UFUNCTION(Server, Reliable)
	void Server_SendChatMessage(APlayerState* SenderState, const FString& Message);

	UFUNCTION(Server, Reliable)
	void Server_CheckReady();

	UFUNCTION(Server, Reliable)
	void Server_MatchVictory();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SetResult(const FString& Result);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SetIsReady();

	UFUNCTION(Server, Reliable)
	void Server_RequestReceiveResult();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SetIsSubmitted(bool IsSubmitted);

	UFUNCTION(Server, Reliable)
	void Server_RequestAddPlayerList(const FString& PlayerName, bool IsHost);

#pragma endregion 서버RPC함수
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseballPlayerController.generated.h"

class UChatWindowWidget;
class UNotifyWidget;
class USessionWidget;

UCLASS()
class NUMBERBASEBALL_API ABaseballPlayerController : public APlayerController
{
	GENERATED_BODY()
	
/*----------PROPERTY----------*/
public:
#pragma region 위젯 관련 변수
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
	TSubclassOf<UUserWidget> LoginWidget;

	UPROPERTY()
	UUserWidget* LoginWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> NotifyWidget;

	UPROPERTY()
	UNotifyWidget* NotifyWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> SessionWidget;

	UPROPERTY()
	USessionWidget* SessionWidgetInstance;

	UPROPERTY()
	FString UserUnique;

#pragma endregion 위젯 관련 변수
#pragma region Replicated 변수
	UPROPERTY(Replicated)
	bool bIsReady;

	UPROPERTY(Replicated)
	bool bIsVictory;

	UPROPERTY(Replicated)
	FString TempResult;
#pragma endregion Replicated 변수

/*----------FUNCTION----------*/
public:
#pragma region 생성자, getter, setter 함수
	ABaseballPlayerController();

	UFUNCTION(BlueprintPure)
	bool GetbIsReady();

	UFUNCTION(BlueprintCallable)
	void SetUserUnique(const FString& Unique);

#pragma endregion 생성자, getter, setter 함수

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
	void OpenOpeningServerWidget();

	UFUNCTION(BlueprintCallable)
	void OpenOpeningClientWidget();
	
	UFUNCTION(BlueprintCallable)
	void OpenChatWidget();

	UFUNCTION(BlueprintCallable)
	void OpenSessionWidget();

	UFUNCTION(BlueprintCallable)
	void OpenLoginWidget();
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
#pragma endregion 서버RPC함수
};

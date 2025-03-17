#include "BaseballPlayerController.h"
#include "ChatWindowWidget.h"
#include "BaseballGameMode.h"
#include "BaseballGameState.h"
#include "NotifyWidget.h"

#include "Misc/DefaultValueHelper.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

#pragma region 생성자, 퓨어 함수

ABaseballPlayerController::ABaseballPlayerController() 
	: bIsReady(false)
{
}

bool ABaseballPlayerController::GetbIsReady()
{
	return bIsReady;
}

#pragma endregion 생성자, 퓨어 함수

#pragma region virtual함수

void ABaseballPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("[BeginPlay] 서버를 가진 클라이언트 실행"));
		OpenOpeningServerWidget();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[BeginPlay] 일반 클라이언트 실행"));
		OpenOpeningClientWidget();
	}
}

void ABaseballPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseballPlayerController, bIsReady);
	DOREPLIFETIME(ABaseballPlayerController, bIsVictory);
	DOREPLIFETIME(ABaseballPlayerController, TempResult);
}

#pragma endregion virtual함수

#pragma region 버튼 제어 함수

void ABaseballPlayerController::DeactiveSubmitButton()
{
	if (ChatWindowWidgetInstance)
	{
		ChatWindowWidgetInstance->DeactivateSubmittionButton();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[DeactivSubmitButton] 위젯 인스턴스 설정 안 됨"));
	}
}

void ABaseballPlayerController::ActiveSubmitButton()
{
	if (ChatWindowWidgetInstance)
	{
		ChatWindowWidgetInstance->ActivateSubmittionButton();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ActivSubmitButton] 위젯 인스턴스 설정 안 됨"));
	}
}

#pragma endregion 버튼 제어 함수

#pragma region 위젯 관련 함수

void ABaseballPlayerController::AddMyChatWidget(const FString& Message)
{
	UE_LOG(LogTemp, Warning, TEXT("[AddMyChatWidget] 메시지: %s"), *Message);

	if (IsValid(ChatWindowWidgetInstance))
	{
		// 다른 사람이 보낸 메세지
		ChatWindowWidgetInstance->ReceiveMyMessage(Message);
	}
}

void ABaseballPlayerController::AddOtherChatWidget(const FString& Message)
{
	UE_LOG(LogTemp, Warning, TEXT("[AddOtherChatWidget] 메시지: %s"), *Message);

	if (IsValid(ChatWindowWidgetInstance))
	{
		// 다른 사람이 보낸 메세지
		ChatWindowWidgetInstance->ReceiveOtherMessage(Message);
	}
}

void ABaseballPlayerController::AddMyResultWidget(const FString& Result)
{
	if (IsValid(ChatWindowWidgetInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("[AddMyResultWidget] 메시지: %s"), *Result);
		// 게임 스테이트에서 보낸 결과 메세지
		ChatWindowWidgetInstance->ReceiveMyResultMessage(Result);
	}
}

void ABaseballPlayerController::AddOtherResultWidget(const FString& Result)
{
	UE_LOG(LogTemp, Warning, TEXT("[AddOtherResultWidget] 결과 출력 준비"));
	if (IsValid(ChatWindowWidgetInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("[AddOtherResultWidget] 메시지: %s"), *Result);
		// 게임 스테이트에서 보낸 결과 메세지
		ChatWindowWidgetInstance->ReceiveOtherResultMessage(Result);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[AddOtherResultWidget] 결과 출력 실패"));
	}
}

void ABaseballPlayerController::OpenOpeningServerWidget()
{
	if (IsLocalController())
	{
		bIsReady = true;

		UE_LOG(LogTemp, Warning, TEXT("[OpenOpeningServerWidget] 서버를 가진 클라이언트 실행"));
		OpeningServerWidgetInstance = CreateWidget<UUserWidget>(this, OpeningServerWidget);

		if (OpeningServerWidgetInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("[OpenOpeningServerWidget] 서버 위젯 인스턴스 실행"));
			OpeningServerWidgetInstance->AddToViewport();

			this->bShowMouseCursor = true;
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(OpeningServerWidgetInstance->TakeWidget());
			this->SetInputMode(InputMode);
		}
	}
}

void ABaseballPlayerController::OpenOpeningClientWidget()
{
	if (IsLocalController())
	{
		UE_LOG(LogTemp, Warning, TEXT("[OpenOpeningClientWidget] 일반 클라이언트 실행"));
		OpeningClientWidgetInstance = CreateWidget<UUserWidget>(this, OpeningClientWidget);

		if (OpeningClientWidgetInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("[OpenOpeningServerWidget] 클라이언트 인스턴스 실행"));
			OpeningClientWidgetInstance->AddToViewport();

			this->bShowMouseCursor = true;
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(OpeningClientWidgetInstance->TakeWidget());
			this->SetInputMode(InputMode);
		}
	}
}

void ABaseballPlayerController::OpenChatWidget()
{
	TArray<UUserWidget*> FoundWidgets;

	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UUserWidget::StaticClass(), false);

	for (UUserWidget* Widget : FoundWidgets)
	{
		if (Widget)
		{
			Widget->RemoveFromParent();
		}
	}

	if (IsLocalController())
	{
		UE_LOG(LogTemp, Warning, TEXT("[OpenChatWidget] 게임 위젯 오픈"));
		ChatWindowWidgetInstance = Cast<UChatWindowWidget>(CreateWidget<UUserWidget>(this, ChatWindowWidget));

		if (ChatWindowWidgetInstance)
		{
			ChatWindowWidgetInstance->AddToViewport();

			this->bShowMouseCursor = true;
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(ChatWindowWidgetInstance->TakeWidget());
			this->SetInputMode(InputMode);
		}

		NotifyWidgetInstance = Cast<UNotifyWidget>(CreateWidget<UUserWidget>(this, NotifyWidget));

		if (NotifyWidgetInstance)
		{
			NotifyWidgetInstance->AddToViewport();
		}
	}
}

#pragma endregion 위젯 관련 함수

#pragma region 리퀘스트, 숫자 판별 함수

void ABaseballPlayerController::CheckSubmittedNumber(const FString& SubmittedNumber)
{
	TArray<int32> ResultNumber = { 0, 0 };

	if (ABaseballGameState* BaseballGameState = Cast< ABaseballGameState>(GetWorld()->GetGameState()))
	{
		FString RandNumber = BaseballGameState->GetRandomNumber();

		UE_LOG(LogTemp, Warning, TEXT("[CheckSubmittedNumber] 제출한 숫자 : %s"), *SubmittedNumber);
		for (int i = 0; i < RandNumber.Len(); i++)
		{
			if (RandNumber[i] == SubmittedNumber[i])
			{
				ResultNumber[0]++;
			}
		}

		for (int i = 0; i < SubmittedNumber.Len(); i++)
		{
			for (int j = 0; j < RandNumber.Len(); j++)
			{
				if (i == j)
				{
					continue;
				}

				if (RandNumber[j] == SubmittedNumber[i])
				{
					ResultNumber[1]++;
				}
			}
		}

		if (ResultNumber[0] >= 3)
		{
			bIsVictory = true;
			Server_MatchVictory();
		}

		FString Result = FString::Printf(TEXT("%s -> %dS%dB"),
			*SubmittedNumber,
			ResultNumber[0],
			ResultNumber[1]);

		//Server_RequestReceiveResult(Result);
		Server_SetResult(Result);
	}
}

void ABaseballPlayerController::RequestSendMessage(const FString& Message)
{
	if (IsLocalController())
	{
		Server_SendChatMessage(this->PlayerState, Message);
	}
}

void ABaseballPlayerController::RequestReadyCheck()
{
	if (HasAuthority())
	{
		if (ABaseballGameState* BaseballGameState = Cast< ABaseballGameState>(GetWorld()->GetGameState()))
		{
			Server_CheckReady();
		}
	}
}

void ABaseballPlayerController::RequestNotifyTurnStart()
{
	if (NotifyWidgetInstance)
	{
		NotifyWidgetInstance->NotifyTurnStart();
	}
}

void ABaseballPlayerController::RequestNotifyTurnOver()
{
	if (NotifyWidgetInstance)
	{
		NotifyWidgetInstance->NotifyTurnOver();
	}
}

void ABaseballPlayerController::RequestNotifyVictory()
{
	if (NotifyWidgetInstance)
	{
		NotifyWidgetInstance->NotifyVictory();
	}
}

void ABaseballPlayerController::RequestNotifyDefeat()
{
	if (NotifyWidgetInstance)
	{
		NotifyWidgetInstance->NotifyDefeat();
	}
}

void ABaseballPlayerController::RequestNotifyDraw()
{
	if (NotifyWidgetInstance)
	{
		NotifyWidgetInstance->NotifyDraw();
	}
}

#pragma endregion 리퀘스트, 숫자 판별 함수

#pragma region 서버RPC함수

void ABaseballPlayerController::Server_SetResult_Implementation(const FString& Result)
{
	TempResult = Result;
}

void ABaseballPlayerController::Server_SetIsReady_Implementation()
{
	if (bIsReady)
	{
		bIsReady = false;
	}
	else
	{
		bIsReady = true;
	}
}

void ABaseballPlayerController::Server_RequestReceiveResult_Implementation()
{
	FString Result = TempResult;
	if (ABaseballGameState* BaseballGameState = Cast< ABaseballGameState>(GetWorld()->GetGameState()))
	{
		BaseballGameState->Multicast_ReceiveCheckResult(this->PlayerState, Result);
	}
}

void ABaseballPlayerController::Server_SendChatMessage_Implementation(APlayerState* SenderState, const FString& Message)
{
	UE_LOG(LogTemp, Warning, TEXT("[서버] Server_SendChatMessage 실행됨. 메시지: %s"), *Message);
	if (ABaseballGameState* BaseballGameState = Cast< ABaseballGameState>(GetWorld()->GetGameState()))
	{
		BaseballGameState->Multicast_ReceiveMessage(SenderState, Message);
	}
}

void ABaseballPlayerController::Server_CheckReady_Implementation()
{
	int32 count = 0;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		UE_LOG(LogTemp, Warning, TEXT("[서버] 플레이어 컨트롤러: %d"), count);
		if (ABaseballPlayerController* BaseballPlayerController = Cast<ABaseballPlayerController>(It->Get()))
		{
			if (!BaseballPlayerController->GetbIsReady())
			{
				UE_LOG(LogTemp, Warning, TEXT("[서버] 플레이어 컨트롤러%d 준비 안 됨"), count++);
				return;
			}
			UE_LOG(LogTemp, Warning, TEXT("[서버] 플레이어 컨트롤러%d 준비 됨"), count++);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[서버] 플레이어 컨트롤러 캐스팅 실패"));
		}
	}

	if (ABaseballGameState* BaseballGameState = Cast< ABaseballGameState>(GetWorld()->GetGameState()))
	{
		BaseballGameState->Multicast_GameStart();
		BaseballGameState->InitGame();
	}
}

void ABaseballPlayerController::Server_MatchVictory_Implementation()
{
	if (ABaseballGameState* BaseballGameState = Cast< ABaseballGameState>(GetWorld()->GetGameState()))
	{
		BaseballGameState->CountVictoryUsers();
	}
}

#pragma endregion 서버RPC함수
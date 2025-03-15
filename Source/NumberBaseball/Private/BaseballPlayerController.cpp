#include "BaseballPlayerController.h"
#include "ChatWindowWidget.h"
#include "BaseballGameMode.h"
#include "BaseballGameState.h"

#include "Misc/DefaultValueHelper.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

ABaseballPlayerController::ABaseballPlayerController() 
	: bIsReady(false)
{
}

void ABaseballPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ABaseballGameState* BaseballGameState = Cast<ABaseballGameState>(UGameplayStatics::GetGameState(this)))
	{
		RandNumber = BaseballGameState->SetRandomNumber();
	}

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
	}
}

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

void ABaseballPlayerController::AddResultWidget(const FString Result)
{
	UE_LOG(LogTemp, Warning, TEXT("[AddResultWidget] 메시지: %s"), *Result);
	if (IsValid(ChatWindowWidgetInstance))
	{
		// 게임 스테이트에서 보낸 결과 메세지
		ChatWindowWidgetInstance->ReceiveResultMessage(Result);
	}
}

void ABaseballPlayerController::DeactivSubmitButton()
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

void ABaseballPlayerController::ActivSubmitButton()
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

bool ABaseballPlayerController::GetbIsReady()
{
	return bIsReady;
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

FString ABaseballPlayerController::GetRandomNumber()
{
	UE_LOG(LogTemp, Warning, TEXT("[GetRandomNumber] 정답: %d%d%d"), RandNumber[0], RandNumber[1], RandNumber[2]);
	return RandNumber;
}

void ABaseballPlayerController::CheckSubmittedNumber(const FString& SubmittedNumber)
{
	TArray<int32> ResultNumber = { 0, 0 };

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

	FString Result = FString::Printf(TEXT("%s -> %dS%dB"), 
		*SubmittedNumber, 
		ResultNumber[0],
		ResultNumber[1]);

	Server_RequestReceiveResult(Result);
}

void ABaseballPlayerController::Server_RequestReceiveResult_Implementation(const FString& Result)
{
	if (ABaseballGameState* BaseballGameState = Cast< ABaseballGameState>(GetWorld()->GetGameState()))
	{
		BaseballGameState->Multicast_ReceiveCheckResult(Result);
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
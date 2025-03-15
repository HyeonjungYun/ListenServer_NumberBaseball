#include "BaseballGameState.h"
#include "BaseballPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ABaseballGameState::ABaseballGameState()
{
	SetRandomNumber();
}

FString ABaseballGameState::SetRandomNumber()
{
	TArray<int32> NumberPool = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	for (int32 i = 0; i < NumberPool.Num(); ++i)
	{
		int32 RandIndex = FMath::RandRange(i, NumberPool.Num() - 1);
		NumberPool.Swap(i, RandIndex);
	}

	FString RandNumber = FString::Printf(TEXT("%d%d%d"), NumberPool[0], NumberPool[1], NumberPool[2]);

	return RandNumber;
}

void ABaseballGameState::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseballGameState::InitGame()
{
	RequestDeactivateAllSubmitButton();

	GetWorldTimerManager().SetTimer(
		GameStartTimer,
		this,
		&ABaseballGameState::GameStart,
		5.0f,
		false
	);
}

void ABaseballGameState::GameStart()
{
	GetWorldTimerManager().ClearTimer(GameStartTimer);

	UE_LOG(LogTemp, Warning, TEXT("[GameStart] 게임 실행"));

	GetWorldTimerManager().SetTimer(
		SubmittionTurnTimer,
		this,
		&ABaseballGameState::RequestActivateAllSubmitButton,
		20.0f,
		true,
		0.f
	);

	GetWorldTimerManager().SetTimer(
		TurnOverTimer,
		this,
		&ABaseballGameState::RequestDeactivateAllSubmitButton,
		20.0f,
		true,
		15.0f
	);
}

void ABaseballGameState::Multicast_ReceiveMessage_Implementation(APlayerState* SenderState, const FString& Message)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		if (ABaseballPlayerController* BaseballPlayerController = Cast<ABaseballPlayerController>(It->Get()))
		{
			if (APlayerState* MyState = BaseballPlayerController->PlayerState)
			{
				if (!SenderState || !MyState)
				{
					return;
				}

				if (SenderState == MyState)
				{
					// 내가 보낸 메세지
					BaseballPlayerController->AddMyChatWidget(Message);
				}
				else
				{
					// 다른 사람이 보낸 메세지
					BaseballPlayerController->AddOtherChatWidget(Message);
				}
			}
		}
	}
}

void ABaseballGameState::Multicast_ReceiveCheckResult_Implementation(const FString& Result)
{
	UE_LOG(LogTemp, Warning, TEXT("[Multicast_ReceiveCheckResult_Implementation] 결과: %s"), *Result);
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (ABaseballPlayerController* BaseballPlayerController = Cast<ABaseballPlayerController>(PlayerController))
	{
		BaseballPlayerController->AddResultWidget(Result);
	}
}

void ABaseballGameState::Multicast_GameStart_Implementation()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (ABaseballPlayerController* BaseballPlayerController = Cast<ABaseballPlayerController>(PlayerController))
	{
		BaseballPlayerController->OpenChatWidget();
	}
}

void ABaseballGameState::RequestActivateAllSubmitButton()
{
	UE_LOG(LogTemp, Warning, TEXT("[RequestDeactivateAllSubmitButton] 제출 버튼 활성화"));
	Multicast_BeTimeToSubmit();
}

void ABaseballGameState::RequestDeactivateAllSubmitButton()
{
	UE_LOG(LogTemp, Warning, TEXT("[RequestDeactivateAllSubmitButton] 제출 버튼 비활성화"));
	Multicast_OverTurnToSubmit();
}

void ABaseballGameState::RequestDeactivateSubmitButton()
{
	Client_DeactiveSubmitButton();
}

void ABaseballGameState::Multicast_BeTimeToSubmit_Implementation()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (ABaseballPlayerController* BaseballPlayerController = Cast<ABaseballPlayerController>(PlayerController))
	{
		BaseballPlayerController->ActivSubmitButton();
	}
}

void ABaseballGameState::Multicast_OverTurnToSubmit_Implementation()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (ABaseballPlayerController* BaseballPlayerController = Cast<ABaseballPlayerController>(PlayerController))
	{
		BaseballPlayerController->DeactivSubmitButton();
	}
}

void ABaseballGameState::Client_DeactiveSubmitButton_Implementation()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (ABaseballPlayerController* BaseballPlayerController = Cast<ABaseballPlayerController>(PlayerController))
	{
		BaseballPlayerController->DeactivSubmitButton();
	}
}
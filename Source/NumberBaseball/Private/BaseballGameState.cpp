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

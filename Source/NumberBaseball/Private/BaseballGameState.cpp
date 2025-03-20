#include "BaseballGameState.h"
#include "BaseballPlayerController.h"
#include "NotifyWidget.h"
#include "BaseballCalculator.h"

#include "Misc/DefaultValueHelper.h"
#include "Net/UnrealNetwork.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

#pragma region 생성자, virtual 함수

ABaseballGameState::ABaseballGameState()
	: VictoryUsers(0)
	, RemainedTime(0)
{
	Refree = new BaseballCalculator();
	RandNumber = Refree->SetRandomNumber();
}

void ABaseballGameState::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseballGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseballGameState, RemainedTime);
	DOREPLIFETIME(ABaseballGameState, RandNumber);
}

#pragma endregion 생성자, virtual 함수

void ABaseballGameState::InitGame()
{
	Multicast_OverTurnToSubmit();

	RemainedTime = 5;

	GetWorldTimerManager().SetTimer(
		GameTimer,
		this,
		&ABaseballGameState::Multicast_UpdateRemainedTime,
		1.0f,
		true
	);

	GetWorldTimerManager().SetTimer(
		GameStartTimer,
		this,
		&ABaseballGameState::GameStart,
		5.0f,
		false
	);
}

FString ABaseballGameState::GetRandomNumber()
{
	return RandNumber;
}

void ABaseballGameState::NotifyMatchResult()
{
	Multicast_RequestSendResult();
	RequestDeactivateAllSubmitButton();

	if (!VictoryUsers)
	{
		return;
	}

	if (VictoryUsers >= 2)
	{
		Multicast_NotifyDraw();
	}
	else
	{
		Multicast_CheckMatchResult();
	}

}

void ABaseballGameState::CleanGameStateInf()
{
	GetWorldTimerManager().ClearTimer(GameTimer);
	GetWorldTimerManager().ClearTimer(GameStartTimer);
	GetWorldTimerManager().ClearTimer(GamePlayingTimer);
	GetWorldTimerManager().ClearTimer(SubmittionTurnTimer);
	GetWorldTimerManager().ClearTimer(TurnOverTimer);

	VictoryUsers = 0;
	RemainedTime = 0;

	RandNumber = Refree->SetRandomNumber();
}

TMap<FString, bool> ABaseballGameState::GetPlayerList() const
{
	return PlayerList;
}

void ABaseballGameState::AddPlayerToPlayerList(FString PlayerName, bool bIsHost)
{
	Multicast_AddPlayerToPlayerList(PlayerName, bIsHost);
}

void ABaseballGameState::RemoveAllPlayerList()
{
	PlayerList.Empty();
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
		&ABaseballGameState::NotifyMatchResult,
		20.0f,
		true,
		15.0f
	);
}

void ABaseballGameState::CountVictoryUsers()
{
	VictoryUsers++;
}

#pragma region 리퀘스트 함수

void ABaseballGameState::RequestActivateAllSubmitButton()
{
	RemainedTime = 15;

	UE_LOG(LogTemp, Warning, TEXT("[RequestDeactivateAllSubmitButton] 제출 버튼 활성화"));
	Multicast_BeTimeToSubmit();

	// 턴 시작 알림
	Multicast_NotifyTurnStart();

	// 모든 플레이어 제출 상태 false로 변경
	Multicast_SetbIsSubmittedAllFalse();
}

void ABaseballGameState::RequestDeactivateAllSubmitButton()
{
	RemainedTime = 5;

	UE_LOG(LogTemp, Warning, TEXT("[RequestDeactivateAllSubmitButton] 제출 버튼 비활성화"));
	Multicast_OverTurnToSubmit();

	// 턴 종료 알림
	Multicast_NotifyTurnOver();
}

void ABaseballGameState::RequestDeactivateSubmitButton()
{
	Client_DeactiveSubmitButton();
}

#pragma endregion 리퀘스트 함수

#pragma region 클라이언트 함수

void ABaseballGameState::Client_DeactiveSubmitButton_Implementation()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (ABaseballPlayerController* BaseballPlayerController = Cast<ABaseballPlayerController>(PlayerController))
	{
		BaseballPlayerController->DeactiveSubmitButton();
	}
}

void ABaseballGameState::Client_NotifyVictory_Implementation()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (ABaseballPlayerController* BaseballPlayerController = Cast<ABaseballPlayerController>(PlayerController))
	{
		BaseballPlayerController->RequestNotifyVictory();
	}
}

void ABaseballGameState::Client_NotifyDefeat_Implementation()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (ABaseballPlayerController* BaseballPlayerController = Cast<ABaseballPlayerController>(PlayerController))
	{
		BaseballPlayerController->RequestNotifyDefeat();
	}
}

#pragma endregion 클라이언트 함수

#pragma region 멀티캐스트 함수

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

void ABaseballGameState::Multicast_ReceiveCheckResult_Implementation(APlayerState* SenderState, const FString& Result)
{
	UE_LOG(LogTemp, Warning, TEXT("[Multicast_ReceiveCheckResult_Implementation] 결과: %s"), *Result);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (ABaseballPlayerController* BaseballPlayerController = Cast<ABaseballPlayerController>(PlayerController))
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
				BaseballPlayerController->AddMyResultWidget(Result);
			}
			else
			{
				// 다른 사람이 보낸 메세지
				BaseballPlayerController->AddOtherResultWidget(Result);
			}
		}
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

void ABaseballGameState::Multicast_BeTimeToSubmit_Implementation()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (ABaseballPlayerController* BaseballPlayerController = Cast<ABaseballPlayerController>(PlayerController))
	{
		BaseballPlayerController->ActiveSubmitButton();
	}
}

void ABaseballGameState::Multicast_OverTurnToSubmit_Implementation()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (ABaseballPlayerController* BaseballPlayerController = Cast<ABaseballPlayerController>(PlayerController))
	{
		BaseballPlayerController->DeactiveSubmitButton();
	}
}

void ABaseballGameState::Multicast_NotifyTurnStart_Implementation()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (ABaseballPlayerController* BaseballPlayerController = Cast<ABaseballPlayerController>(PlayerController))
	{
		BaseballPlayerController->RequestNotifyTurnStart();
	}
}

void ABaseballGameState::Multicast_NotifyTurnOver_Implementation()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (ABaseballPlayerController* BaseballPlayerController = Cast<ABaseballPlayerController>(PlayerController))
	{
		BaseballPlayerController->RequestNotifyTurnOver();
	}
}

void ABaseballGameState::Multicast_NotifyDraw_Implementation()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (ABaseballPlayerController* BaseballPlayerController = Cast<ABaseballPlayerController>(PlayerController))
	{
		BaseballPlayerController->RequestNotifyDraw();
	}
}

void ABaseballGameState::Multicast_CheckMatchResult_Implementation()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (ABaseballPlayerController* BaseballPlayerController = Cast<ABaseballPlayerController>(PlayerController))
	{
		if (BaseballPlayerController->bIsVictory)
		{
			Client_NotifyVictory();
		}
		else
		{
			Client_NotifyDefeat();
		}
	}
}

void ABaseballGameState::Multicast_RequestSendResult_Implementation()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (ABaseballPlayerController* BaseballPlayerController = Cast<ABaseballPlayerController>(PlayerController))
	{
		BaseballPlayerController->Server_RequestReceiveResult();
	}
}

void ABaseballGameState::Multicast_SetbIsSubmittedAllFalse_Implementation()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (ABaseballPlayerController* BaseballPlayerController = Cast<ABaseballPlayerController>(PlayerController))
	{
		BaseballPlayerController->Server_SetIsSubmitted(false);
	}
}

void ABaseballGameState::Multicast_AddPlayerToPlayerList_Implementation(const FString& PlayerName, bool bIsHost)
{
	UE_LOG(LogTemp, Warning, TEXT("[Multicast_AddPlayerToPlayerList_Implementation] %s 세션 입장"), *PlayerName);
	
	PlayerList.Add(PlayerName, bIsHost);
	OnEnterPlayer.Broadcast(PlayerName, bIsHost);
}

void ABaseballGameState::Multicast_UpdateRemainedTime_Implementation()
{
	RemainedTime--;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (ABaseballPlayerController* BaseballPlayerController = Cast<ABaseballPlayerController>(PlayerController))
	{
		if (UNotifyWidget* Nortify = BaseballPlayerController->GetNotifyWidget())
		{
			if (UTextBlock* TimeText = Cast<UTextBlock>(Nortify->GetWidgetFromName(TEXT("TimeText"))))
			{
				TimeText->SetText(FText::FromString(FString::Printf(TEXT("%d"), RemainedTime)));
			}
		}
	}
}

#pragma endregion 멀티캐스트 함수
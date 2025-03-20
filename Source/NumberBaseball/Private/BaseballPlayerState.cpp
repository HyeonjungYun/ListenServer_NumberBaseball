#include "BaseballPlayerState.h"
#include "BaseballGameState.h"
#include "BaseballPlayerController.h"

#include "Kismet/GameplayStatics.h"

const FString ABaseballPlayerState::GetBaseballPlayerName() const
{
	return BaseballPlayerName;
}

void ABaseballPlayerState::SetBaseballPlayerName(const FString& PlayerName, bool IsHost)
{
	BaseballPlayerName = PlayerName;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (ABaseballPlayerController* BaseballPlayerController = Cast<ABaseballPlayerController>(PlayerController))
	{
		BaseballPlayerController->Server_RequestAddPlayerList(PlayerName, IsHost);
	}
}

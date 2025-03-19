#include "BaseballPlayerState.h"

const FString ABaseballPlayerState::GetBaseballPlayerName() const
{
	return BaseballPlayerName;
}

void ABaseballPlayerState::SetBaseballPlayerName(const FString& PlayerName)
{
	BaseballPlayerName = PlayerName;
}

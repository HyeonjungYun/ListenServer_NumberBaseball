#pragma once

#include "CoreMinimal.h"

class NUMBERBASEBALL_API BaseballCalculator
{
public:
	BaseballCalculator();
	~BaseballCalculator();

	FString SetRandomNumber();
	FString CheckNumber(const FString RandNumber, const FString& SubmittedNumber, TArray<int32>& ResultNumber);
};

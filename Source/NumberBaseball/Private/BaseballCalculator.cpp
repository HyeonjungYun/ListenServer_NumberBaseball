#include "BaseballCalculator.h"

BaseballCalculator::BaseballCalculator()
{
}

BaseballCalculator::~BaseballCalculator()
{
	delete this;
}

FString BaseballCalculator::SetRandomNumber()
{
	TArray<int32> NumberPool = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	for (int32 i = 0; i < NumberPool.Num(); ++i)
	{
		int32 RandIndex = FMath::RandRange(i, NumberPool.Num() - 1);
		NumberPool.Swap(i, RandIndex);
	}

	FString Number = FString::Printf(TEXT("%d%d%d"), NumberPool[0], NumberPool[1], NumberPool[2]);

	return Number;
}

FString BaseballCalculator::CheckNumber(const FString RandNumber, const FString& SubmittedNumber, TArray<int32>& ResultNumber)
{
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

	return Result;
}

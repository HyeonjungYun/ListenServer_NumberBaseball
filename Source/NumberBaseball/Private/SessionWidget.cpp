#include "SessionWidget.h"
#include "BaseballGameInstance.h"

#include "Kismet/GameplayStatics.h"

void USessionWidget::NativeConstruct()
{
	if (UBaseballGameInstance* GI = Cast<UBaseballGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		GI->OnSessionFound.AddDynamic(this, &USessionWidget::AddFoundSession);
		GI->OnCreateSession.AddDynamic(this, &USessionWidget::CreateSession);
	}
}

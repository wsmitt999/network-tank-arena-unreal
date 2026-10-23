#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "TankPickupPopupWidget.generated.h"

class UTextBlock;

UCLASS()
class NETWORKTANKARENA_API UTankPickupPopupWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void ShowMessage(const FText& Message, float DisplayDuration = 3.f);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

private:
	void RemovePopup();

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> MessageText;

	FTimerHandle RemoveTimerHandle;
};

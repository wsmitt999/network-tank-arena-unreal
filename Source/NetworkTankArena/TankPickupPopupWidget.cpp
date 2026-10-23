#include "TankPickupPopupWidget.h"

#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"

void UTankPickupPopupWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UCanvasPanel* Canvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("PopupCanvas"));
	WidgetTree->RootWidget = Canvas;

	UBorder* Background = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("PopupBackground"));
	Background->SetBrushColor(FLinearColor(0.015f, 0.04f, 0.08f, 0.9f));
	Background->SetPadding(FMargin(24.f, 14.f));
	UCanvasPanelSlot* BackgroundSlot = Canvas->AddChildToCanvas(Background);
	BackgroundSlot->SetAnchors(FAnchors(0.5f, 0.f, 0.5f, 0.f));
	BackgroundSlot->SetAlignment(FVector2D(0.5f, 0.f));
	BackgroundSlot->SetPosition(FVector2D(0.f, 80.f));
	BackgroundSlot->SetAutoSize(true);

	MessageText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("PopupMessage"));
	FSlateFontInfo Font = MessageText->GetFont();
	Font.Size = 22;
	MessageText->SetFont(Font);
	MessageText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	MessageText->SetJustification(ETextJustify::Center);
	Background->SetContent(MessageText);
}

void UTankPickupPopupWidget::ShowMessage(const FText& Message, float DisplayDuration)
{
	MessageText->SetText(Message);

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(RemoveTimerHandle);
		World->GetTimerManager().SetTimer(
			RemoveTimerHandle,
			this,
			&UTankPickupPopupWidget::RemovePopup,
			FMath::Max(DisplayDuration, 0.1f),
			false);
	}
}

void UTankPickupPopupWidget::NativeDestruct()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(RemoveTimerHandle);
	}

	Super::NativeDestruct();
}

void UTankPickupPopupWidget::RemovePopup()
{
	RemoveFromParent();
}

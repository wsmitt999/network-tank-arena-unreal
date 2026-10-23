#include "TankPickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "TankPawn.h"
#include "TankPickupPopupWidget.h"

ATankPickup::ATankPickup()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	NetUpdateFrequency = 10.f;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->InitSphereRadius(75.f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetGenerateOverlapEvents(true);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(CollisionComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetCanEverAffectNavigation(false);
}

void ATankPickup::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATankPickup::OnOverlapBegin);
	}
}

void ATankPickup::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{
		return;
	}

	if (bIsCollected)
	{
		return;
	}

	if (ATankPawn* TankPawn = Cast<ATankPawn>(OtherActor))
	{
		bIsCollected = true;
		
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		Multicast_AnnouncePickup(TankPawn);
		OnPickedUp(TankPawn);

		Destroy();
	}
}

void ATankPickup::OnPickedUp(ATankPawn* TankPawn)
{
	
}

void ATankPickup::Multicast_AnnouncePickup_Implementation(
	ATankPawn* CollectingTank)
{
	if (!CollectingTank)
	{
		return;
	}

	FString Message;

	APlayerController* LocalPC =
		UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (LocalPC && LocalPC->GetPawn() == CollectingTank)
	{
		Message = TEXT("You picked up the artifact!");
	}
	else
	{
		FString PlayerName = TEXT("An unknown player");
		int32 PlayerId = -1;

		if (APlayerState* CollectingPS =
			CollectingTank->GetPlayerState())
		{
			PlayerName = CollectingPS->GetPlayerName();
			PlayerId = CollectingPS->GetPlayerId();
		}

		Message = FString::Printf(
			TEXT("%s (ID: %d) picked up the artifact!"),
			*PlayerName,
			PlayerId
		);
	}

	if (LocalPC)
	{
		if (UTankPickupPopupWidget* Popup =
			CreateWidget<UTankPickupPopupWidget>(LocalPC))
		{
			Popup->AddToViewport(100);
			Popup->ShowMessage(FText::FromString(Message));
		}
	}
}

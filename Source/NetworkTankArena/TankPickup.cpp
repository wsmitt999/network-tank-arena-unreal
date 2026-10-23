#include "TankPickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "TankPawn.h"

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

    if (ATankPawn* TankPawn = Cast<ATankPawn>(OtherActor))
    {
        APlayerState* CollectingPS = TankPawn->GetPlayerState();

        Multicast_AnnouncePickup(CollectingPS);

        OnPickedUp(TankPawn);
        Destroy();
    }
}

void ATankPickup::OnPickedUp(ATankPawn* TankPawn)
{
    // Reward can be added here
}

void ATankPickup::Multicast_AnnouncePickup_Implementation(APlayerState* CollectingPlayerState)
{
    APlayerController* LocalPC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!LocalPC)
    {
        return;
    }

    FString Message;
    
    if (CollectingPlayerState && CollectingPlayerState == LocalPC->PlayerState)
    {
        Message = TEXT("You picked up the artifact!");
    }
    else
    {
        FString PlayerName = CollectingPlayerState ? CollectingPlayerState->GetPlayerName() : TEXT("An unknown player");
        int32 PlayerId = CollectingPlayerState ? CollectingPlayerState->GetPlayerId() : -1;

        Message = FString::Printf(TEXT("%s (ID: %d) picked up the artifact!"), *PlayerName, PlayerId);
    }
    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, Message);
    }
    
}
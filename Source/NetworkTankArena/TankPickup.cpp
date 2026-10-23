#include "TankPickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TankPawn.h"

ATankPickup::ATankPickup()
{
    PrimaryActorTick.bCanEverTick = false;

    bReplicates = true;
    NetUpdateFrequency = 10.f; 

    // Setup Collision Root
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    SetRootComponent(CollisionComponent);
    CollisionComponent->InitSphereRadius(75.f);
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetGenerateOverlapEvents(true);

    // Setup Visual Mesh
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
        OnPickedUp(TankPawn);
        Destroy();
    }
}

void ATankPickup::OnPickedUp(ATankPawn* TankPawn)
{

}
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TankPickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class ATankPawn;
class APlayerState; // Forward declaration

UCLASS()
class NETWORKTANKARENA_API ATankPickup : public AActor
{
	GENERATED_BODY()
    
public:    
	ATankPickup();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult
	);

	virtual void OnPickedUp(ATankPawn* TankPawn);

	// Multicast RPC to announce the pickup to all clients
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AnnouncePickup(APlayerState* CollectingPlayerState);
};
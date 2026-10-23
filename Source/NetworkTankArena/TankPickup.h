#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TankPickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class ATankPawn;

UCLASS()
class NETWORKSTANKARENA_API ATankPickup : public AActor
{
	GENERATED_BODY()
    
public:    
	ATankPickup();

protected:
	virtual void BeginPlay() override;

	// Overlap collision volume
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComponent;

	// Visual mesh 
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
};
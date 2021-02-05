// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "FPSBlackHole.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class STEALTH_ASSASSIN_API AFPSBlackHole : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPSBlackHole();

	// mesh for teh black hole
	UPROPERTY(VisibleAnywhere, Category = "Components");
	UStaticMeshComponent* MeshComp;
	
	// Sphere that is a magnet
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackHole")
	USphereComponent* MagnetBlackHole;

	// Sphere that is a destroyer
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackHole")
	USphereComponent* DestroyerBlackHole;

	TArray<UPrimitiveComponent*> OverlappedActors;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PullInActors();

	UFUNCTION()
	void OverlapInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

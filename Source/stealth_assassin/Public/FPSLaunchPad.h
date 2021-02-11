// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLaunchPad.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UParticleSystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEALTH_ASSASSIN_API AFPSLaunchPad : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AFPSLaunchPad();

protected:

	UPROPERTY(EditInstanceOnly, Category = "LaunchPad")
	float LaunchVelocity;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	// Box that is our trigger
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LaunchPad")
	UBoxComponent* LaunchPadComp;

	// Effect to trigger after launch
	UPROPERTY(EditInstanceOnly, Category = "LaunchPad")
	UParticleSystem* ActivateLaunchPadEffect;

	TArray<UPrimitiveComponent*> OverlappedActors;

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		
};

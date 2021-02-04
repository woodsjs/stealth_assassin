// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSObjectiveActor.generated.h"

class USphereComponent;

UCLASS()
class STEALTH_ASSASSIN_API AFPSObjectiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSObjectiveActor();

protected:
	// Called when the game starts or when spawned

	UPROPERTY(VisibleAnywhere, Category = "Components");
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components");
	USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly, Category = "Effects");
	UParticleSystem* PickupFX;

	UPROPERTY(EditDefaultsOnly, Category = "Effects");
	UParticleSystem* LocateFX;

	virtual void BeginPlay() override;

	void PlayEffects(UParticleSystem* FX);


public:	

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};

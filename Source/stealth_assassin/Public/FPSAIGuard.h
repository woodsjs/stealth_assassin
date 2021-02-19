// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;
class ATargetPoint;

// Needs to be uint8 when exposing to blueprint
UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,
	Suspicious,
	Alerted
};

UCLASS()
class STEALTH_ASSASSIN_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// handle data for AI rotation onSeen and onHeard
	bool isRotating;
	FVector RotateToLocation;
	FRotator OriginalRotation;
	FTimerHandle TimerHandle_ResetOrientation;

	// handle visual above AI, ? or !
	EAIState GuardState;

	// Handle AI walking around map
	UPROPERTY(EditInstanceOnly, category = "AI", meta = (EditCondition = "CanWander"))
	TArray<AActor*> Waypoints;

	UPROPERTY(EditInstanceOnly, category = "AI")
	bool CanWander;

	FTimerHandle TimerHandle_Wander;
	AActor* NextTarget;
	bool bIsWandering;

	UPROPERTY(VisibleAnywhere, category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	// used to set the length of time the AI will turn towards a noise
	UPROPERTY(EditInstanceOnly, category = "AI")
	float DistractionTime = 3.0f;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

	// Go back to start pose after being distracted
	UFUNCTION()
	void ResetOrientation();

	// if the guard is alerted or idle or suspicious
	void SetGuardState(EAIState NewState);

	// In our blueprint, we can build what happens when the guard moves between states
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void onGuardStateChanged(EAIState NewState);

	// functions for AI to move around the map
	// Of all the waypoints, which one are we going to move to?
	UFUNCTION()
	void ChooseAvailableWaypoint();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

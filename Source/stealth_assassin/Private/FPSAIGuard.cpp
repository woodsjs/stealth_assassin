// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "AIModule/Classes/Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "FPSGameMode.h"
#include "UnrealNetwork.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject <UPawnSensingComponent>(TEXT("PawnSensingComp"));

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);

	SetGuardState(EAIState::Idle);
}	

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();

	OriginalRotation = GetActorRotation();
	isRotating = false;

	if (CanWander)
	{
		bIsWandering = false;
		//MoveToWaypoint();
		ChooseAvailableWaypoint();
	}
	
}

void AFPSAIGuard::OnRep_GuardState()
{
	onGuardStateChanged(GuardState);

}

void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{

	if (SeenPawn == nullptr)
	{
		return;
	}

	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);

	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->CompleteMission(SeenPawn, false);
	}

	SetGuardState(EAIState::Alerted);
}

void AFPSAIGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{

	// We don't need to be suspicious of a sound if we're already alerted
	// because being alerted means we saw the player
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);
	RotateToLocation = Location;
	
	// let's set is rotating here, then tick should fire out code?
	// we would then setactorrotation using FMath::Lerp, which is what I thought!
	isRotating = true;

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, DistractionTime);

	SetGuardState(EAIState::Suspicious);
}

void AFPSAIGuard::ResetOrientation()
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

	//SetActorRotation(NewLookAt);
	SetActorRotation(OriginalRotation);

	SetGuardState(EAIState::Idle);
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState)
	{
		return;
	}

	GuardState = NewState;
	OnRep_GuardState();

	//onGuardStateChanged(NewState);
}

void AFPSAIGuard::ChooseAvailableWaypoint()
{
	if (GuardState == EAIState::Idle)
	{
		//int index = rand() % Waypoints.Num();
		auto index = FMath::RandRange(0, Waypoints.Num() - 1);

		NextTarget = Waypoints[index];

		// We have a waypoint, so wander
		bIsWandering = true;

		// we could use unavigationsystem::simplemovetoactor()
		// you would then use stopmovement to stop them!
	}
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isRotating)
	{
		FVector Direction = RotateToLocation - GetActorLocation();
		Direction.Normalize();

		FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
		NewLookAt.Pitch = 0.0f;
		NewLookAt.Roll = 0.0f;

		//SetActorRotation(NewLookAt);
		SetActorRotation(FMath::Lerp(GetActorRotation(), NewLookAt, 1.0f));
		isRotating = false;

	}

	if (bIsWandering && (GuardState == EAIState::Idle))
	{
		// move to that waypoint
		if (NextTarget == nullptr)
		{
			bIsWandering = false;
			return;
		}

		FVector direction = NextTarget->GetActorLocation() - GetActorLocation();
		direction.Normalize();

		// Our Z is wack. Just push it to zero cause we don't care.
		direction.Z = 0.0f;

		FRotator newLookAt = FRotationMatrix::MakeFromX(direction).Rotator();
		newLookAt.Pitch = 0.0f;
		newLookAt.Roll = 0.0f;

		GetRootComponent()->MoveComponent(direction, newLookAt, true);

		// we use isnearlyzero cause nothing is sacred. 
		// if we're there, just stop the charade.
		if (direction.IsZero() || direction.IsNearlyZero(0.01))
		{
			bIsWandering = false;

			// wait a tick
			GetWorldTimerManager().ClearTimer(TimerHandle_Wander);
			GetWorldTimerManager().SetTimer(TimerHandle_Wander, this, &AFPSAIGuard::ChooseAvailableWaypoint, 3.0f);
		}
	}
}

	void AFPSAIGuard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > &OutLifetimeProps) const
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(AFPSAIGuard, GuardState);
	}




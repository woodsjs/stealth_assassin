// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchPad.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
//#include "GameFramework/Character.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
AFPSLaunchPad::AFPSLaunchPad()
{

	LaunchPadComp = CreateDefaultSubobject<UBoxComponent>(TEXT("LaunchPadComp"));
	LaunchPadComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LaunchPadComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	LaunchPadComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	LaunchPadComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	//LaunchPadComp->SetBoxExtent(FVector(200.0f));
	RootComponent = LaunchPadComp;

	//LaunchPadComp->SetHiddenInGame(false);
	LaunchPadComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::HandleOverlap);


	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	LaunchVelocity = 1000;
}


// We're going to launch either the pawn, or an actor if it's moved into the zone
// But we have to handle those differently.  We can launch a pawn, but have to apply an impulse to an actor.
void AFPSLaunchPad::HandleOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Another way to get this We would need a LaunchAngle param
	//FRotator LaunchDirection = GetActorRotation();
	//LaunchDirection.Pitch = LaunchAngle;
	//FVector LaunchVector = LaunchDirection.Vector() * LaunchVelocity;

	FVector forwardVector = this->GetActorForwardVector();
	FVector upVector = this->GetActorUpVector();

	// We need a forward/upwards vector to launch the player in
	FVector launchVector = forwardVector.operator+(upVector).operator*(LaunchVelocity);

	AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);
	if (MyPawn != nullptr)
	{
		MyPawn->LaunchCharacter(launchVector, true, true);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchPadEffect, GetActorLocation());
	}
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		// We need to use this as a change in velocity.
		OtherComp->AddImpulse(launchVector, NAME_None, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchPadEffect, GetActorLocation());
	}



}


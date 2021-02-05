// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBlackHole.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	// Use a sphere as a simple collision representation
	MagnetBlackHole = CreateDefaultSubobject<USphereComponent>(TEXT("MagnetBlackHoleComp"));
	MagnetBlackHole->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MagnetBlackHole->SetCollisionResponseToAllChannels(ECR_Ignore);
	MagnetBlackHole->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	MagnetBlackHole->InitSphereRadius(3000.0f);

	MagnetBlackHole->SetupAttachment(MeshComp);

	DestroyerBlackHole = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	DestroyerBlackHole->InitSphereRadius(5.0f);
	DestroyerBlackHole->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DestroyerBlackHole->SetCollisionProfileName("BlackHole");
	DestroyerBlackHole->SetCollisionResponseToAllChannels(ECR_Ignore);
	DestroyerBlackHole->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	DestroyerBlackHole->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OverlapInnerSphere);

	DestroyerBlackHole->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("About to pull in actors"));
	PullInActors();
	
}

void AFPSBlackHole::PullInActors()
{

	MagnetBlackHole->GetOverlappingComponents(OverlappedActors);

	for (UPrimitiveComponent* primComp : OverlappedActors)
	{
		primComp->SetGenerateOverlapEvents(true);
	}
	
}

void AFPSBlackHole::OverlapInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Only add impulse and destroy projectile if we hit a physics
	UE_LOG(LogTemp, Display, TEXT("Hitters"));

	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		//OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		OtherActor->Destroy();
	}
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (UPrimitiveComponent* primComp : OverlappedActors)
	{
		//UE_LOG(LogTemp, Display, TEXT("Loops"));
		primComp->AddRadialForce(GetActorLocation(), MagnetBlackHole->GetUnscaledSphereRadius(), -2000, RIF_Constant, true);
	}

}
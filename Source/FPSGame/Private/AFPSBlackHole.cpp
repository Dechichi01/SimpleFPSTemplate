// Fill out your copyright notice in the Description page of Project Settings.

#include "AFPSBlackHole.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Delegates/Delegate.h"

// Sets default values
AAFPSBlackHole::AAFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	InnerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Inner Sphere"));
	InnerSphere->InitSphereRadius(100);
	InnerSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	InnerSphere->OnComponentBeginOverlap.AddDynamic(this, &AAFPSBlackHole::InnerSphereOverlap);
	InnerSphere->SetupAttachment(MeshComp);

	OutterSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Outter Sphere"));
	OutterSphere->InitSphereRadius(3000);
	OutterSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	OutterSphere->SetupAttachment(MeshComp);

	RadialImpulse = ERadialImpulseFalloff::RIF_Constant;
	PullForce = 2000.f;
}

// Called when the game starts or when spawned
void AAFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAFPSBlackHole::InnerSphereOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}

// Called every frame
void AAFPSBlackHole::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappingComps;
	OutterSphere->GetOverlappingComponents(OverlappingComps);

	for (int32 i = 0; i < OverlappingComps.Num(); i++)
	{
		UPrimitiveComponent* PrimitiveComp = OverlappingComps[i];
		if (PrimitiveComp && PrimitiveComp->IsSimulatingPhysics())
		{
			const float SphereRadius = OutterSphere->GetScaledSphereRadius();
			PrimitiveComp->AddRadialForce(GetActorLocation(), SphereRadius, -PullForce, RadialImpulse, true);
		}
	}
}


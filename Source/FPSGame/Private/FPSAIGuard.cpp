// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "FPSGameMode.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSenseComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn sensing"));

	PawnSenseComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnSeenPawn);
	PawnSenseComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);

	InitialRotation = GetActorRotation();
	
	CurrentState = EAIState::Idle;
	PatrolPointDistThreshold = 50.f;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();	

	CurrentPatrolPointIndex = -1;
	MoveToNextPatrolPoint();
}

void AFPSAIGuard::OnSeenPawn(APawn * Pawn)
{
	UE_LOG(LogTemp, Log, TEXT("OnSeenPawn called"));
	if (Pawn == nullptr)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Pawn not null"));
	DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(), 32.f, 12, FColor::Red, false, 10.f);

	AFPSGameMode* GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	ensure(GameMode);

	GameMode->CompleteMission(Pawn, false);

	SetGuardState(EAIState::Alerted);
	StopPatrol();
}

void AFPSAIGuard::OnNoiseHeard(APawn * Pawn, const FVector & Location, float Volume)
{
	UE_LOG(LogTemp, Log, TEXT("OnHead something"));
	DrawDebugSphere(GetWorld(), Location, 32.f, 12, FColor::Green, false, 10.f);

	FVector Dir = Location - GetActorLocation();

	auto NewLookAt = FRotationMatrix::MakeFromX(Dir).Rotator();
	NewLookAt.Pitch = 0;
	NewLookAt.Roll = 0;
	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(ResetLookAtTime);
	GetWorldTimerManager().SetTimer(ResetLookAtTime, this, &AFPSAIGuard::ResetOrientation, 3.f);

	SetGuardState(EAIState::Suspicious);
	StopPatrol();
}

void AFPSAIGuard::ResetOrientation()
{
	SetGuardState(EAIState::Idle);
	SetActorRotation(InitialRotation);

	MoveToNextPatrolPoint();
}

void AFPSAIGuard::MoveToNextPatrolPoint()
{
	if (!bPatrol || PatrolPoints.Num() == 0)
	{
		return;
	}

	if (++CurrentPatrolPointIndex >= PatrolPoints.Num())
	{
		CurrentPatrolPointIndex = 0;
	}

	UE_LOG(LogTemp, Log, TEXT("Setting timet to next spawn position %d"), CurrentPatrolPointIndex)

	GetWorldTimerManager().ClearTimer(WaitPatrolPointTimer);
	GetWorldTimerManager().SetTimer(WaitPatrolPointTimer, this, &AFPSAIGuard::ExecuteMoveToNextPatrolPoint, 3.f);
}

void AFPSAIGuard::StopPatrol()
{
	Controller->StopMovement();
}

void AFPSAIGuard::ExecuteMoveToNextPatrolPoint()
{
	if (this == nullptr)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Start move to next spawn position %d"), CurrentPatrolPointIndex)

	const AActor* CurrentPatrolPoint = PatrolPoints[CurrentPatrolPointIndex];
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPatrolPoint);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bPatrol)
	{
		FVector Delta = GetActorLocation() - PatrolPoints[CurrentPatrolPointIndex]->GetActorLocation();

		if (Delta.SizeSquared() < PatrolPointDistThreshold*PatrolPointDistThreshold)
		{
			MoveToNextPatrolPoint();
		}
	}
}

// Called to bind functionality to input
void AFPSAIGuard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFPSAIGuard::SetGuardState(EAIState State)
{
	if (CurrentState == EAIState::Alerted)
	{
		return;
	}

	CurrentState = State;

	OnStateChanged(CurrentState);
}


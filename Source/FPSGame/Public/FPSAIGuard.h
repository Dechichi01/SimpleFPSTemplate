// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

UENUM()
enum class EAIState : uint8
{
	Idle = 0,
	Suspicious,
	Alerted,
};


UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	FRotator InitialRotation;

	FTimerHandle ResetLookAtTime;
	FTimerHandle WaitPatrolPointTimer;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UPawnSensingComponent* PawnSenseComp;

	UFUNCTION()
	void OnSeenPawn(APawn* Pawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* Pawn, const FVector& Location, float Volume);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
		void OnStateChanged(EAIState NewState);

	UPROPERTY(EditAnywhere, Category = "AI")
	EAIState CurrentState;

	UPROPERTY(EditInstanceOnly, Category = "AI")
	bool bPatrol;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))
	TArray<AActor*> PatrolPoints;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))
	float PatrolPointDistThreshold;

	uint8 CurrentPatrolPointIndex;

	void SetGuardState(EAIState State);

	void ResetOrientation();
	
	void MoveToNextPatrolPoint();

	void StopPatrol();

	void ExecuteMoveToNextPatrolPoint();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};

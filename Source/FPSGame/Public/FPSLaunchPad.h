// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLaunchPad.generated.h"

UCLASS()
class FPSGAME_API AFPSLaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSLaunchPad();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UBoxComponent* OverlapComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UParticleSystem* OverlapFX;

	UPROPERTY(EditInstanceOnly, Category = "Launch parameters")
	float LaunchStrenght;

	UPROPERTY(EditInstanceOnly, Category = "Launch parameters")
	float LaunchPitchAngleDegres;

	UFUNCTION()
	void OverlapLaunchPad(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AFPSBlackHole.generated.h"

UCLASS()
class FPSGAME_API AAFPSBlackHole : public AActor
{
	GENERATED_BODY()

  public:
	// Sets default values for this actor's properties
	AAFPSBlackHole();

  protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	class USphereComponent* InnerSphere;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	USphereComponent* OutterSphere;

	UPROPERTY(EditDefaultsOnly, Category = "Physics Properties")
	TEnumAsByte<ERadialImpulseFalloff> RadialImpulse;

	UPROPERTY(EditDefaultsOnly, Category = "Physics Properties")
	float PullForce;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void InnerSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

  public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

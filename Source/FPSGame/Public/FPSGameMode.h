// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

UCLASS()
class AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Game Over")
	TSubclassOf<AActor> SpectatingViewpointClas;

public:

	AFPSGameMode();

	UPROPERTY(EditAnywhere, Category = "Game Over")
	float GoToExpectationBlendTime;

	UPROPERTY(EditAnywhere, Category = "Game Over")
	TEnumAsByte<EViewTargetBlendFunction> GoToExpectationBlendFunc;

	void CompleteMission(APawn* InstigatorPawn, bool bIsSuccess);

	UFUNCTION(BlueprintImplementableEvent, Category = "Game Mode")
	void OnMissionCompleted(APawn* InstigatorPawn, bool bIsSuccess);
};




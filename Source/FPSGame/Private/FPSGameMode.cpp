// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();

	GoToExpectationBlendTime = 0.5f;
	GoToExpectationBlendFunc = EViewTargetBlendFunction::VTBlend_Cubic;
}

void AFPSGameMode::CompleteMission(APawn * InstigatorPawn, bool bIsSuccess)
{
	if (InstigatorPawn)
	{
		InstigatorPawn->DisableInput(nullptr);
	}

	OnMissionCompleted(InstigatorPawn, bIsSuccess);

	TArray<AActor*> FoundExpectators;
	UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClas, FoundExpectators);

	if (FoundExpectators.Num() > 0)
	{
		auto Controller = Cast<APlayerController>(InstigatorPawn->GetController());
		ensure(Controller);

		Controller->SetViewTargetWithBlend(FoundExpectators[0], GoToExpectationBlendTime, GoToExpectationBlendFunc);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No expectators found"));
	}
}

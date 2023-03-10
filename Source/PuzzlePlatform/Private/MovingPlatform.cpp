// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);
	
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		// Local -> Global

		//GEngine->AddOnScreenDebugMessage(1, -1, FColor::Cyan, FString::Printf(TEXT("GlobalTargetLocation x : %f, y : %f, z : %f"), GlobalTargetLocation.X, GlobalTargetLocation.Y, GlobalTargetLocation.Z));
		
		FVector DirectionVector;
		if (bIsTurn)
		{
			DirectionVector = (GlobalStartLocation - GlobalTargetLocation).GetSafeNormal();
			Distance = (GlobalTargetLocation - GetActorLocation()).Length();
		}
		else
		{
			DirectionVector = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
			Distance = (GlobalStartLocation - GetActorLocation()).Length();
		}

		SetActorLocation(GetActorLocation() + DirectionVector * DeltaTime * MovementSpeed);

		if (Distance > (GlobalStartLocation - GlobalTargetLocation).Length()) bIsTurn = !bIsTurn;
	}
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
		GlobalStartLocation = GetActorLocation();
		GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
		Distance = (GlobalTargetLocation - GlobalStartLocation).Length();
	}
}
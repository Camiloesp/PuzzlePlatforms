// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"


AMovingPlatform::AMovingPlatform() 
{
    PrimaryActorTick.bCanEverTick = true;
    SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay() 
{
    Super::BeginPlay();

    if(HasAuthority())
    {
        SetReplicates(true);
        SetReplicateMovement(true);
    }

    GlobalStartLocation = GetActorLocation();
    GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void AMovingPlatform::AddActiveTrigger() 
{
    ActiveTriggers++;
}

void AMovingPlatform::RemoveActiveTrigger() 
{
    if(ActiveTriggers>0)
    {
        ActiveTriggers--;
    }
}

void AMovingPlatform::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime); 

    if(ActiveTriggers > 0)
    {
        if(HasAuthority()) // Not on server is cliet == !HasAuthority. HasAuthority() returns true if this actor has network authority
        {
            FVector Location = GetActorLocation();
            float JourneyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
            float JourneyTravelled = (Location - GlobalStartLocation).Size();

            if(JourneyTravelled >= JourneyLength)
            {
                FVector Swap = GlobalStartLocation;
                GlobalStartLocation = GlobalTargetLocation;
                GlobalTargetLocation = Swap;
            }

            FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
            Location += Speed * DeltaTime * Direction;
            SetActorLocation(Location);
        }
    }


}

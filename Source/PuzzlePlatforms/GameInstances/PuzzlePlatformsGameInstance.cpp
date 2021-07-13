// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h" //FClassFinder
#include "PuzzlePlatforms/Actors/PlatformTrigger.h"
#include "Blueprint/UserWidget.h"


UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer) 
{
    ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
    if(!ensure(MenuBPClass.Class != nullptr)) return;
    MenuClass = MenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init() 
{
    Super::Init();
    UE_LOG(LogTemp, Warning, TEXT("Found class: %s"), *MenuClass->GetName());
}

void UPuzzlePlatformsGameInstance::LoadMenu() 
{
    if(!ensure(MenuClass != nullptr)) return;
    UUserWidget* Menu = CreateWidget<UUserWidget>(this, MenuClass);
    if(!ensure(Menu != nullptr)) return;

    Menu->AddToViewport();
}

void UPuzzlePlatformsGameInstance::Host() 
{
    UEngine* Engine = GetEngine();
    if(!ensure(Engine != nullptr)) return;

    Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

    // Change from lobby to other level
    UWorld* World = GetWorld();
    if(!ensure(World != nullptr)) return;
    World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address) 
{
    UEngine* Engine = GetEngine();
    if(!ensure(Engine != nullptr)) return;

    Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining... %s"), *Address));

    // join an existing level
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if(!ensure(PlayerController != nullptr)) return;
    PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

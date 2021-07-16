// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h" //FClassFinder
#include "PuzzlePlatforms/Actors/PlatformTrigger.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Blueprint/UserWidget.h"
#include "PuzzlePlatforms/Widgets/MainMenu.h"
#include "PuzzlePlatforms/Widgets/MenuWidget.h"


const static FName SESSION_NAME = TEXT("My Session Game");


UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer) 
{
    ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
    if(!ensure(MenuBPClass.Class != nullptr)) return;
    MenuClass = MenuBPClass.Class;

    ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
    if(!ensure(InGameMenuBPClass.Class != nullptr)) return;
    InGameMenuClass = InGameMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init() 
{
    Super::Init();
    //Online Subsystem starts here
    IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get();
    if(SubSystem != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Online Sub-System Found: %s"), *SubSystem->GetSubsystemName().ToString()); //GetNamedInterface()  GetLocalPlatformName()
        //auto SessionInterface = SubSystem->GetSessionInterface();
        SessionInterface = SubSystem->GetSessionInterface();
        if(SessionInterface.IsValid())
        {
            SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
            SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
        }
    } 
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No Sub-System Found"));
    }
}

void UPuzzlePlatformsGameInstance::LoadMenuWidget() 
{
    if(!ensure(MenuClass != nullptr)) return;
    Menu = CreateWidget<UMainMenu>(this, MenuClass);
    if(!ensure(Menu != nullptr)) return;

    Menu->Setup();
    Menu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::InGameLoadMenu() 
{
    if(!ensure(InGameMenuClass != nullptr)) return;
    UMenuWidget* InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
    if(!ensure(InGameMenu != nullptr)) return;

    InGameMenu->Setup();
    InGameMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::Host()
{
    if(SessionInterface.IsValid())
    {
        auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME); // auto is FNamedOnlineSession*
        if(ExistingSession != nullptr)
        {
            SessionInterface->DestroySession(SESSION_NAME);
        }
        else
        {
            CreateSession();
        }
    }
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success) 
{
    if(Success)
    {
        CreateSession();
    }
}

void UPuzzlePlatformsGameInstance::CreateSession() 
{
    if(SessionInterface.IsValid())
    {
        FOnlineSessionSettings SessionSettings;
        SessionInterface->CreateSession(0, SESSION_NAME , SessionSettings);
    }
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success) 
{
    if(!Success)
    {
        UE_LOG(LogTemp, Warning, TEXT("Could NOT create Session"));
        return;
    }
    if(Menu!=nullptr)
    {
        //Menu->Teardown();
        Menu->OnLevelRemovedFromWorld(GetWorld()->GetCurrentLevel(), GetWorld());
    }

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

void UPuzzlePlatformsGameInstance::LoadMainMenu() 
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if(!ensure(PlayerController != nullptr)) return;
    PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}


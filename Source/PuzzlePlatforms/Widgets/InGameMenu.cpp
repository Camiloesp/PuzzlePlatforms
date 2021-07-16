// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"

#include "Components/Button.h"

#include "GameFramework/GameModeBase.h" // ?

bool UInGameMenu::Initialize() 
{
    bool Success = Super::Initialize();
    if(!Success) return false;

    if(!ensure(CancelButton != nullptr)) return false;
    CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::CancelledPressed);

    if(!ensure(CancelButton != nullptr)) return false;
    QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitPressed);

    return true;
}

void UInGameMenu::CancelledPressed() 
{
    OnLevelRemovedFromWorld(GetWorld()->GetCurrentLevel(), GetWorld());
}

void UInGameMenu::QuitPressed() 
{
    if(MenuInterface != nullptr)
    {
        OnLevelRemovedFromWorld(GetWorld()->GetCurrentLevel(), GetWorld());
        MenuInterface->LoadMainMenu();
    }

    // APlayerController* PlayerController = GetOwningPlayer();
    // if (!ensure(PlayerController != nullptr)) return;
 
    // UWorld* World = PlayerController->GetWorld();
    // if (!ensure(World != nullptr)) return;
 
    // if (World->IsServer()) {
    //     AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>();
    //     if (GameMode) {
    //         GameMode->ReturnToMainMenuHost();
    //     }
    // } else {
        
    //     if (PlayerController) {
    //         PlayerController->ClientReturnToMainMenu("Back to main menu");
    //     }
    // }
    // UE_LOG(LogTemp, Warning, TEXT("Returning safely"));
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"


bool UMainMenu::Initialize() 
{
    bool Success = Super::Initialize();
    if(!Success) return false;

    //TODO: setup
    if(!ensure(HostButton != nullptr)) return false;
    HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
    
    if(!ensure(JoinButton != nullptr)) return false;
    JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

    if(!ensure(CancelJoinMenuButton != nullptr)) return false;
    CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

    if(!ensure(ConfirmJoinMenuButton != nullptr)) return false;
    ConfirmJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

    if(!ensure(QuitButton != nullptr)) return false;
    QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitPressed);

    return true;
}

void UMainMenu::HostServer() 
{
    if(MenuInterface != nullptr)
    {
        MenuInterface->Host();
    }
    //UE_LOG(LogTemp, Warning, TEXT("I'm Hosting a Server"));
}

void UMainMenu::JoinServer() 
{
    if(MenuInterface != nullptr)
    {
        if(!ensure(IPAddressField != nullptr)) return;
        const FString &Address = IPAddressField->GetText().ToString();
        MenuInterface->Join(Address);
    }
}

void UMainMenu::OpenJoinMenu() 
{
    //UE_LOG(LogTemp, Warning, TEXT("I'm Joining a Server"));
    if(!ensure(MenuSwitcher != nullptr)) return;
    if(!ensure(JoinMenu != nullptr)) return;
    MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::OpenMainMenu() 
{
    if(!ensure(MenuSwitcher != nullptr)) return;
    if(!ensure(MainMenu != nullptr)) return;
    MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitPressed() 
{
    // UWorld* World = GetWorld();
    // if(!ensure(World != nullptr)) return;

    // APlayerController* PlayerController = World->GetFirstPlayerController();
    // if(!ensure(PlayerController != nullptr)) return;

    // PlayerController->ConsoleCommand("quit");


    FGenericPlatformMisc::RequestExit(false); // exits from editor
}



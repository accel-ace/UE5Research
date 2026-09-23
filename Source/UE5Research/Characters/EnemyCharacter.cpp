// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "GameModes/CppGameplayGameMode.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyCharacter::HandleEnemyDeath()
{
    if (bIsDead)
    {
        return;
    }
    
    bIsDead = true;
    
    AGameModeBase* BaseGameMode =
    UGameplayStatics::GetGameMode(this);
    
    if (ACppGameplayGameMode* GameMode =
        Cast<ACppGameplayGameMode>(BaseGameMode))
    {
        GameMode->AddDefeatedEnemyCount();
        
        UE_LOG(
           LogTemp,
           Log,
           TEXT("Enemy defeated. Count added.")
       );
    }
    else
    {
        UE_LOG(
           LogTemp,
           Warning,
           TEXT("Failed to get AMyGameMode.")
       );
    }
    
    Destroy();
}

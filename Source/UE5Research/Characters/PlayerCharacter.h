// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE5ResearchCharacter.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UHealthComponent;

UCLASS()
class UE5RESEARCH_API APlayerCharacter : public AUE5ResearchCharacter
{
    GENERATED_BODY()
    
public:
    // Sets default values for this character's properties
    APlayerCharacter();
    
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    
    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category="Combat")
    void Attack();
    
    void SetCombatInputEnabled(bool bEnabled);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    
    UFUNCTION()
    void HandleDeath();
    
    UFUNCTION(Server, Reliable)
    void ServerAttack();

    void PerformAttack();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    TObjectPtr<UHealthComponent> HealthComponent;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
    float AttackDamage = 25.0f;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
    float AttackRange = 500.0f;
    
    bool bCanAttack = true;

};

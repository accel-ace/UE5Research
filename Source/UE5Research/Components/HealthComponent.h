// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FOnHealthChanged,
    float, CurrentHealth,
    float, MaxHealth
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5RESEARCH_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()
    
    public:	
    // Sets default values for this component's properties
    UHealthComponent();
        
    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty>& OutLifetimeProps
    ) const override;

    void InitializeHealth(float InMaxHealth);
    
    UFUNCTION(BlueprintCallable, Category = "Health")
    void ApplyDamage(float DamageAmount);
    
    UFUNCTION(BlueprintCallable, Category = "Health")
    void Heal(float HealAmount);
    
    UFUNCTION(BlueprintPure, Category = "Health")
    bool IsDead() const;
    
    UFUNCTION(BlueprintPure, Category = "Health")
    float GetCurrentHealth() const;
    
    UFUNCTION(BlueprintPure, Category = "Health")
    float GetMaxHealth() const;
    
    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnHealthChanged OnHealthChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnDeath OnDeath;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (ClampMin = "1.0"))
    float MaxHealth = 100.0f;
    
private:
    void ModifyHealth(float DeltaHealth);
    
    UFUNCTION()
    void OnRep_CurrentHealth();
    
    UPROPERTY(ReplicatedUsing=OnRep_CurrentHealth, VisibleAnywhere, Category = "Health")
    float CurrentHealth = 100.0f;
    
    UPROPERTY(VisibleAnywhere, Category = "Health")
    bool bIsDead = false;

    bool bIsInitialized = false;
};

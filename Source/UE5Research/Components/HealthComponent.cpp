// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    // Component自身のReplicationを有効化する
    SetIsReplicatedByDefault(true);

    CurrentHealth = MaxHealth;
}


void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    // HPの初期値を決定するのはサーバーだけ
    if (GetOwner() && GetOwner()->HasAuthority())
    {
        InitializeHealth(MaxHealth);
    }
}

void UHealthComponent::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps
) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UHealthComponent, CurrentHealth);
}

void UHealthComponent::InitializeHealth(float InMaxHealth)
{
    if (bIsInitialized)
    {
        return;
    }
    
    if (!GetOwner() || !GetOwner()->HasAuthority())
    {
        return;
    }
    
    MaxHealth = FMath::Max(InMaxHealth, 1.0f);
    CurrentHealth = MaxHealth;
    bIsDead = false;
    bIsInitialized = true;
    
    OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UHealthComponent::ApplyDamage(float DamageAmount)
{
    // HP変更の決定権はサーバーだけが持つ
    if (!GetOwner() || !GetOwner()->HasAuthority())
    {
        return;
    }
    
    if (bIsDead || DamageAmount <= 0.0f)
    {
        return;
    }

    const float PreviousHealth = CurrentHealth;

    ModifyHealth(-DamageAmount);
    
    UE_LOG(
        LogTemp,
        Log,
        TEXT("Health Changed | Owner:%s | Previous:%.1f | Current:%.1f"),
        *GetNameSafe(GetOwner()),
        MaxHealth,
        CurrentHealth
    );
}

void UHealthComponent::Heal(float HealAmount)
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
    {
        return;
    }

    if (bIsDead || HealAmount <= 0.0f)
    {
        return;
    }

    ModifyHealth(HealAmount);
}

void UHealthComponent::ModifyHealth(float DeltaHealth)
{
    const float PreviousHealth = CurrentHealth;

    CurrentHealth = FMath::Clamp(
        CurrentHealth + DeltaHealth,
        0.0f,
        MaxHealth
    );

    if (FMath::IsNearlyEqual(PreviousHealth, CurrentHealth))
    {
        return;
    }

    // Listen Server側を含むサーバー上の表示更新
    OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);

    if (!bIsDead && CurrentHealth <= 0.0f)
    {
        bIsDead = true;
        OnDeath.Broadcast();
    }
}

void UHealthComponent::OnRep_CurrentHealth()
{
    const bool bWasDead = bIsDead;

    bIsDead = CurrentHealth <= 0.0f;

    // Replicationを受け取ったクライアント側の表示更新
    OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);

    if (!bWasDead && bIsDead)
    {
        OnDeath.Broadcast();
    }
}

float UHealthComponent::GetCurrentHealth() const
{
    return CurrentHealth;
}

float UHealthComponent::GetMaxHealth() const
{
    return MaxHealth;
}

bool UHealthComponent::IsDead() const
{
    return bIsDead;
}

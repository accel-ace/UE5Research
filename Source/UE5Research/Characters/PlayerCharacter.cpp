// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"
#include "Engine/World.h"
#include "Components/HealthComponent.h"
#include "GameModes/CppGameplayGameMode.h"
#include "States/ResearchPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    bReplicates = true;

    HealthComponent =
        CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerCharacter::Attack()
{
    UE_LOG(
        LogTemp,
        Log,
        TEXT("Attack called | Player:%s | Local:%s | Authority:%s | LocalRole:%d"),
        *GetName(),
        IsLocallyControlled() ? TEXT("true") : TEXT("false"),
        HasAuthority() ? TEXT("true") : TEXT("false"),
        static_cast<int32>(GetLocalRole())
    );
    
    if (!bCanAttack)
    {
        UE_LOG(
            LogTemp,
            Log,
            TEXT("Attack input ignored: combat input disabled"));
        return;
    }
    
    if (!HealthComponent || HealthComponent->IsDead())
    {
        UE_LOG(
            LogTemp,
            Log,
            TEXT("Attack rejected: player is dead | Player:%s"),
            *GetNameSafe(this)
        );
        return;
    }
    // 入力した本人のCharacterだけが攻撃要求を送る
    if (!IsLocallyControlled())
    {
        UE_LOG(LogTemp, Log, TEXT("Attack stopped: not locally controlled"));
        return;
    }

    if (HealthComponent && HealthComponent->IsDead())
    {
        UE_LOG(LogTemp, Log, TEXT("Attack stopped: player is dead"));
        return;
    }

    if (HasAuthority())
    {
        UE_LOG(LogTemp, Log, TEXT("Attack executes directly on server"));
        PerformAttack();
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Attack sends ServerAttack RPC"));
        ServerAttack();
    }
}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    if (HealthComponent)
    {
        HealthComponent->OnDeath.AddDynamic(
            this,
            &APlayerCharacter::HandleDeath
        );
    }
}

void APlayerCharacter::HandleDeath()
{
    if (!HasAuthority())
    {
        return;
    }

    UE_LOG(
        LogTemp,
        Log,
        TEXT("HandleDeath | Character:%s | Authority:Server"),
        *GetNameSafe(this));


    ACppGameplayGameMode* GameMode = GetWorld()->GetAuthGameMode<ACppGameplayGameMode>();
    
    if (GameMode)
    {
        GameMode->HandlePlayerDeath(this);
    }
    else
    {
        UE_LOG(
            LogTemp,
            Log,
            TEXT("HandleDeath: ResearchGameMode not found"));
    }
    
    UE_LOG(
        LogTemp,
        Log,
        TEXT("HandleDeath | Character:%s | Authority:%s"),
        *GetNameSafe(this),
        HasAuthority() ? TEXT("Server") : TEXT("Client")
    );

    // 移動を止める
    GetCharacterMovement()->DisableMovement();

    // 攻撃などの入力も受け付けないようにする
    bCanAttack = false;

    // 当たり判定を無効化
    GetCapsuleComponent()->SetCollisionEnabled(
        ECollisionEnabled::NoCollision
    );
}

void APlayerCharacter::ServerAttack_Implementation()
{
    UE_LOG(
        LogTemp,
        Log,
        TEXT("ServerAttack received | Player:%s | Authority:%s"),
        *GetName(),
        HasAuthority() ? TEXT("true") : TEXT("false")
    );
    
    if (!HealthComponent || HealthComponent->IsDead())
    {
        return;
    }

    if (ACppGameplayGameMode* GameMode =
            GetWorld()->GetAuthGameMode<ACppGameplayGameMode>())
    {
        if (GameMode->IsMatchFinished())
        {
            UE_LOG(
                LogTemp,
                Log,
                TEXT("Attack rejected: match already finished"));
            return;
        }
    }

    PerformAttack();
}

void APlayerCharacter::PerformAttack()
{
    if (!HasAuthority() || !GetWorld())
    {
        return;
    }

    FVector ViewLocation;
    FRotator ViewRotation;
    GetActorEyesViewPoint(ViewLocation, ViewRotation);

    const FVector TraceStart = ViewLocation;
    const FVector TraceEnd =
        TraceStart + ViewRotation.Vector() * AttackRange;

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    FHitResult HitResult;

    const bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        TraceStart,
        TraceEnd,
        ECC_Visibility,
        QueryParams
    );

    const FVector DebugEnd =
        bHit ? HitResult.ImpactPoint : TraceEnd;

    DrawDebugLine(
        GetWorld(),
        TraceStart,
        DebugEnd,
        bHit ? FColor::Red : FColor::Green,
        false,
        1.5f,
        0,
        2.0f
    );

    if (!bHit)
    {
        UE_LOG(
            LogTemp,
            Log,
            TEXT("Attack missed | Player: %s"),
            *GetName()
        );

        return;
    }

    AActor* HitActor = HitResult.GetActor();

    if (!HitActor || HitActor == this)
    {
        return;
    }

    UHealthComponent* TargetHealthComponent =
        HitActor->FindComponentByClass<UHealthComponent>();

    if (!TargetHealthComponent)
    {
        UE_LOG(
            LogTemp,
            Log,
            TEXT("Attack hit actor without HealthComponent | Target: %s"),
            *HitActor->GetName()
        );

        return;
    }

    TargetHealthComponent->ApplyDamage(AttackDamage);

    UE_LOG(
        LogTemp,
        Log,
        TEXT("Attack hit | Attacker: %s | Target: %s | Damage: %.1f | RemainingHealth: %.1f"),
        *GetName(),
        *HitActor->GetName(),
        AttackDamage,
        TargetHealthComponent->GetCurrentHealth()
    );
}

void APlayerCharacter::SetCombatInputEnabled(bool bEnabled)
{
    bCanAttack = bEnabled;
}

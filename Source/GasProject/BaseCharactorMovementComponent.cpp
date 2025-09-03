// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharactorMovementComponent.h"

#include "BaseCharacter.h"
#include "GameplayTagsManager.h"


// Sets default values for this component's properties
UBaseCharactorMovementComponent::UBaseCharactorMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SprintSpeedMultiplier = 1.4f;
	// ...
}


// Called when the game starts
void UBaseCharactorMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBaseCharactorMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                    FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	const auto Owner = Cast<ABaseCharacter>(GetOwner());
	if (!Owner) return;
	
	if (GetCurrentAcceleration().Length() > 0)
	{
		Owner->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Moving")));
		Owner->GetAbilitySystemComponent()->SetLooseGameplayTagCount(FGameplayTag::RequestGameplayTag(FName("State.Moving")), 1);
	}else
	{
		Owner->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Moving")));
	}
}


float UBaseCharactorMovementComponent::GetMaxSpeed() const
{
	const auto Owner = Cast<ABaseCharacter>(GetOwner());
	if (!Owner)
		return Super::GetMaxSpeed();

	if (!Owner->IsAlive())
		return 0;

	//if (Owner->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun"))))
	//	return 0;

	switch(MovementMode)
	{
	case MOVE_Walking:
	case MOVE_NavWalking:
		return IsCrouching() ? MaxWalkSpeedCrouched : bRequestToStartSprinting ? Owner->GetMoveSpeed() * SprintSpeedMultiplier : Owner->GetMoveSpeed();
	case MOVE_Falling:
		return FVector2D(Owner->GetVelocity()).Length();
	case MOVE_Swimming:
		return MaxSwimSpeed;
	case MOVE_Flying:
		return MaxFlySpeed;
	case MOVE_Custom:
		return MaxCustomMovementSpeed;
	case MOVE_None:
	default:
		return 0.f;
	}
}

void UBaseCharactorMovementComponent::StartSprinting()
{
	bRequestToStartSprinting = true;
}

void UBaseCharactorMovementComponent::StopSprinting()
{
	bRequestToStartSprinting = false;
}

void UBaseCharactorMovementComponent::SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode)
{
	Super::SetMovementMode(NewMovementMode, NewCustomMode);

	const auto Owner = Cast<ABaseCharacter>(GetOwner());
	if (!Owner) return;
	
	Owner->AbilitySystemComponent->RemoveLooseGameplayTags(
		UGameplayTagsManager::Get().RequestGameplayTagChildren(FGameplayTag::RequestGameplayTag("State.Movement")));

	switch (MovementMode)
	{
	case MOVE_Walking:
		Owner->AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Movement.Walking"));
		break;
	case MOVE_None:
		break;
	case MOVE_NavWalking:
		Owner->AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Movement.NavWalking"));
		break;
	case MOVE_Falling:
		Owner->AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Movement.Falling"));
		break;
	case MOVE_Swimming:
		Owner->AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Movement.Swimming"));
		break;
	case MOVE_Flying:
		Owner->AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Movement.Flying"));
		break;
	case MOVE_Custom:
		Owner->AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Movement.Custom"));
		break;
	case MOVE_MAX:
		Owner->AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Movement.MAX"));
		break;
	default: ;
	}
}

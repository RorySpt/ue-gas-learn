// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "BaseAttributeSet.h"
#include "BaseCharactorMovementComponent.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
	: Super(FObjectInitializer::Get().SetDefaultSubobjectClass<UBaseCharactorMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		BaseAbilitySet = AbilitySystemComponent->GetSet<UBaseAttributeSet>();

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAbilitySet->GetHealthAttribute()).AddUObject(this, &ABaseCharacter::OnHealthChangedNative);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAbilitySet->GetStaminaAttribute()).AddUObject(this, &ABaseCharacter::OnStaminaChangedNative);


		AddStartupEffects();

		AddDefaultAbilities();
	}

	//AbilitySystemComponent->ApplyGameplayEffectToSelf()
}

void ABaseCharacter::AddStartupEffects()
{
	auto EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	for (auto& GameplayEffect : StartupEffects)
	{
		auto NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 0, EffectContextHandle);

		if (NewHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data);
		}
	}
}

void ABaseCharacter::AddDefaultAbilities()
{
	for (auto Ability: CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability));
	}
}


// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::InitializeAbility(TSubclassOf<UGameplayAbility> AbilityToGet, int32 AbilityLevel)
{
	if (AbilitySystemComponent)
	{
		if (HasAuthority() && AbilityToGet)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityToGet, AbilityLevel));
		}
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseCharacter::GetHealthValues(float& Health, float& MaxHealth) const
{
	Health = BaseAbilitySet->GetHealth();
	MaxHealth = BaseAbilitySet->GetMaxHealth();
}

float ABaseCharacter::GetHealthRegenRate() const
{
	return BaseAbilitySet->GetHealthRegenRate();
}

void ABaseCharacter::GetStaminaValues(float& Stamina, float& MaxStamina) const
{
	Stamina = BaseAbilitySet->GetStamina();
	MaxStamina = BaseAbilitySet->GetMaxStamina();
}

float ABaseCharacter::GetStaminaRegenRate() const
{
	return BaseAbilitySet->GetStaminaRegenRate();
}

float ABaseCharacter::GetMoveSpeed()
{
	return BaseAbilitySet->GetMoveSpeed();
}

void ABaseCharacter::OnHealthChangedNative(const FOnAttributeChangeData& Data)
{
	OnHealthChanged(Data.OldValue, Data.NewValue);
}

void ABaseCharacter::OnStaminaChangedNative(const FOnAttributeChangeData& Data)
{
	OnStaminaChanged(Data.OldValue, Data.NewValue);
}

bool ABaseCharacter::IsAlive() const
{
	float Health; float MaxHealth;
	GetHealthValues(Health, MaxHealth);
	return Health > 0;
}

void ABaseCharacter::InitializeAbilityMulti(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToAcquire,
                                            int32 AbilityLevel)
{
	for (TSubclassOf<UGameplayAbility> AbilityToAcquire : AbilitiesToAcquire)
	{
		InitializeAbility(AbilityToAcquire, AbilityLevel);
	}
}

void ABaseCharacter::RemoveAbilityWithTag(FGameplayTagContainer TagContainer)
{
	TArray<FGameplayAbilitySpec*> MatchingAbilities;

	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, MatchingAbilities);

	for (FGameplayAbilitySpec* AbilitySpec : MatchingAbilities)
	{
		AbilitySystemComponent->ClearAbility(AbilitySpec->Handle);
	}
}

void ABaseCharacter::ChangeAbilityLevelWithTags(FGameplayTagContainer TagContainer, int NewLevel)
{
	TArray<FGameplayAbilitySpec*> MatchingAbilities;
	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, MatchingAbilities);

	for (FGameplayAbilitySpec* AbilitySpec : MatchingAbilities)
	{
		AbilitySpec->Level = NewLevel;
		// 将其标记为Dirty以同步到所属(Owning)客户端.
		AbilitySystemComponent->MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void ABaseCharacter::CancelAbilityWithTag(FGameplayTagContainer WithTags, FGameplayTagContainer WithoutTags)
{
	AbilitySystemComponent->CancelAbilities(&WithTags, &WithoutTags);
}

void ABaseCharacter::AddLooseGameplayTag(FGameplayTag TagToAdd)
{
	GetAbilitySystemComponent()->AddLooseGameplayTag(TagToAdd);
	GetAbilitySystemComponent()->SetLooseGameplayTagCount(TagToAdd,1);
}

void ABaseCharacter::RemoveLooseGameplayTag(FGameplayTag TagToRemove)
{
	GetAbilitySystemComponent()->RemoveLooseGameplayTag(TagToRemove);
}

void ABaseCharacter::ApplyGEToTargetData(const FGameplayEffectSpecHandle& GESpec,
	const FGameplayAbilityTargetDataHandle& TargetData)
{
	for (auto Data: TargetData.Data)
	{
		Data->ApplyGameplayEffectSpec(*GESpec.Data);
	}
}

void ABaseCharacter::SetHealthValues(float NewHealth, float NewMaxHealth)
{
	GetAbilitySystemComponent()->ApplyModToAttribute(BaseAbilitySet->GetHealthAttribute(), EGameplayModOp::Override, NewHealth);
	GetAbilitySystemComponent()->ApplyModToAttribute(BaseAbilitySet->GetMaxHealthAttribute(), EGameplayModOp::Override, NewMaxHealth);
}

void ABaseCharacter::SetStaminaValues(float NewStamina, float NewMaxStamina)
{
	GetAbilitySystemComponent()->ApplyModToAttribute(BaseAbilitySet->GetStaminaAttribute(), EGameplayModOp::Override, NewStamina);
	GetAbilitySystemComponent()->ApplyModToAttribute(BaseAbilitySet->GetMaxStaminaAttribute(), EGameplayModOp::Override, NewMaxStamina);
}

bool ABaseCharacter::ActivateAbility(FGameplayTag Tag, bool bAllowRemoteActivation)
{
	return GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer{Tag}, bAllowRemoteActivation);
}

bool ABaseCharacter::DeactivateAbility(FGameplayTag Tag)
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivatePtrs;
	GetAbilitySystemComponent()->GetActivatableGameplayAbilitySpecsByAllMatchingTags(FGameplayTagContainer(Tag), AbilitiesToActivatePtrs);
	if (AbilitiesToActivatePtrs.Num() < 1)
	{
		return false;
	}

	// Convert from pointers (which can be reallocated, since they point to internal data) to copies of that data
	TArray<FGameplayAbilitySpec> AbilitiesToActivate;
	AbilitiesToActivate.Reserve(AbilitiesToActivatePtrs.Num());
	Algo::Transform(AbilitiesToActivatePtrs, AbilitiesToActivate, [](FGameplayAbilitySpec* SpecPtr) { return *SpecPtr; });

	bool bSuccess = false;
	for (const FGameplayAbilitySpec& GameplayAbilitySpec : AbilitiesToActivate)
	{
		ensure(IsValid(GameplayAbilitySpec.Ability));
		if (GameplayAbilitySpec.IsActive())
		{
			bSuccess = true;
			GetAbilitySystemComponent()->CancelAbilityHandle(GameplayAbilitySpec.Handle);
		}
			
	}

	return bSuccess;
}

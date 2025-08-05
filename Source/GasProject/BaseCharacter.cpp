// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "BaseAttributeSet.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
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

void ABaseCharacter::GetStaminaValues(float& Stamina, float& MaxStamina) const
{
	Stamina = BaseAbilitySet->GetStamina();
	MaxStamina = BaseAbilitySet->GetMaxStamina();
}

void ABaseCharacter::OnHealthChangedNative(const FOnAttributeChangeData& Data)
{
	OnHealthChanged(Data.OldValue, Data.NewValue);
}

void ABaseCharacter::OnStaminaChangedNative(const FOnAttributeChangeData& Data)
{
	OnStaminaChanged(Data.OldValue, Data.NewValue);
}

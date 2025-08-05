// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class GASPROJECT_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UBaseAttributeSet();

	// Attributes for Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseAttribute)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(UBaseAttributeSet, Health)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseAttribute)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS_BASIC(UBaseAttributeSet, MaxHealth)


	// Attributes for Stamina
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseAttribute)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS_BASIC(UBaseAttributeSet, Stamina)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseAttribute)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS_BASIC(UBaseAttributeSet, MaxStamina)



	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
};

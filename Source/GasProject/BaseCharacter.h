// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "BaseCharacter.generated.h"


class UBaseAttributeSet;

UCLASS()
class GASPROJECT_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
private:
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Base Character")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UFUNCTION(BlueprintCallable)
	void InitializeAbility(TSubclassOf<UGameplayAbility> AbilityToGet, int32 AbilityLevel);

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Base Character")
	TObjectPtr<const UBaseAttributeSet> BaseAbilitySet;

	UFUNCTION(BlueprintPure, Category = "Base Character")
	void GetHealthValues(float& Health, float& MaxHealth) const;

	UFUNCTION(BlueprintCallable, Category = "Base Character")
	void GetStaminaValues(float& Stamina, float& MaxStamina) const;

	void OnHealthChangedNative(const FOnAttributeChangeData& Data);
	void OnStaminaChangedNative(const FOnAttributeChangeData& Data);


	UFUNCTION(BlueprintImplementableEvent, Category = "Base Character")
	void OnHealthChanged(float OldValue, float NewValue);
	UFUNCTION(BlueprintImplementableEvent, Category = "Base Character")
	void OnStaminaChanged(float OldValue, float NewValue);
};

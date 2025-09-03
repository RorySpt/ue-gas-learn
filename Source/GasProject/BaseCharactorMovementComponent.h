// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseCharactorMovementComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GASPROJECT_API UBaseCharactorMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBaseCharactorMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual float GetMaxSpeed() const override;

	UFUNCTION(BlueprintCallable, Category = "Sprinting")
	void StartSprinting();
	UFUNCTION(BlueprintCallable, Category = "Sprinting")
	void StopSprinting();

	virtual void SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode = 0) override;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sprinting")
	float SprintSpeedMultiplier;

	bool bRequestToStartSprinting = false;
};

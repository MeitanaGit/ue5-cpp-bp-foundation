// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyEnemy.generated.h"

UCLASS()
class REPORTWRITE_API AMyEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 敵のHP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyStats")
	float Health = 10.0f;

	// エンジン標準のダメージ受け取り関数をオーバーライド
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;	
	
	// 移動スピード（CharacterMovementコンポーネント経由でも調整可能）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyStats")
	float MoveSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyEffects")
	class UNiagaraSystem* DeathEffect; // Niagaraの場合

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	// class UParticleSystem* DeathEffectCascade; // Cascadeの場合

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyDrops")
	TSubclassOf<class AActor> ExpOrbClass; // 経験値オーブのクラス

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

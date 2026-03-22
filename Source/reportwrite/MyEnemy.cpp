// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemy.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

// Sets default values
AMyEnemy::AMyEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ここから追記箇所
	// 1. プレイヤー（Pawn）を取得
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
    
	if (PlayerPawn)
	{
		// 2. プレイヤーへの方向ベクトルを計算
		FVector Direction = (PlayerPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        
		// 3. 移動入力を与える（Z軸方向は無視）
		Direction.Z = 0.0f;
		AddMovementInput(Direction, 1.0f);
	}
}

// Called to bind functionality to input
void AMyEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AMyEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health -= ActualDamage;
//	UE_LOG(LogTemp, Warning, TEXT("Enemy HP: %f"), Health);

	if (Health <= 0.f)
	{

		// --- ここから撃破演出の追加 ---

		// 1. エフェクト（爆発など）をスポーンさせる
		if (DeathEffect) // BPで設定したエフェクト
		{
			// Niagaraの場合（推奨）
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DeathEffect, GetActorLocation(), GetActorRotation());
            
			// レガシーなCascadeパーティクルの場合
			// UGameplayStatics::SpawnEmitterAtLocation(this, DeathEffectCascade, GetActorLocation(), GetActorRotation());
		}

		// 2. 経験値アイテム（オーブ）をドロップさせる
		if (ExpOrbClass) // BPで設定したオーブのクラス
		{
			FActorSpawnParameters SpawnParams;
			GetWorld()->SpawnActor<AActor>(ExpOrbClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
		}		
		Destroy(); // HPが0になったら消滅
	}

	return ActualDamage;
}
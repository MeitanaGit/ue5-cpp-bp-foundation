// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemySpawner.h"

// Sets default values
AMyEnemySpawner::AMyEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; // スポナー自体はTick不要
}

// Called when the game starts or when spawned
void AMyEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	// 指定した間隔で SpawnEnemy を繰り返し実行するタイマーを開始
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AMyEnemySpawner::SpawnEnemy, SpawnInterval, true);
}

// Called every frame
void AMyEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyEnemySpawner::SpawnEnemy()
{
	if (!EnemyClass) return;

	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn) return;

	// 1. ランダムな角度を決定（0～360度）
	float RandomAngle = FMath::FRandRange(0.f, 360.f);
	FVector Offset = FVector(FMath::Cos(RandomAngle), FMath::Sin(RandomAngle), 0.f) * SpawnRadius;

	// 2. プレイヤーの位置にオフセットを加算
	FVector SpawnLocation = PlayerPawn->GetActorLocation() + Offset;
    
	// Z軸は地面の高さに合わせる（例：地面が0なら0、あるいはプレイヤーと同じ高さ）
	SpawnLocation.Z = PlayerPawn->GetActorLocation().Z;

	// 3. スポーン実行
	FActorSpawnParameters SpawnParams;
	GetWorld()->SpawnActor<AMyEnemy>(EnemyClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
}
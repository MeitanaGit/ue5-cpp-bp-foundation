// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyBullet.generated.h"

UCLASS()
class REPORTWRITE_API AMyBullet : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AMyBullet();
	
	// 当たり判定
	UPROPERTY(VisibleAnywhere, Category = "Bullet")
	class USphereComponent* CollisionComponent;

	// 移動制御（まっすぐ飛ばすコンポーネント）
	UPROPERTY(VisibleAnywhere, Category = "Bullet")
	class UProjectileMovementComponent* BulletMovement;

	// 見た目
	UPROPERTY(VisibleAnywhere, Category = "Bullet")
	class UStaticMeshComponent* BulletMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 衝突時に呼ばれる関数（この形式で宣言する必要があります）
	UFUNCTION()
//	void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	// 反射できる残り回数
	UPROPERTY(EditAnywhere, Category = "Bullet")
	int32 BounceCount = 0;	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

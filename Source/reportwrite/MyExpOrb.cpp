// Fill out your copyright notice in the Description page of Project Settings.


#include "MyExpOrb.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerCharacter.h"

// Sets default values
AMyExpOrb::AMyExpOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 衝突判定の設定
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetSphereRadius(50.0f);
	//　ここはもういらない
//	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	// メッシュの設定
	OrbMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OrbMesh"));
	OrbMesh->SetupAttachment(RootComponent);
	OrbMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // メッシュは当たり判定なし
	
}

// Called when the game starts or when spawned
void AMyExpOrb::BeginPlay()
{
	Super::BeginPlay();
	
	// 実行時のコリジョン設定をログに出して確認
//	UE_LOG(LogTemp, Warning, TEXT("Orb Collision Profile: %s"), *CollisionComponent->GetCollisionProfileName().ToString());
	
	// プレイヤーを取得しておく
	TargetPlayer = UGameplayStatics::GetPlayerPawn(this, 0);

	// オーバーラップイベントをバインド
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMyExpOrb::OnOrbOverlap);
}

// Called every frame
void AMyExpOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TargetPlayer)
	{
		// プレイヤーとの距離を計算
		float Distance = FVector::Dist(GetActorLocation(), TargetPlayer->GetActorLocation());

		// 磁石の範囲内に入ったら吸い寄せられる
		if (Distance <= MagnetRange)
		{
			FVector Direction = (TargetPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
            
			// プレイヤーに向かって移動（フレームレートに依存しないようDeltaTimeを掛ける）
			FVector NewLocation = GetActorLocation() + (Direction * MoveSpeed * DeltaTime);
			SetActorLocation(NewLocation);

			// 距離が非常に近くなったら加速させるなどの演出を入れるとよりヴァンサバっぽくなります
			MoveSpeed += 10.0f; 
		}
	}
}

void AMyExpOrb::OnOrbOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// プレイヤーに触れたかチェック
	if (OtherActor && OtherActor == TargetPlayer)
	{
		// TargetPlayerをAMyPlayerCharacter型にキャスト
		AMyPlayerCharacter* Player = Cast<AMyPlayerCharacter>(OtherActor);
        
		if (Player)
		{
			// 10ポイント加算してみる
			Player->AddScore(10);
            
			// ログ確認用
			UE_LOG(LogTemp, Log, TEXT("Exp Orb Picked Up!"));
		}

		Destroy();
	}
}
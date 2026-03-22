// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBullet.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyEnemy.h" // 敵クラスのヘッダーが必要
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMyBullet::AMyBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1. 当たり判定の設定
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(15.0f);
	RootComponent = CollisionComponent;
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	
	
	// 2. メッシュコンポーネントを生成して CollisionComponent に取り付ける
	// ★ここが抜けている可能性があります！
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetupAttachment(RootComponent);
	
	// 3. 移動の設定（初速や重力の影響）
	BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletMovement"));
	BulletMovement->UpdatedComponent = CollisionComponent;
	BulletMovement->InitialSpeed = 3000.f;  // 弾速
	BulletMovement->MaxSpeed = 3000.f;
	BulletMovement->bRotationFollowsVelocity = true;
	BulletMovement->ProjectileGravityScale = 0.f; // 重力を無視（見下ろしなので）

	// 衝突プロファイルを Block が発生するものに変更
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
    
	// シミュレーション中に Hit イベントを生成するように設定
	CollisionComponent->SetNotifyRigidBodyCollision(true); 

	// ... ProjectileMovementの設定 ...
	BulletMovement->bShouldBounce = true;
	BulletMovement->Bounciness = 1.0f;
	BulletMovement->Friction = 0.0f;
    
	// 反射時に速度が落ちないようにする設定
	BulletMovement->BounceVelocityStopSimulatingThreshold = 0.f;

	// デフォルトの反射回数を0（通常弾）にしておきます
	BounceCount = 0;

	// 4. 数秒後に自動消滅
	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AMyBullet::BeginPlay()
{
	Super::BeginPlay();
	
	// CollisionComponent（Sphere）を取得してイベントをバインド
	// ※C++で作成した衝突用コンポーネント変数名に合わせてください
	if (CollisionComponent)
	{
		// Hitイベントにバインド
		CollisionComponent->OnComponentHit.AddDynamic(this, &AMyBullet::OnBulletHit);
	}
}

// Called every frame
void AMyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyBullet::OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 自分自身や無効なアクター、敵以外には反応しないようにする
	if (OtherActor && (OtherActor != this) && OtherActor->IsA(AMyEnemy::StaticClass()))
	{
		// 敵にダメージを与える
		UGameplayStatics::ApplyDamage(OtherActor, 1.0f, GetInstigatorController(), this, UDamageType::StaticClass());
		// --- 反射処理 ---
		if (BounceCount > 0)
		{
			BounceCount--;
			// Destroyせずに、コンポーネントが勝手に物理反射してくれるのを待つ
			// もし「近くの敵へ自動追尾」させたい場合は、ここで速度ベクトルを書き換えます
			UE_LOG(LogTemp, Log, TEXT("Bounce! Remaining: %d"), BounceCount);
			// 重要：ここで跳ね返り方向を調整したい場合は速度を上書きできますが、
			// 基本的には ProjectileMovement が Normal（壁の面）を見て自動で跳ね返してくれます。
		}
		else
		{
			Destroy();
		}
	}
}
// Copyright Josh "Asheron" Deal 2018

#include "TestingGrounds.h"
#include "DrawDebugHelpers.h"
#include "Tile.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

	void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale) {

		int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);

		for (size_t i = 0; i < NumberToSpawn; i++) {
			FVector SpawnPoint;
			float RandomScale = FMath::RandRange(MinScale, MaxScale);
			bool Found = FindEmptyLocation(SpawnPoint, Radius * RandomScale);
			if (Found) {
				float RandomRotation = FMath::RandRange(-180.0f, 180.0f);
				PlaceActor(ToSpawn, SpawnPoint, RandomRotation, RandomScale);
			}
			//UE_LOG(LogTemp, Warning, TEXT("SpawnPoint: %s"), *SpawnPoint.ToCompactString());
		}
	}

	bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius) {

		FVector Min(0, -2000, 0);
		FVector Max(4000, 2000, 0);
		FBox Bounds(Min, Max);
		const int MAX_ATTEMPTS = 50;

		for (size_t i = 0; i < MAX_ATTEMPTS; i++) {

			FVector CandidatePoint = FMath::RandPointInBox(Bounds);
			if (CanSpawnAtLocation(CandidatePoint, Radius)) {
				OutLocation = CandidatePoint;
				return true;
			}
		}
		return false;
	}

	void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint, float Rotation, float Scale) {

		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ToSpawn);
		SpawnedActor->SetActorRelativeLocation(SpawnPoint);
		SpawnedActor->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		SpawnedActor->SetActorRotation(FRotator(0, Rotation, 0));
		SpawnedActor->SetActorScale3D(FVector(Scale));
	}

	bool ATile::CanSpawnAtLocation(FVector Location, float Radius) {

		FHitResult HitResult;
		FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
		bool HasHit = GetWorld()->SweepSingleByChannel(
			HitResult,
			GlobalLocation,
			GlobalLocation,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel2,
			FCollisionShape::MakeSphere(Radius)
		);
		//FColor ResultColor = HasHit ? FColor::Red : FColor::Green;
		//DrawDebugCapsule(GetWorld(), GlobalLocation, 0, Radius, FQuat::Identity, ResultColor, true, 100);
		return !HasHit;
	}

	// TODO add stairs
	// void ATile::PlaceStairs(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn) {
	//	FVector Min(0, -2000, 0);
	//	FVector Max(4000, 2000, 0);
	//	FBox Bounds(Min, Max);
	//	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);

	//	for (size_t i = 0; i < NumberToSpawn; i++) {
	//		FVector SpawnPoint = FMath::RandPointInBox(Bounds);
	//		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ToSpawn);
	//		SpawnedActor->SetActorRelativeLocation(SpawnPoint);
	//		SpawnedActor->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//		//UE_LOG(LogTemp, Warning, TEXT("SpawnPoint: %s"), *SpawnPoint.ToCompactString());
	//	}
	//}
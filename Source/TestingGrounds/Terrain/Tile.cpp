// Copyright Josh "Asheron" Deal 2018

#include "TestingGrounds.h"
#include "DrawDebugHelpers.h"
#include "ActorPool.h"
#include "Tile.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NavigationBoundsOffset = FVector(2000, 0, 0);

	MinExtent = FVector(0, -2000, 0);
	MaxExtent = FVector(4000, 2000, 0);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::EndPlay(const EEndPlayReason::Type EEndPlayReason) {

	Pool->Return(NavMeshBoundsVolume);
}

// Called every frame
void ATile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

template<class T>
void ATile::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale) {

	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);

	for (size_t i = 0; i < NumberToSpawn; i++) {
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
		bool Found = FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale);
		if (Found) {
			SpawnPosition.Rotation = FMath::RandRange(-180.0f, 180.0f);
			PlaceActor(ToSpawn, SpawnPosition);
		}
		//UE_LOG(LogTemp, Warning, TEXT("SpawnPoint: %s"), *SpawnPoint.ToCompactString());
	}
}

// Places the Objects in the Level
void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale) {

	RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);
}

// Places the AI in the Level
void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn, int MaxSpawn, float Radius) {

	RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, 1, 1);
}

void ATile::SetPool(UActorPool *InPool) {

	UE_LOG(LogTemp, Warning, TEXT("[%s] Setting Pool %s"), *(this->GetName()), *(InPool->GetName()));
	Pool = InPool;

	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume() {

	NavMeshBoundsVolume = Pool->CheckOut();
	if (NavMeshBoundsVolume == nullptr) {

		UE_LOG(LogTemp, Error, TEXT("[%s] Not Enough Actors in pool."), *GetName());
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("[%s] Checked out: {%s}"), *GetName(), *NavMeshBoundsVolume->GetName());
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
	GetWorld()->GetNavigationSystem()->Build();
}

bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius) {

	FBox Bounds(MinExtent, MaxExtent);
	const int MAX_ATTEMPTS = 0;

	for (size_t i = 0; i < MAX_ATTEMPTS; i++) {

		FVector CandidatePoint = FMath::RandPointInBox(Bounds);
		if (CanSpawnAtLocation(CandidatePoint, Radius)) {
			OutLocation = CandidatePoint;
			return true;
		}
	}
	return false;
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition& SpawnPosition) {

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ToSpawn);
	if (SpawnedActor) {
		SpawnedActor->SetActorRelativeLocation(SpawnPosition.Location);
		SpawnedActor->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		SpawnedActor->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
		SpawnedActor->SetActorScale3D(FVector(SpawnPosition.Scale));
	}
}

void ATile::PlaceActor(TSubclassOf<APawn> ToSpawn, const FSpawnPosition& SpawnPosition) {

	APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(ToSpawn);
	if (SpawnedPawn) {
		SpawnedPawn->SetActorRelativeLocation(SpawnPosition.Location);
		SpawnedPawn->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		SpawnedPawn->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
		SpawnedPawn->SpawnDefaultController();
		SpawnedPawn->Tags.Add(FName("Enemy"));
	}
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
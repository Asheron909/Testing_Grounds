// Copyright Josh "Asheron" Deal 2018

#pragma once

#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "Setup")
		void PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius = 500, float MinScale = 1, float MaxScale = 1);

	/*UFUNCTION(BlueprintCallable, Category = "Setup")
	void PlaceStairs(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn);	*/

private:
	bool FindEmptyLocation(FVector& OutLocation, float Radius);

	void PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint, float Rotation, float Scale);

	bool CanSpawnAtLocation(FVector Location, float Radius);
};

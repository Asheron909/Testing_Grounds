// Copyright Josh "Asheron" Deal 2018

#include "TestingGrounds.h"
#include "AI/Navigation/NavMeshBoundsVolume.h"
#include "EngineUtils.h"
#include "ActorPool.h"
#include "InfiniteTerrainGameMode.h"


AInfiniteTerrainGameMode::AInfiniteTerrainGameMode() {

	NavMeshBoundsVolumePool = CreateDefaultSubobject<UActorPool>(FName("Nav Mesh Bounds Volume Pool"));
}

void AInfiniteTerrainGameMode::PopulateBoundsVolumePool() {

	auto VolumeIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());
	while (VolumeIterator) {
		AddToPool(*VolumeIterator);
		++VolumeIterator;
	}
}

void AInfiniteTerrainGameMode::AddToPool(ANavMeshBoundsVolume *VolumeToAdd) {

	NavMeshBoundsVolumePool->Add(VolumeToAdd);
}

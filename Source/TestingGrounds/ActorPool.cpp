// Copyright Josh "Asheron" Deal 2018

#include "TestingGrounds.h"
#include "ActorPool.h"


// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActorPool::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UActorPool::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

AActor * UActorPool::CheckOut() {

	if (Pool.Num() == 0) { return nullptr;  }

	return Pool.Pop();
}

void UActorPool::Return(AActor * ActorToReturn) {

	Add(ActorToReturn);
}

void UActorPool::Add(AActor * ActorToAdd) {

	if (ActorToAdd == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("[%s] added null actor."), *GetName());
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("[%s] Actor added: {%s}."), *GetName(), *ActorToAdd->GetName());

	Pool.Push(ActorToAdd);
}
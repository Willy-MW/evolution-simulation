// © 2020 Marcin Witek 


#include "BTTask_GetFurthestLocation.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Math/UnitConversion.h"

UBTTask_GetFurthestLocation::UBTTask_GetFurthestLocation()
{
	NodeName = "Get Location Furthest From";

	AnimalSightRadiusKey.AddFloatFilter(this,GET_MEMBER_NAME_CHECKED(UBTTask_GetFurthestLocation, AnimalSightRadiusKey));
	EnemyObjectKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetFurthestLocation, EnemyObjectKey), AActor::StaticClass());
	ChosenLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetFurthestLocation, ChosenLocationKey));
}

void UBTTask_GetFurthestLocation::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();

	if(ensure(BBAsset))
	{
		AnimalSightRadiusKey.ResolveSelectedKey(*BBAsset);
		EnemyObjectKey.ResolveSelectedKey(*BBAsset);
		ChosenLocationKey.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type UBTTask_GetFurthestLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	
	if(!MyController || !MyBlackboard)
	{
		return EBTNodeResult::Failed;
	}

	UObject* EnemyObject = MyBlackboard->GetValueAsObject(EnemyObjectKey.SelectedKeyName);
	AActor* EnemyActor = Cast<AActor>(EnemyObject);

	if(!EnemyActor)
	{
		return EBTNodeResult::Failed;
	}

	EnemyLocation = EnemyActor->GetActorLocation();
	SightRadius = MyBlackboard->GetValueAsFloat(AnimalSightRadiusKey.SelectedKeyName);

	const TArray<FVector> PlausibleLocations = GetValidLocationsAroundPawn(8, MyController->GetPawn());

	const FVector ChosenLocation = GetFurthestLocation(PlausibleLocations);

	MyBlackboard->SetValueAsVector(ChosenLocationKey.SelectedKeyName, ChosenLocation);

	return  EBTNodeResult::Succeeded;
}

FString UBTTask_GetFurthestLocation::GetStaticDescription() const
{
	FString FirstKeyDesc, SecondKeyDesc, ThirdKeyDesc;
	FirstKeyDesc = SecondKeyDesc = ThirdKeyDesc = "invalid";

	if (EnemyObjectKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		FirstKeyDesc = EnemyObjectKey.SelectedKeyName.ToString();
	}
	
	if (AnimalSightRadiusKey.SelectedKeyType == UBlackboardKeyType_Float::StaticClass())
	{
		SecondKeyDesc = AnimalSightRadiusKey.SelectedKeyName.ToString();
	}

	if (ChosenLocationKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		ThirdKeyDesc = ChosenLocationKey.SelectedKeyName.ToString();
	}

	return FString::Printf(TEXT("Furthest From: %s\nWithin Radius: %s\nChosen Location: %s"), *FirstKeyDesc, *SecondKeyDesc, *ThirdKeyDesc);
}

TArray<FVector> UBTTask_GetFurthestLocation::GetValidLocationsAroundPawn(const int Number, APawn* Pawn)
{
	const FVector PawnForwardVector = Pawn->GetActorForwardVector();
	const FVector PawnLocation = Pawn->GetActorLocation();

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	TArray<FVector> Locations;
	
	const float Angle = 360.f / Number;
	
	for(int i = 0; i < Number; i++)
	{
		FRotator Rotation(0.f, Angle * i, 0.f);
		FVector Offset = Rotation.RotateVector(PawnForwardVector * SightRadius);
		FNavLocation Nav;

		if(NavSys->GetRandomPointInNavigableRadius(PawnLocation + Offset, 5.f, Nav))
		{
			Locations.Add(Nav.Location);
			//DrawDebugPoint(GetWorld(), Nav.Location, 10.f, FColor::Yellow, false, 0.2f);
		}
	}

	return Locations;
}

FVector UBTTask_GetFurthestLocation::GetFurthestLocation(const TArray<FVector>& Locations)
{
	if(Locations.Num() <= 0)
	{
		return FVector::ZeroVector;
	}
	
	FVector FurthestLocation = Locations[0];
	float FurthestDistance = FVector::Dist(FurthestLocation, EnemyLocation);

	for(FVector Location : Locations)
	{
		const float Distance = FVector::Dist(Location, EnemyLocation);
		if(Distance > FurthestDistance)
		{
			FurthestDistance = Distance;
			FurthestLocation = Location;
		}
	}

	//DrawDebugPoint(GetWorld(), FurthestLocation, 10.f, FColor::Red, false, 0.2f);

	return FurthestLocation;
}

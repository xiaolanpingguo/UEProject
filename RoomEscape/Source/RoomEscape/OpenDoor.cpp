// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	doorDuration_ = 1.f;
	lastOpenDoorTime_ = 0.f;

	owner_ = GetOwner();
	defaultPawn_ = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 角色进入了触发器
	if (triggerVolume_ && GetTotalMassTrigger() >= 25)
	{
		OpenDoor();
		//lastOpenDoorTime_ = GetWorld()->GetTimeSeconds();
	}
	else
	{
		CloseDoor();
	}
	
	// 最后修改，也不用延迟开关门操作了
	//if (GetWorld()->GetTimeSeconds() - lastOpenDoorTime_ > doorDuration_)
	//{
	//	CloseDoor();
	//}
}

void UOpenDoor::OpenDoor()
{
	// 这里我们直接在蓝图进行开关门操作
	// pitch:y yaw:z roll: x
	//FRotator rotator = FRotator(0, 00, 0);
	//owner_->SetActorRotation(rotator);
	
	// 触发事件,如果蓝图监听了过后，会回调到你的蓝图
	OnOpenDoor.Broadcast();
}

void UOpenDoor::CloseDoor()
{
	// 这里我们直接在蓝图进行开关门操作
	//FRotator rotator = FRotator(0, -90, 0);
	//owner_->SetActorRotation(rotator);

	// 触发事件,如果蓝图监听了过后，会回调到你的蓝图
	OnCloseDoor.Broadcast();
}

float UOpenDoor::GetTotalMassTrigger()
{
	float TotalMass = 0.f;

	if (triggerVolume_ == nullptr)
	{
		return TotalMass;
	}

	TArray<AActor*> Actors;
	triggerVolume_->GetOverlappingActors(Actors);

	for (const auto& Actor : Actors)
	{
		// 获取组件->获取质量
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	UE_LOG(LogTemp, Warning, TEXT("Total Mass:%f."), TotalMass);
	return TotalMass;
}


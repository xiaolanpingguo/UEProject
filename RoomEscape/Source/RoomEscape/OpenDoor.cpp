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

	// ��ɫ�����˴�����
	if (triggerVolume_ && GetTotalMassTrigger() >= 25)
	{
		OpenDoor();
		//lastOpenDoorTime_ = GetWorld()->GetTimeSeconds();
	}
	else
	{
		CloseDoor();
	}
	
	// ����޸ģ�Ҳ�����ӳٿ����Ų�����
	//if (GetWorld()->GetTimeSeconds() - lastOpenDoorTime_ > doorDuration_)
	//{
	//	CloseDoor();
	//}
}

void UOpenDoor::OpenDoor()
{
	// ��������ֱ������ͼ���п����Ų���
	// pitch:y yaw:z roll: x
	//FRotator rotator = FRotator(0, 00, 0);
	//owner_->SetActorRotation(rotator);
	
	// �����¼�,�����ͼ�����˹��󣬻�ص��������ͼ
	OnOpenDoor.Broadcast();
}

void UOpenDoor::CloseDoor()
{
	// ��������ֱ������ͼ���п����Ų���
	//FRotator rotator = FRotator(0, -90, 0);
	//owner_->SetActorRotation(rotator);

	// �����¼�,�����ͼ�����˹��󣬻�ص��������ͼ
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
		// ��ȡ���->��ȡ����
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	UE_LOG(LogTemp, Warning, TEXT("Total Mass:%f."), TotalMass);
	return TotalMass;
}


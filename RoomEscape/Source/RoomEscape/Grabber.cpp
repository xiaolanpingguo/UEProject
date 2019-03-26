// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// 获取组件
	inputComponent_ = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent_)
	{
		UE_LOG(LogTemp, Warning, TEXT("InputComponent is Founded."));
		inputComponent_->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent_->BindAction("Grab", IE_Released, this, &UGrabber::ReleaseGrab);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InputComponent Not Founded!"));
	}

	physicsComponent_ = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (physicsComponent_)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPhysicsHandleComponent is Founded."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UPhysicsHandleComponent Not Founded!"));
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 抓住了物品
	if (physicsComponent_ && physicsComponent_->GrabbedComponent)
	{
		physicsComponent_->SetTargetLocation(GetLineEnd());
	}
}

FVector UGrabber::GetLineStart()
{
	// 获取视野的起点
	FVector start;
	FRotator view_rotator;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(start, view_rotator);
	return start;
}

FVector UGrabber::GetLineEnd()
{
	// 根据自己旋转来确定向量终点
	FVector start;
	FRotator view_rotator;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(start, view_rotator);
	return start + view_rotator.Vector() * 100;
}

FHitResult UGrabber::LineTrace()
{
	// 1米之类的射线，依次来检测碰撞
	FHitResult hit;
	GetWorld()->LineTraceSingleByObjectType(
		hit, 		// 结果
		GetLineStart(),	// 开始位置
		GetLineEnd(),	// 结束位置
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), // 有物理效果的物体
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner())
	);

	AActor* hit_actor = hit.GetActor();
	if (hit_actor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line Hit: %s"), *hit_actor->GetName());
	}

	return hit;
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("grab is press!"));
	FHitResult hit = LineTrace();
	UPrimitiveComponent* hit_component = hit.GetComponent();
	if (hit.GetActor() && physicsComponent_)
	{
		UE_LOG(LogTemp, Warning, TEXT("grab component_!"));

		// 抓去到物体后，被抓取的物体要和抓的实体保持旋转一致
		physicsComponent_->GrabComponentAtLocationWithRotation(
			hit_component, 
			NAME_None,
			hit_component->GetOwner()->GetActorLocation(), 
			GetOwner()->GetActorRotation());
	}
}

void UGrabber::ReleaseGrab()
{
	UE_LOG(LogTemp, Warning, TEXT("grab is release!"));
	if (physicsComponent_)
	{
		physicsComponent_->ReleaseComponent();
	}
}


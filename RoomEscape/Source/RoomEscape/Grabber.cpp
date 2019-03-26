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

	// ��ȡ���
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

	// ץס����Ʒ
	if (physicsComponent_ && physicsComponent_->GrabbedComponent)
	{
		physicsComponent_->SetTargetLocation(GetLineEnd());
	}
}

FVector UGrabber::GetLineStart()
{
	// ��ȡ��Ұ�����
	FVector start;
	FRotator view_rotator;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(start, view_rotator);
	return start;
}

FVector UGrabber::GetLineEnd()
{
	// �����Լ���ת��ȷ�������յ�
	FVector start;
	FRotator view_rotator;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(start, view_rotator);
	return start + view_rotator.Vector() * 100;
}

FHitResult UGrabber::LineTrace()
{
	// 1��֮������ߣ������������ײ
	FHitResult hit;
	GetWorld()->LineTraceSingleByObjectType(
		hit, 		// ���
		GetLineStart(),	// ��ʼλ��
		GetLineEnd(),	// ����λ��
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), // ������Ч��������
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

		// ץȥ������󣬱�ץȡ������Ҫ��ץ��ʵ�屣����תһ��
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


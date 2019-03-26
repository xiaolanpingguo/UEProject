// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOMESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	void OpenDoor();
	void CloseDoor();

	// 开门事件，在上面声明过后，然后在蓝图中编辑进行触发
	// 声明蓝图是可以编辑的
	UPROPERTY(BlueprintAssignable)
	FDoorEvent OnOpenDoor;
	UPROPERTY(BlueprintAssignable)
	FDoorEvent OnCloseDoor;

	// 得到触发区域内总质量，这样就会触发，然后开门
	float GetTotalMassTrigger();

private:	

	AActor* owner_;

	// 玩家控制对象
	AActor* defaultPawn_;

	// 这个定义出来，需要去编辑器里面编辑
	UPROPERTY(EditAnywhere)
	ATriggerVolume* triggerVolume_;

	float doorDuration_;
	float lastOpenDoorTime_;
};

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

	// �����¼�����������������Ȼ������ͼ�б༭���д���
	// ������ͼ�ǿ��Ա༭��
	UPROPERTY(BlueprintAssignable)
	FDoorEvent OnOpenDoor;
	UPROPERTY(BlueprintAssignable)
	FDoorEvent OnCloseDoor;

	// �õ������������������������ͻᴥ����Ȼ����
	float GetTotalMassTrigger();

private:	

	AActor* owner_;

	// ��ҿ��ƶ���
	AActor* defaultPawn_;

	// ��������������Ҫȥ�༭������༭
	UPROPERTY(EditAnywhere)
	ATriggerVolume* triggerVolume_;

	float doorDuration_;
	float lastOpenDoorTime_;
};

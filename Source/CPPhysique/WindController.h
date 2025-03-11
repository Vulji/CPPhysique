#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WindController.generated.h"

UCLASS()
class CPPHYSIQUE_API AWindController : public AActor
{
	GENERATED_BODY()

public:
	AWindController();

protected:
	virtual void BeginPlay() override;

public:    
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* WindArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wind", meta = (AllowPrivateAccess = "true"))
	float WindForce = 50000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wind", meta = (AllowPrivateAccess = "true"))
	FVector WindDirection = FVector(0, 0, 1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* IncreaseWindAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* DecreaseWindAction;


	void ApplyWind();
	void IncreaseWindStrength();
	void DecreaseWindStrength();
	void ChangeWindDirection(FVector NewDirection);
};
// Shoot Them Up Game, All Rights Reserved.

#include "Player/STUPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

ASTUPlayerCharacter::ASTUPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    PrimaryActorTick.bCanEverTick = true;

    FPCCollisionComponentSphereRadius = 10.f;

    TPCCollisionComponentSphereRadius = 10.f;
    
    FPCSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("FirstPersenCameraSpringArmComonent"));
    FPCSpringArmComponent->SetupAttachment(GetRootComponent());
    FPCSpringArmComponent->SocketOffset = FVector(0.f, 50.f, 40.f);
    FPCSpringArmComponent->bUsePawnControlRotation = true;
    
    FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera")); 
    FirstPersonCamera->SetupAttachment(FPCSpringArmComponent);
    
    FPCCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("FirstPersenCameraCollisionComponent"));
    FPCCollisionComponent->SetupAttachment(FirstPersonCamera);
    FPCCollisionComponent->SetSphereRadius(FPCCollisionComponentSphereRadius);
    FPCCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    FPCCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    
    TPCSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("ThirdPersenCameraSpringArmComonent"));    
    TPCSpringArmComponent->SetupAttachment(GetRootComponent());       
    TPCSpringArmComponent->bUsePawnControlRotation = true;    
    TPCSpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);
    
    ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Third Person Camera"));
    ThirdPersonCamera->SetupAttachment(TPCSpringArmComponent);
    
    TPCCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ThirdPersenCameraCollisionComponent"));
    TPCCollisionComponent->SetupAttachment(ThirdPersonCamera);
    TPCCollisionComponent->SetSphereRadius(TPCCollisionComponentSphereRadius);
    TPCCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    TPCCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    
    ActiveViewCameraMode = UPlayerViewCameraMode::E_ThirdPerson;
}

void ASTUPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(FPCCollisionComponent);
    check(TPCCollisionComponent);
    
    SetViewCameraMode(ActiveViewCameraMode);

    FPCCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionBeginOverlap);
    FPCCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionEndOverlap);

    TPCCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionBeginOverlap);
    TPCCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionEndOverlap);
}

void ASTUPlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    CheckCameraOverlap();
}

void ASTUPlayerCharacter::OnCameraCollisionEndOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    CheckCameraOverlap();
}

USphereComponent* ASTUPlayerCharacter::GetActiveCCollisionComponent() const
{
    if(FirstPersonCamera->IsActive())
    {
        return  FPCCollisionComponent;
    }
    else 
    {
        return TPCCollisionComponent;
    }
}

void ASTUPlayerCharacter::SetViewCameraMode(UPlayerViewCameraMode NewViewCameraMode)
{
    ActiveViewCameraMode = NewViewCameraMode;

    switch (NewViewCameraMode)
    {
        case  UPlayerViewCameraMode::E_ThirdPerson:
              ThirdPersonCamera->SetActive(true);
              UpdateCameraComponents(ThirdPersonCamera);
        
              FirstPersonCamera->SetActive(false);
              UpdateCameraComponents(FirstPersonCamera);
                      
              break;

        case  UPlayerViewCameraMode::E_FirstPerson:
              FirstPersonCamera->SetActive(true);
              UpdateCameraComponents(FirstPersonCamera);
        
              ThirdPersonCamera->SetActive(false);
              UpdateCameraComponents(ThirdPersonCamera);
        
              break;        
    }
}

void ASTUPlayerCharacter::UpdateCameraComponents(UCameraComponent* InCamera)
{
    const bool bIsActive = InCamera->IsActive();

    TArray<USceneComponent*> CameraChildrenComponents;        
    InCamera->GetChildrenComponents(true, CameraChildrenComponents);
    for (USceneComponent* CurrentComponent: CameraChildrenComponents)
    {
        CurrentComponent->SetActive(bIsActive);
    }
}

void ASTUPlayerCharacter::CheckCameraOverlap()
{
    const auto HideMesh = GetActiveCCollisionComponent()->IsOverlappingComponent(GetCapsuleComponent());
    GetMesh()->SetOwnerNoSee(HideMesh);

    TArray<USceneComponent*> MeshChildren;
    GetMesh()->GetChildrenComponents(true, MeshChildren);

    for (auto MeshChild : MeshChildren)
    {
        if (const auto MeshChildGeometry = Cast<UPrimitiveComponent>(MeshChild))
        {
            MeshChildGeometry->SetOwnerNoSee(HideMesh);
        }
    }
}

void ASTUPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);
    check(WeaponComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ASTUPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ASTUPlayerCharacter::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ASTUPlayerCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis(TEXT("TurnAround"), this, &ASTUPlayerCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ASTUPlayerCharacter::Jump);
    PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &ASTUPlayerCharacter::OnStartRunning);
    PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &ASTUPlayerCharacter::OnStopRunning);
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ASTUPlayerCharacter::OnStartFire);
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire);
    PlayerInputComponent->BindAction(TEXT("NextWeapon"), IE_Pressed, WeaponComponent, &USTUWeaponComponent::NextWeapon);
    PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, WeaponComponent, &USTUWeaponComponent::Reload);
    PlayerInputComponent->BindAction(TEXT("ChangeViewCameraMode"), IE_Pressed, this, &ASTUPlayerCharacter::ChangeViewCameraMode);
    
    DECLARE_DELEGATE_OneParam(FZoomInputSignature, bool);
    PlayerInputComponent->BindAction<FZoomInputSignature>("Zoom", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Zoom, true);
    PlayerInputComponent->BindAction<FZoomInputSignature>("Zoom", IE_Released, WeaponComponent, &USTUWeaponComponent::Zoom, false);
}

void ASTUPlayerCharacter::MoveForward(float Amount)
{
    IsMovingForward = Amount > 0.0f;
    if (Amount == 0.0f) return;
    AddMovementInput(GetActorForwardVector(), Amount);

    if (IsRunning() && WeaponComponent->IsFiring())
    {
        WeaponComponent->StopFire();
    }
}

void ASTUPlayerCharacter::MoveRight(float Amount)
{
    if (Amount == 0.0f) return;
    AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUPlayerCharacter::ChangeViewCameraMode()
{
    if(ActiveViewCameraMode == UPlayerViewCameraMode::E_FirstPerson)
    {
        SetViewCameraMode(UPlayerViewCameraMode::E_ThirdPerson);
    }
    else
    {
        SetViewCameraMode(UPlayerViewCameraMode::E_FirstPerson);
    }
}

void ASTUPlayerCharacter::OnStartRunning()
{
    WantsToRun = true;
    if (IsRunning())
    {
        WeaponComponent->StopFire();
    }
}

void ASTUPlayerCharacter::OnStopRunning()
{
    WantsToRun = false;
}

bool ASTUPlayerCharacter::IsRunning() const
{
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

void ASTUPlayerCharacter::OnDeath()
{
    Super::OnDeath();
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}

void ASTUPlayerCharacter::OnStartFire()
{
    if (IsRunning()) return;
    WeaponComponent->StartFire();
}

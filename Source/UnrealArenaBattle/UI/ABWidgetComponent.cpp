// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABWidgetComponent.h"
#include "ABUserWidget.h"

void UABWidgetComponent::InitWidget()
{
	// 위젯에 대한 인스턴스가 생성된 직후 호출되는 함수
	// 위젯에 대한 UI 요소들이 준비 완료 되면
	// NativeConstruct 함수가 호출됨. UABHpBarWidget::NativeConstruct()
	// 위젯에 대한 인스턴스가 생성되었을 때, 이 때 관련된 액터 정보를 얻어서 OwningActor 에 넣겠다.

	Super::InitWidget();

	UABUserWidget* ABUserWidget = Cast<UABUserWidget>(GetWidget());
	// 현재 컴포넌트가 소유하고 있는 위젯을 가져옴.
	// 위에 Super::InitWidget 함수 들어가보면, CreateWidget 을 하고 있음. 이 때 인스턴스가 만들어지게 되는 것.
	// Super::InitWidget(); 다음 줄에 Cast로 위젯을 가져오기 때문에 인스턴스를 가져올 수 있음.

	if (ABUserWidget)
	{
		ABUserWidget->SetOwningActor(GetOwner());
	}
}

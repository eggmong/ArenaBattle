// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABWidgetComponent.h"
#include "ABUserWidget.h"

void UABWidgetComponent::InitWidget()
{
	// ������ ���� �ν��Ͻ��� ������ ���� ȣ��Ǵ� �Լ�
	// ������ ���� UI ��ҵ��� �غ� �Ϸ� �Ǹ�
	// NativeConstruct �Լ��� ȣ���. UABHpBarWidget::NativeConstruct()
	// ������ ���� �ν��Ͻ��� �����Ǿ��� ��, �� �� ���õ� ���� ������ �� OwningActor �� �ְڴ�.

	Super::InitWidget();

	UABUserWidget* ABUserWidget = Cast<UABUserWidget>(GetWidget());
	// ���� ������Ʈ�� �����ϰ� �ִ� ������ ������.
	// ���� Super::InitWidget �Լ� ������, CreateWidget �� �ϰ� ����. �� �� �ν��Ͻ��� ��������� �Ǵ� ��.
	// Super::InitWidget(); ���� �ٿ� Cast�� ������ �������� ������ �ν��Ͻ��� ������ �� ����.

	if (ABUserWidget)
	{
		ABUserWidget->SetOwningActor(GetOwner());
	}
}

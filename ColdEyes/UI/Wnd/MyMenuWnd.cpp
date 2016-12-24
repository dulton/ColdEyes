#include "stdafx.h"
#include "MyMenuWnd.h"
#include "conio.h"
#include "Device\Port.h"


CMyMenuWnd::CMyMenuWnd()
{
}

CMyMenuWnd::~CMyMenuWnd()
{
}


LPCTSTR CMyMenuWnd::GetWindowClassName() const
{
	return _T("MenuWnd");
}

CDuiString CMyMenuWnd::GetSkinFile()
{
	return _T("MenuUI.xml");
}

void CMyMenuWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_KILLFOCUS) {
		FocusedItemClassName = msg.pSender->GetClass();
		pKillFocsedItem = msg.pSender;

	}

	if (msg.sType == DUI_MSGTYPE_SETFOCUS) {
		if (_tcscmp(msg.pSender->GetUserData(), _T("PortConfig")) == 0) {
			FillCameraInfo(msg.pSender);
		}

		if (FocusedItemClassName != msg.pSender->GetClass()) {
			//焦点在一级模块
			if (_tcsicmp(msg.pSender->GetClass(), _T("MenuItemUI")) == 0) {
				if (pKillFocsedItem) {
					((CMenuItemUI*)pKillFocsedItem)->SetStatus(0);
				}
				((CMenuItemUI*)msg.pSender)->SetStatus(0);
				UpdataBkColor(0,LAYOUT_MENUITEM_FOCUSED, LAYOUT_SUB_NOFOCUS);
				ShowBodyLayout(false);
			}
			//焦点在二级模块
			else if (_tcsicmp(msg.pSender->GetClass(), _T("SubMenuItemUI")) == 0) {
				if (_tcsicmp(FocusedItemClassName, _T("MenuItemUI")) == 0) {
					((CMenuItemUI*)pKillFocsedItem)->SetStatus(1);
					UpdataBkColor(0,LAYOUT_MENUITEM_NOFOCUS, LAYOUT_SUB_FOCUSED);
					ShowBodyLayout(true);
				}
				else {
					UpdataBkColor(1, LAYOUT_BODY_NOFOUCS, LAYOUT_SUB_FOCUSED);
					((CSubMenuItemUI*)msg.pSender)->SetStatus(0);
				}
			}
			//焦点在三级模块
			else {
				if (_tcsicmp(FocusedItemClassName, _T("SubMenuItemUI")) == 0) {
					((CMenuItemUI*)pKillFocsedItem)->SetStatus(1);
					UpdataBkColor(1, LAYOUT_BODY_FOCUSED, LAYOUT_SUB_SEL_NOFOCUS);
				}
			}
		}
	}
}


LRESULT CMyMenuWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

void CMyMenuWnd::InitWindow()
{
	MakeItemsDelegate();
	ShowVoiceOption(false);
	//m_pm.SetDPI(50);
	//CPort* port1 = new CPort();
	//port1->SetId(2);
	//port1->SetNameIndex(2);

	//AddVideoObtainSubMenu(port1);

	//testPort = new CPort;
	//testPort->SetId(3);
	//testPort->SetNameIndex(3);
	//AddPortConfigSubMenu(testPort);

}

void CMyMenuWnd::UpdataBkColor(int focusLevel,DWORD Color1,DWORD Color2)
{ 
	int index;
	CButtonUI* pItem;
	CContainerUI* pLayout;	

	//二级模块
	CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(m_pm.FindControl(kLayoutChildrenMenuName));
	pLayout = (CContainerUI*)pTabLayout->GetItemAt(pTabLayout->GetCurSel());	

	pLayout = (CContainerUI*)pLayout->GetItemAt(0);
	pLayout->SetBkColor(Color2);				//布局颜色
	for (int i = 0; i < pLayout->GetCount(); i += 2) {		//控件颜色
		pItem = (CButtonUI*)pLayout->GetItemAt(i);
		pItem->SetBkColor(Color2);
	}

	//焦点在一二级模块
	if (focusLevel == 0) {
		pLayout = (CContainerUI*)m_pm.FindControl(KLayoutParentMenuName);
		pLayout->SetBkColor(Color1);			//布局颜色
		for (int i = 0; i < pLayout->GetCount(); i += 2) {		//控件颜色
			pItem = (CButtonUI*)pLayout->GetItemAt(i);
			pItem->SetBkColor(Color1);
		}
	}	
	//焦点在三级模块
	else if (focusLevel == 1) {
		pTabLayout = static_cast<CTabLayoutUI*>(m_pm.FindControl(kLayoutChildrenMenuName));
		pLayout = (CContainerUI*)pTabLayout->GetItemAt(pTabLayout->GetCurSel());
		pTabLayout = (CTabLayoutUI*)pLayout->GetItemAt(1);  //Body Layout
		pTabLayout->SetBkColor(Color1);
	}

}

void CMyMenuWnd::MakeItemsDelegate()
{
	MenuItemMakeDelegate(KMenuItemHomeWatchName);

	SubMenuMakeDelegate(kSubMenuItemHostName);
	SubMenuMakeDelegate(kSubMenuItemSysSetName);
	SubMenuMakeDelegate(kSubMenuItemAwTimeName);
	SubMenuMakeDelegate(kSubMenuItemAlarmVoiceName);
	SubMenuMakeDelegate(kSubMenuItemAlarmLightName);
	SubMenuMakeDelegate(kSubMenuItemAwOnOffRecordName);

	m_pm.FindControl(kEditCtlHostNameName)->OnEvent += MakeDelegate(this, &CMyMenuWnd::OnHostName);

	m_pm.FindControl(kCameraNameEditName)->OnEvent += MakeDelegate(this, &CMyMenuWnd::OnCameraName);
	m_pm.FindControl(kCameraSwitchName)->OnEvent += MakeDelegate(this, &CMyMenuWnd::OnCameraSwitch);
	m_pm.FindControl(kCameraVolumeName)->OnEvent += MakeDelegate(this, &CMyMenuWnd::OnCameraVolume);
	m_pm.FindControl(kCameraVideoSaveName)->OnEvent += MakeDelegate(this, &CMyMenuWnd::OnCameraStore);
	m_pm.FindControl(kCameraAwName)->OnEvent += MakeDelegate(this, &CMyMenuWnd::OnCameraAutoWatch);

	m_pm.FindControl(kSysBrightName)->OnEvent += MakeDelegate(this, &CMyMenuWnd::OnSysBrightness);
	m_pm.FindControl(kSysVolumeName)->OnEvent += MakeDelegate(this, &CMyMenuWnd::OnSysVolume);
	m_pm.FindControl(kSysVersionName)->OnEvent += MakeDelegate(this, &CMyMenuWnd::OnSysVersion);
	m_pm.FindControl(kSysFactoryName)->OnEvent += MakeDelegate(this, &CMyMenuWnd::OnFactorySet);
	m_pm.FindControl(kSysHostModelName)->OnEvent += MakeDelegate(this, &CMyMenuWnd::OnSysHostModel);
	m_pm.FindControl(kSysSerialNumberName)->OnEvent += MakeDelegate(this, &CMyMenuWnd::OnSysSerialNumber);

	((CTimeSpanPickerUI*)m_pm.FindControl(kTimeSpanPickerAwName))->ItemMakeDelegate(this, &CMyMenuWnd::OnAwTime);
	m_pm.FindControl(kSwitchAlarmVoiceName)->OnEvent += MakeDelegate(this, &CMyMenuWnd::OnAlarmVoiceSwitch);
	m_pm.FindControl(kOptionAlarmVoiceDefaultName)->OnEvent += MakeDelegate(this, &CMyMenuWnd::OnAlarmVoiceOption);
	m_pm.FindControl(kBtAlarmVoiceRecordName)->OnEvent += MakeDelegate(this, &CMyMenuWnd::OnAlarmVoiceRecord);
	m_pm.FindControl(kAlarmLightName)->OnEvent += MakeDelegate(this, &CMyMenuWnd::OnAlarmLight);
}

void CMyMenuWnd::MenuItemMakeDelegate(const TCHAR* const Name)
{
	static_cast<CMenuItemUI*>(m_pm.FindControl(Name))->OnEvent += MakeDelegate(this, &CMyMenuWnd::OnMenuItem);
}

void CMyMenuWnd::SubMenuMakeDelegate(const TCHAR* const Name)
{
	static_cast<CSubMenuItemUI*>(m_pm.FindControl(Name))->OnEvent += MakeDelegate(this, &CMyMenuWnd::OnSubMenuItem);
}


bool CMyMenuWnd::OnMenuItem(void * param)
{
	TEventUI* pMsg = (TEventUI*)param;
	CMenuItemUI* Item = (CMenuItemUI*)pMsg->pSender;

	if (pMsg->Type == UIEVENT_KEYDOWN) {
		if (pMsg->wParam == VK_RIGHT) {
			if (_tcscmp(Item->GetName(), KMenuItemHomeWatchName) == 0) {

				return false;
			}
		}
	}
	return true;
}

/*************** Delegate Functions*********************/
bool CMyMenuWnd::OnSubMenuItem(void * param)
{
	TEventUI* pMsg = (TEventUI*)param;
	CSubMenuItemUI* pItem = (CSubMenuItemUI*)pMsg->pSender;
	CDuiString Name = pItem->GetName();
	switch (pMsg->Type) {
	case UIEVENT_KEYDOWN:
		switch (pMsg->wParam) {
		case VK_BACK:
			if (pItem->GetOnwerMenuItemName()) {
				m_pm.FindControl(pItem->GetOnwerMenuItemName())->SetFocus();
			}
			break;
		//----------------------------------------------
		case VK_RIGHT:
			if (_tcsicmp(Name, kSubMenuItemHostName) == 0) {
				m_pm.FindControl(kEditCtlHostNameName)->SetFocus();
			}
			else if (_tcsicmp(Name, kSubMenuItemSysSetName) == 0) {
				m_pm.FindControl(kSysVolumeName)->SetFocus();
			}
			else if (_tcsicmp(Name, kSubMenuItemAwTimeName) == 0) {
				((CTimeSpanPickerUI*)m_pm.FindControl(kTimeSpanPickerAwName))->GetItemAt(0)->SetFocus();
			}
			else if (_tcsicmp(Name, kSubMenuItemAlarmVoiceName) == 0) {
				m_pm.FindControl(kSwitchAlarmVoiceName)->SetFocus();
			}
			else if (_tcsicmp(Name, kSubMenuItemAlarmLightName) == 0) {
				m_pm.FindControl(kAlarmLightName)->SetFocus();
			}
			else if (_tcsicmp(Name, kSubMenuItemAwOnOffRecordName) == 0) {
				
			}
			return false;
			break;

		}
		break;
	}
	return true;
}

bool CMyMenuWnd::OnHostName(void * param)
{
	TEventUI* pMsg = (TEventUI*)param;
	CEditExUI* Item = (CEditExUI*)pMsg->pSender;
	switch (pMsg->Type){
	case UIEVENT_KEYDOWN:
		switch (pMsg->wParam) {
		case VK_BACK:
			m_pm.FindControl(kSubMenuItemHostName)->SetFocus();
			break;
		//------------------------------
		case VK_RETURN:
			Item->SetStatus(true);
			m_pm.FindControl(kKeyboardName)->SetVisible(true);
			m_pm.FindControl(kKeyboardName)->SetFocus();
			m_pm.FindControl(kLayoutPromptName)->SetVisible(true);
			break;
		}
		break;
	//----------------------------------------------
	case UIEVENT_SETFOCUS:
		Item->SetStatus(false);
		if (m_pm.FindControl(kKeyboardName)->IsVisible()) {
			m_pm.FindControl(kKeyboardName)->SetVisible(false);
			m_pm.FindControl(kLayoutPromptName)->SetVisible(false);
		}
		break;
	}


	return true;
}

bool CMyMenuWnd::OnAlarmFileList(void * param)
{
	TEventUI* pMsg = (TEventUI*)param;

	return false;
}

bool CMyMenuWnd::OnNormalFileList(void * param)
{
	TEventUI* pMsg = (TEventUI*)param;
	return false;
}

bool CMyMenuWnd::OnCameraName(void * param)
{
	TEventUI* pMsg = (TEventUI*)param;
	switch (pMsg->Type) {
	case UIEVENT_KEYDOWN:
		switch (pMsg->wParam){
		case VK_BACK:
			/*if(cameraname is change){
				if(ok==messagebox()){
					save
				}
				else{
					
				}
			}*/

			//SetFocus to submenuItem
			break;
		//----------------------------------------------
		case VK_DOWN:
			m_pm.FindControl(kCameraSwitchName)->SetFocus();
			break;
		}
		break;
	}
	return true;
}

bool CMyMenuWnd::OnCameraVolume(void * param)
{
	TEventUI* pMsg = (TEventUI*)param;
	CSliderExUI* pItem = (CSliderExUI*)pMsg->pSender;
	switch (pMsg->Type) {
	case UIEVENT_KEYDOWN:
		switch (pMsg->wParam) {
		case VK_UP:
			m_pm.FindControl(kCameraSwitchName)->SetFocus();
			break;
		//----------------------------------------------
		case VK_LEFT:
			if (pItem->GetValue() > pItem->GetMinValue()) {
				pItem->SetValue(pItem->GetValue() - 1);
				//save();
			}
			break;
		//----------------------------------------------
		case VK_RIGHT:
			if (pItem->GetValue() < pItem->GetMaxValue()) {
				pItem->SetValue(pItem->GetValue() + 1);
				// save
			}
			break;
		//----------------------------------------------
		case VK_DOWN:
			m_pm.FindControl(kCameraVideoSaveName)->SetFocus();
			break;
		//----------------------------------------------
		case VK_BACK:
			/*
			if(cameraname is change){
				if(idok==messagebox()){
					save()
				}
				else{

				}
			}
			setfocus to submenuitem
			*/
			break;
		}

		return false;
		break;
	}

	return true;
}

bool CMyMenuWnd::OnCameraSwitch(void * param)
{
	TEventUI* pMsg = (TEventUI*)param;
	CSwitchExUI* pItem = (CSwitchExUI*)pMsg->pSender;
	switch (pMsg->Type) {
	case UIEVENT_KEYDOWN:
		switch (pMsg->wParam) {
		case VK_UP:
			m_pm.FindControl(kCameraNameEditName)->SetFocus();
			break;

		case VK_DOWN:
			m_pm.FindControl(kCameraVolumeName)->SetFocus();
			break;

		case VK_BACK:
			/*
			if(cameraname is change){
				if(idok==messagebox()){
					save()
				}
				else{

				}
			}
			setfocus to submenuitem
			*/
			break;

		case VK_LEFT:
			if (pItem->GetValue() == true) {
				/*if(id==messagebox()){
					pItem->SetValue(false);
					pItem->invalidate();
					save();
				}
				*/
			}
			break;

		case VK_RIGHT:
			if (pItem->GetValue() == false) {
				pItem->SetValue(true);
				pItem->Invalidate();
				//save();
			}
			break;
		}
		
		return false;
		break;
	}
	return true;
}

bool CMyMenuWnd::OnCameraStore(void * param)
{
	TEventUI* pMsg = (TEventUI*)param;
	switch (pMsg->Type) {
	case UIEVENT_KEYDOWN:
		switch (pMsg->wParam) {
		case VK_UP:
			m_pm.FindControl(kCameraVolumeName)->SetFocus();
			break;

		//----------------------------------------------
		case VK_DOWN:
			m_pm.FindControl(kCameraAwName)->SetFocus();
			break;
		//----------------------------------------------
		case VK_BACK:
			/*
			if(cameraname is change){
				if(idok==messagebox()){
					save()
				}
				else{

				}
			}
			setfocus to submenuitem
			*/
			break;
		}

		return false;

		break;
	}
	return true;
}

bool CMyMenuWnd::OnCameraAutoWatch(void * param)
{
	TEventUI* pMsg = (TEventUI*)param;
	CLabelExUI* pItem = (CLabelExUI*)pMsg->pSender;
	switch (pMsg->Type) {
	case UIEVENT_KEYDOWN:
		switch (pMsg->wParam) {
		case VK_UP:
			m_pm.FindControl(kCameraVideoSaveName)->SetFocus();
			break;
		//----------------------------------------------
		case VK_RETURN:
			if (!pItem->GetValue()) {
				pItem->SetValue(true);
				pItem->Invalidate();
				//save();
			}
			else {
				/*
				if(idok==messagebox()){
					pItem->SetValue(false);
					pItem->invalidate();
					save();
				}
				*/
			}
			break;
		//----------------------------------------------
		case VK_BACK:
			/*
			if(cameraname is change){
				if(idok==messagebox()){
					save()
				}
				else{
				
				}
			}
			setfocus to submenuitem
			*/
			break;
		}

		return false;

		break;
	}
	return true;
}


bool CMyMenuWnd::OnSysBrightness(void * param)
{
	TEventUI* pMsg = (TEventUI*)param;
	CSliderExUI* pItem = (CSliderExUI*)pMsg->pSender;
	switch (pMsg->Type) {
	case UIEVENT_KEYDOWN:
		switch (pMsg->wParam) {
		case VK_LEFT:
			if (pItem->GetValue() > pItem->GetMinValue()) {
				pItem->SetValue(pItem->GetValue() - 1);
			}
			break;
		//----------------------------------------------
		case VK_RIGHT:
			if (pItem->GetValue() < pItem->GetMaxValue()) {
				pItem->SetValue(pItem->GetValue() + 1);
			}
			break;
		//----------------------------------------------
		case VK_DOWN:
			//setfocus to sysset voice
			break;
		//----------------------------------------------
		case VK_BACK:
			break;
		}

		return false;
		break;
	}
	return true;
}

bool CMyMenuWnd::OnSysVolume(void * param)
{
	TEventUI* pMsg = (TEventUI*)param;
	CSliderExUI* pItem = (CSliderExUI*)pMsg->pSender;
	switch (pMsg->Type) {
	case UIEVENT_KEYDOWN:
		switch (pMsg->wParam) {
		case VK_UP:
			//setfocus to sysset brightness
			break;
		//----------------------------------------------
		case VK_LEFT:
			if (pItem->GetValue() > pItem->GetMinValue()) {
				pItem->SetValue(pItem->GetValue() - 1);
			}
			break;
		//----------------------------------------------
		case VK_RIGHT:
			if (pItem->GetValue() < pItem->GetMaxValue()) {
				pItem->SetValue(pItem->GetValue() + 1);
			}
			break;
		//----------------------------------------------
		case VK_DOWN:
			m_pm.FindControl(kSysVersionName)->SetFocus();
			break;
		//----------------------------------------------
		case VK_BACK:
			/*
			if(volume is change){
				if(idok==messagebox()){
					save()
				}
				else{

				}
			}
			*/
			m_pm.FindControl(kSubMenuItemSysSetName)->SetFocus();
			break;
		}

		return false;
		break;
	}
	return true;
}

bool CMyMenuWnd::OnSysVersion(void * param)
{
	TEventUI* pMsg = (TEventUI*)param;
	CLabelExUI* pItem = (CLabelExUI*)pMsg->pSender;
	switch (pMsg->Type) {
	case UIEVENT_KEYDOWN:
		switch (pMsg->wParam) {
		case VK_UP:
			m_pm.FindControl(kSysVolumeName)->SetFocus();
			break;
		//----------------------------------------------
		case VK_DOWN:
			m_pm.FindControl(kSysFactoryName)->SetFocus();
			break;
		//----------------------------------------------
		case VK_BACK:
			/*
			if(volume is change){
			if(idok==messagebox()){
			save()
			}
			else{

			}
			}
			*/
			m_pm.FindControl(kSubMenuItemSysSetName)->SetFocus();
			break;
		//----------------------------------------------
		}

		return false;
		break;
	}
	return true;
}

bool CMyMenuWnd::OnFactorySet(void * param)
{
	TEventUI* pMsg = (TEventUI*)param;
	CLabelExUI* pItem = (CLabelExUI*)pMsg->pSender;
	switch (pMsg->Type) {
	case UIEVENT_KEYDOWN:
		switch (pMsg->wParam) {
		case VK_UP:
			m_pm.FindControl(kSysVersionName)->SetFocus();
			break;
			//----------------------------------------------
		case VK_DOWN:
			//setfocus to factory set
			m_pm.FindControl(kSysHostModelName)->SetFocus();;
			break;
			//----------------------------------------------
		case VK_BACK:
			/*
			if(volume is change){
				if(idok==messagebox()){
					save()
				}
				else{

				}
			}
			*/
			m_pm.FindControl(kSubMenuItemSysSetName)->SetFocus();
			break;
			//----------------------------------------------
		}

		return false;
		break;
	}
	return true;
}

bool CMyMenuWnd::OnSysHostModel(void * param)
{
	TEventUI* pMsg = (TEventUI*)param;
	CLabelExUI* pItem = (CLabelExUI*)pMsg->pSender;
	switch (pMsg->Type) {
	case UIEVENT_KEYDOWN:
		switch (pMsg->wParam) {
		case VK_UP:
			m_pm.FindControl(kSysFactoryName)->SetFocus();
			break;
			//----------------------------------------------
		case VK_DOWN:
			m_pm.FindControl(kSysSerialNumberName)->SetFocus();;
			break;
			//----------------------------------------------
		case VK_BACK:
			/*
			if(volume is change){
			if(idok==messagebox()){
			save()
			}
			else{

			}
			}
			*/
			m_pm.FindControl(kSubMenuItemSysSetName)->SetFocus();
			break;
			//----------------------------------------------
		}

		return false;
		break;
	}
	return true;
}

bool CMyMenuWnd::OnSysSerialNumber(void * param)
{
	TEventUI* pMsg = (TEventUI*)param;
	CLabelExUI* pItem = (CLabelExUI*)pMsg->pSender;
	switch (pMsg->Type) {
	case UIEVENT_KEYDOWN:
		switch (pMsg->wParam) {
		case VK_UP:
			m_pm.FindControl(kSysHostModelName)->SetFocus();
			break;
			//----------------------------------------------
		case VK_BACK:
			/*
			if(volume is change){
			if(idok==messagebox()){
			save()
			}
			else{

			}
			}
			*/
			m_pm.FindControl(kSubMenuItemSysSetName)->SetFocus();
			break;
			//----------------------------------------------
		}

		return false;
		break;
	}
	return true;
}


bool CMyMenuWnd::OnAwTime(void * param)
{
	TEventUI* pMsg = (TEventUI*)param;
	switch (pMsg->Type) {
	case UIEVENT_KEYDOWN:
		switch (pMsg->wParam) {
		case VK_BACK:
			/*
			if(time is change){
				if(idok==messagebox()){
					save();
				}
			}
			*/
			m_pm.FindControl(kSubMenuItemAwTimeName)->SetFocus();
			break;
		}
		break;

		return false;
	}
	return true;
}

bool CMyMenuWnd::OnAlarmVoiceSwitch(void * param)
{
	TEventUI* pMsg = (TEventUI*)param;
	CSwitchExUI* pItem = (CSwitchExUI*)pMsg->pSender;
	switch (pMsg->Type) {
	case UIEVENT_KEYDOWN:
		switch (pMsg->wParam) {
		case VK_LEFT:
			if (pItem->GetValue()) {
				pItem->SetValue(false);
				pItem->Invalidate();
				
				ShowVoiceOption(false);
			}
			break;
		//----------------------------------------------
		case VK_RIGHT:
			if (!pItem->GetValue()) {
				pItem->SetValue(true);
				pItem->Invalidate();

				ShowVoiceOption(true);
			}
			break;
		//----------------------------------------------
		case VK_BACK:
			/*
			if(voice Sets is change){
				if(idok==messagebox()){
					save();
				}
			}
			*/
			m_pm.FindControl(kSubMenuItemAlarmVoiceName)->SetFocus();
			break;
		//----------------------------------------------
		case VK_DOWN:
			if(pItem->GetValue())
				m_pm.FindControl(kOptionAlarmVoiceDefaultName)->SetFocus();
			break;
		}
		break;

		return false;
	}
	return true;
}

bool CMyMenuWnd::OnAlarmVoiceOption(void * param)
{
	TEventUI* pMsg = (TEventUI*)param;
	COptionExUI* pItem = (COptionExUI*)pMsg->pSender;
	switch (pMsg->Type) {
	case UIEVENT_KEYDOWN:
		switch (pMsg->wParam) {
		case VK_UP:
			if (_tcscmp(pItem->GetName(),kOptionAlarmVoiceDefaultName)==0) {
				m_pm.FindControl(kSwitchAlarmVoiceName)->SetFocus();
			}
			else {
				m_pm.FindControl(kOptionAlarmVoiceDefaultName)->SetFocus();
			}
			break;
		//----------------------------------------------
		case VK_DOWN:
			if (_tcscmp(pItem->GetName(),kOptionAlarmVoiceDefaultName)==0) {
				if (m_pm.FindControl(kOptionAlarmVoiceRecordName)) {
					m_pm.FindControl(kOptionAlarmVoiceRecordName)->SetFocus();
				}
				else {
					m_pm.FindControl(kBtAlarmVoiceRecordName)->SetFocus();
				}
			}
			else {
				m_pm.FindControl(kBtAlarmVoiceRecordName)->SetFocus();
			}
			break;

		case VK_BACK:
			/*
			if(voice Sets is change){
				if(idok==messagebox()){
					save();
				}
			}
			*/
			m_pm.FindControl(kSubMenuItemAlarmVoiceName)->SetFocus();
			break;
		}
		break;

		return false;
	}
	return true;
}

bool CMyMenuWnd::OnAlarmVoiceRecord(void * param)
{
	TEventUI* pMsg = (TEventUI*)param;
	CButtonUI* pItem = (CButtonUI*)pMsg->pSender;
	switch (pMsg->Type) {
	case UIEVENT_KEYDOWN:
		switch (pMsg->wParam) {
		case VK_UP:
			if (m_pm.FindControl(kOptionAlarmVoiceRecordName)) {
				m_pm.FindControl(kOptionAlarmVoiceRecordName)->SetFocus();
			}
			else {
				m_pm.FindControl(kOptionAlarmVoiceDefaultName)->SetFocus();
			}
			break;

		//----------------------------------------------
		case VK_BACK:
			/*
			if(voice Sets is change){
			if(idok==messagebox()){
			save();
			}
			}
			*/
			m_pm.FindControl(kSubMenuItemAlarmVoiceName)->SetFocus();
			break;
		//----------------------------------------------
		case VK_RETURN:
			//AddAlarmVoice();
			break;
		}
		break;

		return false;
	}
	return true;
}

bool CMyMenuWnd::OnAlarmLight(void * param)
{
	TEventUI* pMsg = (TEventUI*)param;
	CSwitchExUI* pItem = (CSwitchExUI*)pMsg->pSender;
	switch (pMsg->Type) {
	case UIEVENT_KEYDOWN:
		switch (pMsg->wParam) {
		case VK_LEFT:
			if (pItem->GetValue()) {
				pItem->SetValue(false);
				pItem->Invalidate();
			}
			break;
		//----------------------------------------------
		case VK_RIGHT:
			if (!pItem->GetValue()) {
				pItem->SetValue(true);
				pItem->Invalidate();
			}
			break;
		//----------------------------------------------
		case VK_BACK:
			/*
			if(Sets is change){
				if(idok==messagebox()){
					save();
				}
			}
			*/
			m_pm.FindControl(kSubMenuItemAlarmLightName)->SetFocus();
			break;
		}
		break;

		return false;
	}
	return true;
}


bool CMyMenuWnd::OnRecords(void * param)
{
	TEventUI* pMsg = (TEventUI*)param;
	return true;
}

bool CMyMenuWnd::OnHome(void * param)
{
	TEventUI* pMsg = (TEventUI*)param;
	return true;
}


int CMyMenuWnd::InsertSubMenuAt(CContainerUI * pLayout, CPort * pPort)
{
	CSubMenuItemUI* pItem=NULL;
	CPort* tagPort;
	int index=0;
	if (!pLayout->GetCount()) return 0;

	for (index = 0; index < pLayout->GetCount(); index += 2) {
		pItem = (CSubMenuItemUI*)pLayout->GetItemAt(index);
		tagPort = (CPort*)pItem->GetTag();
		if (!tagPort)	continue;
		if (tagPort->GetId() > pPort->GetId()) {
			return index;
		}
	}

	if (_tcscmp(pItem->GetName(), kSubMenuItemSysSetName) == 0)
		return index - 2;
	return index;
}

CSubMenuItemUI * CMyMenuWnd::AddSubMenuItem(CDuiString layoutName, CPort * pPort)
{
	CSubMenuItemUI* pItem;
	CContainerUI* pLayout = (CContainerUI*)(m_pm.FindControl(layoutName));
	pItem = new CSubMenuItemUI(pLayout, pPort->GetName(), InsertSubMenuAt(pLayout, pPort));
	pItem->SetTag((UINT_PTR)pPort);
	pItem->OnEvent += MakeDelegate(this, &CMyMenuWnd::OnSubMenuItem);
	return pItem;
}

// SubMenuItem Add Or Delete
void CMyMenuWnd::AddAlarmSubMenu(CPort* pPort)
{
	CSubMenuItemUI* pItem = NULL;
	pItem = AddSubMenuItem(kLayoutSubAlarmVideoName, pPort);
	pItem->SetOnwerMenuItemName(kMenuItemAlarmVideoName);
}

void CMyMenuWnd::AddVideoObtainSubMenu(CPort* pPort)
{
	CSubMenuItemUI* pItem = NULL;
	pItem = AddSubMenuItem(kLayoutSubVideoObtainName, pPort);
	pItem->SetOnwerMenuItemName(kMenuItemVideoObtainName);
}

void CMyMenuWnd::AddPortConfigSubMenu(CPort* pPort)
{
	CSubMenuItemUI* pItem = NULL;
	pItem = AddSubMenuItem(kLayoutSubHostSetName, pPort);
	pItem->SetOnwerMenuItemName(kMenuItemHostSetName);
	pItem->SetUserData(_T("PortConfig"));
	pItem->BindTabLayoutName(kBodyLayoutHostSetName);
	pItem->BindTabIndex(1);
}

void CMyMenuWnd::DeleteSubMenuItem(CDuiString layoutName, CPort * pPort)
{
	int index = 0;
	CSubMenuItemUI* pItem;
	CPort* tagPort;
	CContainerUI* pLayout = (CContainerUI*)(m_pm.FindControl(layoutName));
	for (index = 0; index < pLayout->GetCount(); index += 2) {
		pItem = (CSubMenuItemUI*)pLayout->GetItemAt(index);
		tagPort = (CPort*)pItem->GetTag();

		if (tagPort == pPort) {
			int order = pLayout->GetItemIndex(pItem);
			pLayout->RemoveAt(order);
			pLayout->RemoveAt(order);
			return;
		}
	}
}

void CMyMenuWnd::DeleteAlarmSubMenu(CPort* pPort)
{
	DeleteSubMenuItem(kLayoutSubAlarmVideoName, pPort);
}

void CMyMenuWnd::DeleteVideoObtainSubMenu(CPort* pPort)
{
	DeleteSubMenuItem(kLayoutSubVideoObtainName, pPort);
}

void CMyMenuWnd::DeletePortConfigSubMenu(CPort* pPort)
{
	DeleteSubMenuItem(kLayoutSubHostSetName, pPort);
}
 
bool CMyMenuWnd::AlarmVoideIsChange()
{
	CStdPtrArray* aOptionGroup = m_pm.GetOptionGroup(kAlarmVoiceGroupName);
	int sel = 0;
	for (; sel < aOptionGroup->GetSize(); sel++) {
		COptionUI* pControl = static_cast<COptionUI*>(aOptionGroup->GetAt(sel));
		if (pControl->IsSelected()) {
			break;
		}
	}
	return false;
}

void CMyMenuWnd::AddAlarmVoice()
{
	CVerticalLayoutUI* pParentLayout;
	CVerticalLayoutUI* pLayout = (CVerticalLayoutUI*)m_pm.FindControl(_T("layout_alarm_voice"));
	COptionExUI* pItem = new COptionExUI();

	pItem->SetName(kOptionAlarmVoiceRecordName);
	pLayout->AddAt(pItem, 3);
	pItem->SetAttribute(_T("style"), _T("style_alarm_voice"));
	pItem->SetText(_T("录音1"));
	pItem->OnEvent += MakeDelegate(this, &CMyMenuWnd::OnAlarmVoiceOption);

	pLayout->SetFixedHeight(pLayout->GetFixedHeight() + pItem->GetFixedHeight());
	pParentLayout = (CVerticalLayoutUI*)pLayout->GetParent();
	pParentLayout->SetFixedHeight(pParentLayout->GetFixedHeight()+pItem->GetFixedHeight());
}



void CMyMenuWnd::ShowVoiceOption(bool isShow)
{
	CContainerUI* pContain2 = (CContainerUI*)m_pm.FindControl(_T("layout_alarm_voice"));
	CContainerUI* pParentLayout = (CContainerUI*)pContain2->GetParent();
	CContainerUI* pContain1 = (CContainerUI*)pParentLayout->GetItemAt(0);
	if (isShow) {
		pContain2->SetVisible(isShow);
		pParentLayout->SetFixedHeight(pContain1->GetFixedHeight()+ pContain2->GetFixedHeight());
	}
	else {
		pContain2->SetVisible(isShow);
		pParentLayout->SetFixedHeight(pContain1->GetFixedHeight());
	}
}

void CMyMenuWnd::ShowBodyLayout(bool isShow)
{
	m_pm.FindControl(kBodyLayoutAlarmVideo)->SetVisible(isShow);
	m_pm.FindControl(kBodyLayoutAwTime)->SetVisible(isShow);
	m_pm.FindControl(kBodyLayoutHostSetName)->SetVisible(isShow);
	m_pm.FindControl(kBodyLayoutVideoObtain)->SetVisible(isShow);
}

void CMyMenuWnd::FillCameraInfo(CControlUI* pItem)
{
	//填充摄像头信息： 标题、摄像头名称、摄像头开关、语音音量、存储设置、自动看船设置
	CDuiString text;
	CPort* tagPort = (CPort*)pItem->GetTag();
	if (tagPort) {
		m_pm.FindControl(kLayoutCameraSetName)->SetTag((UINT_PTR)tagPort);
		text.Format(_T("%s设置"), tagPort->GetName());
		m_pm.FindControl(kCameraLayoutTitleName)->SetText(text);
		m_pm.FindControl(kCameraNameEditName)->SetText(tagPort->GetName());
		static_cast<CSwitchExUI*>(m_pm.FindControl(kCameraSwitchName))->SetValue(true);
		static_cast<CSliderExUI*>(m_pm.FindControl(kCameraVolumeName))->SetValue(1);
		static_cast<CLabelExUI*>(m_pm.FindControl(kCameraVideoSaveName))->SetValue(true);
		static_cast<CLabelExUI*>(m_pm.FindControl(kCameraAwName))->SetValue(true);
		m_pm.Invalidate();
	}
}
